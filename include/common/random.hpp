#pragma once

#include "defines.hpp"

#include <random>

namespace CORE_NAMESPACE {

class Random {
  public:
    static void set_seed(const CORE_NAMESPACE::uint32 seed);

    static CORE_NAMESPACE::uint8  uint8( //
        CORE_NAMESPACE::uint8 min = 0,
        CORE_NAMESPACE::uint8 max = uint8_max
    );
    static CORE_NAMESPACE::uint16 uint16(
        CORE_NAMESPACE::uint16 min = 0, CORE_NAMESPACE::uint16 max = uint16_max
    );
    static CORE_NAMESPACE::uint32 uint32(
        CORE_NAMESPACE::uint32 min = 0, CORE_NAMESPACE::uint32 max = uint32_max
    );
    static CORE_NAMESPACE::uint64 uint64(
        CORE_NAMESPACE::uint64 min = 0, CORE_NAMESPACE::uint64 max = uint64_max
    );
    static CORE_NAMESPACE::uint128 uint128(
        CORE_NAMESPACE::uint128 min = 0,
        CORE_NAMESPACE::uint128 max = uint128_max
    );

    static CORE_NAMESPACE::int8 int8(
        CORE_NAMESPACE::int8 min = int8_min, CORE_NAMESPACE::int8 max = int8_max
    );
    static CORE_NAMESPACE::int16 int16(
        CORE_NAMESPACE::int16 min = int16_min,
        CORE_NAMESPACE::int16 max = int16_max
    );
    static CORE_NAMESPACE::int32 int32(
        CORE_NAMESPACE::int32 min = int32_min,
        CORE_NAMESPACE::int32 max = int32_max
    );
    static CORE_NAMESPACE::int64 int64(
        CORE_NAMESPACE::int64 min = int64_min,
        CORE_NAMESPACE::int64 max = int64_max
    );
    static CORE_NAMESPACE::int128 int128(
        CORE_NAMESPACE::int128 min = int128_min,
        CORE_NAMESPACE::int128 max = int128_max
    );

    static CORE_NAMESPACE::float32 float32(
        CORE_NAMESPACE::float32 min = float32_min,
        CORE_NAMESPACE::float32 max = float32_max
    );
    static CORE_NAMESPACE::float64 float64(
        CORE_NAMESPACE::float64 min = float64_min,
        CORE_NAMESPACE::float64 max = float64_max
    );
    // static CORE_NAMESPACE::float128 float128(
    //     CORE_NAMESPACE::float128 min = float128_min,
    //     CORE_NAMESPACE::float128 max = float128_max
    // );

    static CORE_NAMESPACE::float32 float32_01();
    static CORE_NAMESPACE::float64 float64_01();
    // static CORE_NAMESPACE::float128 float128_01();

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