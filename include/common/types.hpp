/**
 * @file types.hpp
 * @author Android172 (android172unity@gmail.com)
 * @brief Defines and validates default elementary types used in core namespace
 * @version 0.1
 * @date 2024-05-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "defines.hpp"

#include <limits>

namespace CORE_NAMESPACE {

// -----------------------------------------------------------------------------
// TYPES
// -----------------------------------------------------------------------------
// byte
typedef char          byte;
typedef unsigned char ubyte;

// Unsigned char
typedef unsigned char uchar;

// Unsigned integer
typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned long long uint64;
typedef __uint128_t        uint128;

// Signed integer
typedef signed char      int8;
typedef signed short     int16;
typedef signed int       int32;
typedef signed long long int64;
typedef __int128_t       int128;

// Floats
typedef float      float32;
typedef double     float64;
typedef __float128 float128;

// Check if sizes are correct
static_assert(sizeof(byte) == 1, "Expected a 1 byte long byte.");
static_assert(sizeof(ubyte) == 1, "Expected a 1 byte long ubyte.");

static_assert(sizeof(uchar) == 1, "Expected a 1 byte long uchar.");

static_assert(sizeof(uint8) == 1, "Expected a 1 byte long uint8.");
static_assert(sizeof(uint16) == 2, "Expected a 2 byte long uint16.");
static_assert(sizeof(uint32) == 4, "Expected a 4 byte long uint32.");
static_assert(sizeof(uint64) == 8, "Expected a 8 byte long uint64.");
static_assert(sizeof(uint128) == 16, "Expected a 16 byte long uint128.");

static_assert(sizeof(int8) == 1, "Expected a 1 byte long int8.");
static_assert(sizeof(int16) == 2, "Expected a 2 byte long int16.");
static_assert(sizeof(int32) == 4, "Expected a 4 byte long int32.");
static_assert(sizeof(int64) == 8, "Expected a 8 byte long int64.");
static_assert(sizeof(int128) == 16, "Expected a 16 byte long int128.");

static_assert(sizeof(float32) == 4, "Expected a 4 byte long float32.");
static_assert(sizeof(float64) == 8, "Expected a 8 byte long float64.");
static_assert(sizeof(float128) == 16, "Expected a 16 byte long float128.");

// -----------------------------------------------------------------------------
// Numeric limits
// -----------------------------------------------------------------------------

// Max unsigned integers
inline const constexpr uint8   uint8_max   = 255;
inline const constexpr uint16  uint16_max  = 65535;
inline const constexpr uint32  uint32_max  = 4294967295U;
inline const constexpr uint64  uint64_max  = (uint64) -1;
inline const constexpr uint128 uint128_max = (uint128) -1;

// Max / Min integers
inline const constexpr int8   int8_max   = 127;
inline const constexpr int16  int16_max  = 32767;
inline const constexpr int32  int32_max  = 2147483647;
inline const constexpr int64  int64_max  = uint64_max / 2;
inline const constexpr int128 int128_max = uint128_max / 2;

inline const constexpr int8   int8_min   = -int8_max - 1;
inline const constexpr int16  int16_min  = -int16_max - 1;
inline const constexpr int32  int32_min  = -int32_max - 1;
inline const constexpr int64  int64_min  = -int64_max - 1;
inline const constexpr int128 int128_min = -uint128_max - 1;

// Smallest positive floats
inline const constexpr float32 Epsilon32 =
    std::numeric_limits<float32>::epsilon();
inline const constexpr float64 Epsilon64 =
    std::numeric_limits<float64>::epsilon();
inline const constexpr float128 Epsilon128 =
    std::numeric_limits<float128>::epsilon();

// Smallest finite float
inline const constexpr float32 float32_min =
    std::numeric_limits<float32>::lowest();
inline const constexpr float64 float64_min =
    std::numeric_limits<float64>::lowest();
inline const constexpr float128 float128_min =
    std::numeric_limits<float128>::lowest();

// Largest float
inline const constexpr float32 float32_max =
    std::numeric_limits<float32>::max();
inline const constexpr float64 float64_max =
    std::numeric_limits<float64>::max();
inline const constexpr float128 float128_max =
    std::numeric_limits<float128>::max();

// Infinity
inline const constexpr float32 Infinity32 =
    std::numeric_limits<float32>::infinity();
inline const constexpr float64 Infinity64 =
    std::numeric_limits<float64>::infinity();
inline const constexpr float128 Infinity128 =
    std::numeric_limits<float128>::infinity();

// -----------------------------------------------------------------------------
// Macro types
// -----------------------------------------------------------------------------

// String constant expresion attribute
/**
 * @brief This macro represents a constant static type. Used for building enum
 * objects out of strings. Must be used as class or struct attribute.
 */
#define StringEnum constexpr static const char* const
/**
 * @brief Declares and sets StringEnum to passed value. Must be used  as class
 * or struct attribute. Attribute value set to literal stringification of
 * attribute name.
 */
#define STRING_ENUM(x) StringEnum x = #x

} // namespace CORE_NAMESPACE