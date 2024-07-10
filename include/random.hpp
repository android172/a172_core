/**
 * @file random.hpp
 * @author Android172 (android172unity@gmail.com)
 * @brief Random functions
 * @version 0.1
 * @date 2024-07-10
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include "common/types.hpp"

#include <random>

namespace CORE_NAMESPACE {

/**
 * @brief Static class, acts as a container for randomness related functions.
 * Holds bunch of useful random generation algorithms.
 */
class Random {
  public:
    static void set_seed(const CORE_NAMESPACE::uint32 seed);

    /**
     * @brief Generate random uint8 in range [min, max] with uniform
     * distribution.
     */
    static CORE_NAMESPACE::uint8  uint8( //
        CORE_NAMESPACE::uint8 min = 0,
        CORE_NAMESPACE::uint8 max = uint8_max
    );
    /**
     * @brief Generate random uint16 in range [min, max] with uniform
     * distribution.
     */
    static CORE_NAMESPACE::uint16 uint16(
        CORE_NAMESPACE::uint16 min = 0, CORE_NAMESPACE::uint16 max = uint16_max
    );
    /**
     * @brief Generate random uint32 in range [min, max] with uniform
     * distribution.
     */
    static CORE_NAMESPACE::uint32 uint32(
        CORE_NAMESPACE::uint32 min = 0, CORE_NAMESPACE::uint32 max = uint32_max
    );
    /**
     * @brief Generate random uint64 in range [min, max] with uniform
     * distribution.
     */
    static CORE_NAMESPACE::uint64 uint64(
        CORE_NAMESPACE::uint64 min = 0, CORE_NAMESPACE::uint64 max = uint64_max
    );
    /**
     * @brief Generate random uint128 in range [min, max] with uniform
     * distribution.
     */
    static CORE_NAMESPACE::uint128 uint128(
        CORE_NAMESPACE::uint128 min = 0,
        CORE_NAMESPACE::uint128 max = uint128_max
    );

    /**
     * @brief Generate random int8 in range [min, max] with uniform
     * distribution.
     */
    static CORE_NAMESPACE::int8 int8(
        CORE_NAMESPACE::int8 min = int8_min, CORE_NAMESPACE::int8 max = int8_max
    );
    /**
     * @brief Generate random int16 in range [min, max] with uniform
     * distribution.
     */
    static CORE_NAMESPACE::int16 int16(
        CORE_NAMESPACE::int16 min = int16_min,
        CORE_NAMESPACE::int16 max = int16_max
    );
    /**
     * @brief Generate random int32 in range [min, max] with uniform
     * distribution.
     */
    static CORE_NAMESPACE::int32 int32(
        CORE_NAMESPACE::int32 min = int32_min,
        CORE_NAMESPACE::int32 max = int32_max
    );
    /**
     * @brief Generate random int64 in range [min, max] with uniform
     * distribution.
     */
    static CORE_NAMESPACE::int64 int64(
        CORE_NAMESPACE::int64 min = int64_min,
        CORE_NAMESPACE::int64 max = int64_max
    );
    /**
     * @brief Generate random int128 in range [min, max] with uniform
     * distribution.
     */
    static CORE_NAMESPACE::int128 int128(
        CORE_NAMESPACE::int128 min = int128_min,
        CORE_NAMESPACE::int128 max = int128_max
    );

    /**
     * @brief Generate random float32 in range [min, max] with uniform
     * distribution.
     */
    static CORE_NAMESPACE::float32 float32(
        CORE_NAMESPACE::float32 min = float32_min,
        CORE_NAMESPACE::float32 max = float32_max
    );
    /**
     * @brief Generate random float64 in range [min, max] with uniform
     * distribution.
     */
    static CORE_NAMESPACE::float64 float64(
        CORE_NAMESPACE::float64 min = float64_min,
        CORE_NAMESPACE::float64 max = float64_max
    );

    /**
     * @brief Generates random float32 in 0 to 1 range.
     */
    static CORE_NAMESPACE::float32 float32_01();
    /**
     * @brief Generates random float64 in 0 to 1 range.
     */
    static CORE_NAMESPACE::float64 float64_01();

  private:
    Random();
    ~Random();

    static std::random_device _random_device;
    static std::mt19937       _generator;

    template<typename T>
    static T random_integer(T min, T max) {
        std::uniform_int_distribution<T> distribution(min, max);
        return distribution(_generator);
    }

    template<typename T>
    static T random_real(T min, T max) {
        std::uniform_real_distribution<T> distribution(min, max);
        return distribution(_generator);
    }
};

} // namespace CORE_NAMESPACE