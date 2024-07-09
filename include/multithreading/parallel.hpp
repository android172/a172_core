/**
 * @file parallel.hpp
 * @author Android172 (android172unity@gmail.com)
 * @brief Provides a list of parallel algorithms in terms of `parallel`
 * namespace.
 * @version 0.1
 * @date 2024-07-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <functional>
#include <mutex>

namespace CORE_NAMESPACE {

/**
 * @brief Static class holding a list of functions for parallel multithreaded
 * execution of code.
 */
namespace parallel {
    // -------------------------------------------------------------------------
    // Mutex
    // -------------------------------------------------------------------------

    /**
     * @brief Mutex lock. Allows only one thread to enter code between @p
     * `lock()` and @p `unlock()` calls.
     */
    class Mutex : public std::mutex {};

    // -------------------------------------------------------------------------
    // Range
    // -------------------------------------------------------------------------

    // TODO: Reintroduce old code

    template<typename T>
    class Range { //: public tbb::blocked_range<T> {
      public:
        // using tbb::blocked_range<T>::blocked_range;

        // Range(T begin, T end, uint64 grain_size = 1)
        //     : tbb::blocked_range<T>(begin, end, grain_size) {}

        // template<
        //     typename ArrayType,
        //     typename = std::enable_if_t<has_iterator<ArrayType>::value>>
        // Range(ArrayType array_like, uint64 grain_size = 1)
        //     : Range<T>(array_like.begin(), array_like.end(), grain_size) {}
    };

    template<typename T, typename V = T>
    class Range2D { //: public tbb::blocked_range2d<T, V> {
      public:
        // using tbb::blocked_range2d<T>::blocked_range2d;
    };

    template<typename T, typename V = T, typename W = V>
    class Range3D { //: public tbb::blocked_range3d<T, V, W> {
      public:
        // using tbb::blocked_range3d<T>::blocked_range3d;
    };

    // -------------------------------------------------------------------------
    // Parallel algorithms
    // -------------------------------------------------------------------------

    /**
     * @brief Sort data from range [begin, end) in increasing order. Uses
     * default comparator (less than).
     * @tparam RandomAccessIterator Iterator type
     * @param begin Iterator to the first element of sort range
     * @param end Iterator one past the last element of sort range
     */
    template<typename RandomAccessIterator>
    static void sort(RandomAccessIterator begin, RandomAccessIterator end) {
        // tbb::parallel_sort(begin, end);
    }
    /**
     * @brief Sort data from range [begin, end) in order as defined by given
     * comparator.
     * @tparam RandomAccessIterator Iterator type
     * @tparam Compare Comparator type
     * @param begin Iterator to the first element of sort range
     * @param end Iterator one past the last element of sort range
     * @param comp Custom comparator
     */
    template<typename RandomAccessIterator, typename Compare>
    static void sort(
        RandomAccessIterator begin,
        RandomAccessIterator end,
        const Compare&       comp
    ) {
        // tbb::parallel_sort(begin, end, comp);
    }

    /**
     * @brief Sort data from range [begin, end] in order as defined by given
     * comparator.
     * @tparam T Data type
     * @tparam Compare Comparator type
     * @param begin Pointer to the first element of sort range
     * @param end Pointer to the last element of sort range
     * @param comp Custom comparator
     */
    template<typename T, typename Compare>
    static void sort(T* begin, T* end, const Compare& comp = std::less<T>()) {
        // tbb::parallel_sort(begin, end, comp);
    }

    namespace __detail__ {
        template<typename T>
        struct Loop {
            Loop() {};

            T from;
            T to;

            Loop& set_from(T from) {
                this->from = from;
                return *this;
            }
            Loop& set_to(T to) {
                this->to = to;
                return *this;
            }

            void operator=(std::function<void(T)> callback) {
                // tbb::parallel_for(
                //     tbb::blocked_range<T>(from, to),
                //     [&callback](tbb::blocked_range<T> range) {
                //         for (T i = range.begin(); i != range.end(); i++)
                //             callback(i);
                //     }
                // );
            }

            void operator=(std::function<void(Range<T>)> callback) {
                // tbb::parallel_for(
                //     Range<T>(from, to),
                //     [&callback](Range<T> range) { callback(range); }
                // );
            }
        };

        template<typename T>
        static Loop<T> get_for_each_2(const T& type) {
            return Loop<T>();
        }
        template<typename T>
        static Loop<T> get_for_each_2(T&& type) {
            return Loop<T>();
        }

        template<typename T>
        static Loop<typename T::iterator> get_for_each_3(const T& type) {
            auto loop = Loop<typename T::iterator>();
            loop.from = type.begin();
            loop.to   = type.end();
            return loop;
        }
        template<typename T>
        static Loop<typename T::iterator> get_for_each_3(T&& type) {
            auto loop = Loop<typename T::iterator>();
            loop.from = type.begin();
            loop.to   = type.end();
            return loop;
        }

        template<typename T>
        static Loop<T> get_for_range(const Range<T>& range) {
            auto loop = Loop<T>();
            loop.from = range.begin();
            loop.to   = range.end();
            return loop;
        }
        template<typename T>
        static Loop<T> get_for_range(Range<T>&& range) {
            auto loop = Loop<T>();
            loop.from = range.begin();
            loop.to   = range.end();
            return loop;
        }
    } // namespace __detail__

} // namespace parallel

#define for_loop_1(variable, begin, end)                                       \
    __detail__::get_for_each_2(begin).set_from(begin).set_to(end) =            \
        (std::function<void(variable)>) [&](variable)

#define for_loop_2(variable, collection)                                       \
    __detail__::get_for_each_3(collection) =                                   \
        (std::function<void(variable)>) [&](variable)

#define GET_FOR_LOOP_MACRO(_1, _2, _3, NAME, ...) NAME

#define for_each(...)                                                          \
    GET_FOR_LOOP_MACRO(__VA_ARGS__, for_loop_1, for_loop_2)(__VA_ARGS__)

#define for_range(subrange, range)                                             \
    __detail__::get_for_range(range) =                                         \
        (std::function<void(parallel::subrange)>) [&](parallel::subrange)

} // namespace CORE_NAMESPACE