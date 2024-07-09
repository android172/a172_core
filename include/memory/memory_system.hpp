#pragma once

#include <map>
#include <type_traits>
#include <memory>

#include "memory_allocators/allocator.hpp"

namespace CORE_NAMESPACE {

#define MEMORY_PADDING 8

// Size reference points
#define KB 1024
#define MB KB * 1024
#define GB MB * 1024

/** @brief Defines bit count used for different memory tags; By default 16 */
typedef uint16 MemoryTagType;

/**
 * @brief Memory tag used for allocation. Indicates what custom allocator, if
 * any, should be used for given allocation. Usually used with @p new.
 */
struct MemoryTag {
  public:
    MemoryTagType id;

    static MemoryTagType id_count;
    static MemoryTag     INVALID;

    MemoryTag() : id(id_count++) {}

    bool operator==(const MemoryTag& oth) const { return this->id == oth.id; }
    bool operator!=(const MemoryTag& oth) const { return this->id != oth.id; }

  private:
    MemoryTag(MemoryTagType initial_id) : id(initial_id) {}
};

/**
 * @brief Structure from which all other Memory Tag structures inherit. You
 * should never inherit this structure, as its missing some key tags. Inherit
 * BaseMemoryTags instead if you want extended memory system functionality.
 */
struct MemoryTagStruct {
    MemoryTagStruct() { MemoryTag::id_count = 0; }
};

/**
 * @brief Structure containing all base memory tags. This is complete list of
 * all tags used by core. Additional tags can be attached by inheriting from
 * this class. For tag extension to be effective memory system needs to be
 * informed of it. This is done by registration of each new tag.
 */
struct BaseMemoryTags : public MemoryTagStruct {
    // For temporary use. Should be assigned one of the below or have a new
    // tag created.
    MemoryTag Unknown;
    // Temporary data
    MemoryTag Temp;
    // Data types
    MemoryTag Array;
    MemoryTag List;
    MemoryTag Map;
    MemoryTag Set;
    MemoryTag String;
    MemoryTag Callback;

    BaseMemoryTags() { MemoryTag::id_count = 0; }
} static BaseMemoryTags = {};

/**
 * @brief Memory system. Responsible for memory management and tracking, custom
 * allocators, allocations and deallocations for the engine. Collection of
 * static methods, cant be instantiated.
 */
class MemorySystem {
  public:
    MemorySystem()  = delete;
    ~MemorySystem() = delete;

    /**
     * @brief Allocates memory chunk with a custom allocator
     * @param size Chunk size in bytes
     * @param tag Memory tag of allocator to be used
     * @return void* Reference to allocated memory
     */
    static void* allocate(uint64 size, const MemoryTag tag);
    /**
     * @brief Deallocate memory chunk with a custom allocator
     * @param ptr Reference to the first byte of the chunk
     * @param tag Memory tag of allocator to be used
     */
    static void  deallocate(void* ptr, const MemoryTag tag);
    /**
     * @brief Clears all memory for allocator with a given tag
     * @param tag Memory tag of targeted allocator
     */
    static void  reset_memory(const MemoryTag tag);
    /**
     * @brief Output usage for allocator with a given tag
     * @param tag Memory tag of targeted allocator
     */
    static void  print_usage(const MemoryTag tag);

    /**
     * @brief Get owner of a given address
     * @param ptr Address of an allocated object
     * @return MemoryTag Tag of the owning allocator
     */
    static MemoryTag get_owner(void* ptr);

    /**
     * @brief Register new tag and its allocator to memory system. This enables
     * new to preform allocations with given allocator if registered tag is
     * passed.
     * @param tag Tag we want to register.
     * @param allocator Allocator for managing memory of this tag.
     */
    static void register_tag(const MemoryTag& tag, Allocator& allocator);

  private:
    class MemoryMap : public std::map<uint64, MemoryTag> {
      public:
        using std::map<uint64, MemoryTag>::map;

        ~MemoryMap() { _dying = true; }

        MemoryTag get_first_before(const uint64 address);

      private:
        bool _dying = false;
    };

    static MemoryMap     _memory_map;
    static Allocator**   _allocator_array;
    static MemoryTagType _aa_size;

    static Allocator** initialize_allocator_array(MemoryMap& memory_map);
    static void        update_allocator_array();
};

} // namespace CORE_NAMESPACE

// New
void* operator new(std::size_t size, const CORE_NAMESPACE::MemoryTag tag);
void* operator new[](std::size_t size, const CORE_NAMESPACE::MemoryTag tag);

// Delete
void operator delete(void* p) noexcept;
void operator delete[](void* p) noexcept;

// New delete operator
template<typename T>
inline void del(T* p) {
    p->~T();
    ::operator delete(p);
}

// -----------------------------------------------------------------------------
// Typed allocator
// -----------------------------------------------------------------------------

namespace CORE_NAMESPACE {

template<class T>
struct TAllocator {
    MemoryTag tag;

    typedef T value_type;

    // default constructor not required by C++ Standard Library
    TAllocator(MemoryTag tag = BaseMemoryTags.Unknown) noexcept : tag(tag) {}

    // A converting copy constructor:
    template<class U>
    TAllocator(const TAllocator<U>& other) noexcept {
        tag = other.tag;
    }
    template<class U>
    bool operator==(const TAllocator<U>&) const noexcept {
        return true;
    }
    template<class U>
    bool operator!=(const TAllocator<U>&) const noexcept {
        return false;
    }
    T*   allocate(const std::size_t n) const;
    void deallocate(T* const p, std::size_t n) const noexcept;
};

template<class T>
T* TAllocator<T>::allocate(const std::size_t n) const {
    return (T*) operator new(n * sizeof(T), this->tag);
}
template<class T>
void TAllocator<T>::deallocate(T* const p, std::size_t n) const noexcept {
    ::operator delete(p);
}

} // namespace CORE_NAMESPACE

// Make
namespace std {
/// std::make_unique for single objects
template<typename _Tp, typename... _Args>
inline unique_ptr<_Tp> make_unique(
    CORE_NAMESPACE::MemoryTag tag, _Args&&... __args
) {
    return unique_ptr<_Tp>(new (tag) _Tp(std::forward<_Args>(__args)...));
}

/// std::make_unique for arrays of unknown bound
template<typename _Tp>
inline unique_ptr<_Tp[]> make_unique(
    CORE_NAMESPACE::MemoryTag tag, size_t __num
) {
    return unique_ptr<_Tp>(new (tag) remove_extent_t<_Tp>[__num]());
}

/**
 *  @brief  Create an object that is owned by a shared_ptr.
 *  @param  tag  Memory tag
 *  @param  __args  Arguments for the @a _Tp object's constructor.
 *  @return A shared_ptr that owns the newly created object.
 *  @throw  std::bad_alloc, or an exception thrown from the
 *          constructor of @a _Tp.
 */
template<typename _Tp, typename... _Args>
inline shared_ptr<_Tp> make_shared(
    CORE_NAMESPACE::MemoryTag tag, _Args&&... __args
) {
    typedef typename std::remove_cv<_Tp>::type _Tp_nc;
    return std::allocate_shared<_Tp>(
        CORE_NAMESPACE::TAllocator<_Tp_nc>(tag), std::forward<_Args>(__args)...
    );
}
} // namespace std