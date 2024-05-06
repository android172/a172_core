#include "memory/memory_system.hpp"

#include "memory/memory_allocators/c_allocator.hpp"
#include "memory/memory_allocators/free_list_allocator.hpp"
#include "memory/memory_allocators/stack_allocator.hpp"

#include <cstring>
#include <iostream>

namespace CORE_NAMESPACE {

#define MEMORY_SYS_LOG "MemorySystem :: "

// ////////// //
// Memory Tag //
// ////////// //

MemoryTag     MemoryTag::INVALID  = MemoryTag((MemoryTagType) -1);
MemoryTagType MemoryTag::id_count = 0;

static_assert(
    sizeof(MemoryTagType) <= MEMORY_PADDING || MEMORY_PADDING >= 8,
    "Memory padding must be at least 8."
);

// //////////////////////////// //
// MEMORY SYSTEM PUBLIC METHODS //
// //////////////////////////// //

MemorySystem::MemoryMap MemorySystem::_memory_map = {};
Allocator**             MemorySystem::_allocator_array =
    MemorySystem::initialize_allocator_array(MemorySystem::_memory_map);

void* MemorySystem::allocate(uint64 size, const MemoryTag tag) {
    auto allocator = _allocator_array[tag.id];
    return allocator->allocate(size, MEMORY_PADDING);
}
void MemorySystem::deallocate(void* ptr, const MemoryTag tag) {
    auto allocator = _allocator_array[tag.id];
    if (!allocator->owns(ptr)) {
        std::cerr << MEMORY_SYS_LOG << "Deallocation with wrong memory tag."
                  << std::endl;
        exit(EXIT_FAILURE);
    }
    allocator->free(ptr);
}

void MemorySystem::reset_memory(const MemoryTag tag) {
    auto allocator = _allocator_array[tag.id];
    allocator->reset();
}

#define convert_to_unit(u)                                                     \
    if (total >= 1024) {                                                       \
        total /= 1024;                                                         \
        used /= 1024;                                                          \
        peek /= 1024;                                                          \
        unit = #u;                                                             \
    }

void MemorySystem::print_usage(const MemoryTag tag) {
    auto allocator = _allocator_array[tag.id];

    // Calculate total use
    float64 used  = allocator->used();
    float64 total = allocator->total_size();
    float64 peek  = allocator->peak();
    float64 ratio = used / total;

    // Compute unit
    std::string unit = "bytes";
    convert_to_unit(KB);
    convert_to_unit(MB);
    convert_to_unit(GB);
    convert_to_unit(TB);

    // Print
    std::cout << "========================" << std::endl;
    std::cout << used << unit << " / " << total << unit << std::endl;
    std::cout << ratio * 100 << "% / 100%" << std::endl;
    std::cout << "peek : " << peek << unit << std::endl;
    std::cout << "========================" << std::endl;
}

MemoryTag MemorySystem::get_owner(void* p) {
    const auto address = (uint64) p;

    // Get tag
    const auto tag = _memory_map.get_first_before(address);
    if (tag == MemoryTag::INVALID) return tag;

    // Check if tag owns this memory
    if (!_allocator_array[tag.id]->owns(p)) return MemoryTag::INVALID;

    return tag;
}

void MemorySystem::register_tag(const MemoryTag& tag, Allocator& allocator) {
    if (_aa_size < MemoryTag::id_count) update_allocator_array();
    _allocator_array[tag.id]       = &allocator;
    _memory_map[allocator.start()] = tag;
}

// ///////////////////////////// //
// MEMORY SYSTEM PRIVATE METHODS //
// ///////////////////////////// //

// Allocator initializations
#define assign_allocator(tag, allocator)                                       \
    allocator_array[BaseMemoryTags.tag.id] = allocator;                        \
    memory_map[allocator->start()]         = BaseMemoryTags.tag

Allocator** MemorySystem::initialize_allocator_array(MemoryMap& memory_map) {
    const auto allocator_array = new Allocator*[MemoryTag::id_count]();

    // Define used allocators
    auto unknown_allocator = new CAllocator();
    auto temp_allocator    = new StackAllocator(KB);
    auto general_allocator = new FreeListAllocator(
        128 * KB, FreeListAllocator::PlacementPolicy::FindFirst
    );

    // Initialize these allocators
    unknown_allocator->init();
    temp_allocator->init();
    general_allocator->init();

    // Assign allocators
    assign_allocator(Unknown, unknown_allocator);
    assign_allocator(Temp, temp_allocator);
    assign_allocator(Array, general_allocator);
    assign_allocator(List, general_allocator);
    assign_allocator(Map, general_allocator);
    assign_allocator(Set, general_allocator);
    assign_allocator(String, general_allocator);
    assign_allocator(Callback, general_allocator);

    return allocator_array;
}

void MemorySystem::update_allocator_array() {
    const auto old_aa = _allocator_array;
    // Declare new AA
    _allocator_array  = new Allocator*[MemoryTag::id_count]();
    // Copy data over
    memcpy(_allocator_array, old_aa, _aa_size);
    // Update size
    _aa_size = MemoryTag::id_count;
}

// -----------------------------------------------------------------------------
// Memory map
// -----------------------------------------------------------------------------

MemoryTag MemorySystem::MemoryMap::get_first_before(const uint64 address) {
    // Check for nullptr
    if (address == 0) return MemoryTag::INVALID;

    // Check if alive
    if (_dying) return MemoryTag::INVALID;

    // Find adequate memory tag
    auto it = lower_bound(address);
    if (it == end()) it--;
    else if (it->first != address) {
        // This is before begin?
        if (it == begin()) return MemoryTag::INVALID;
        it--;
    }
    const auto tag = it->second;

    return tag;
}

} // namespace CORE_NAMESPACE

using namespace CORE_NAMESPACE;

// New
void* operator new(std::size_t size, MemoryTag tag) {
    return MemorySystem::allocate(size, tag);
}
void* operator new[](std::size_t size, const MemoryTag tag) {
    return operator new(size, tag);
}

// Delete
void operator delete(void* p) noexcept {
    const auto tag = MemorySystem::get_owner(p);
    if (tag != MemoryTag::INVALID) MemorySystem::deallocate(p, tag);
    else free(p);
}
void operator delete[](void* p) noexcept { ::operator delete(p); }