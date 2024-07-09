/**
 * @file inlined_functions.hpp
 * @author Android172 (android172unity@gmail.com)
 * @brief Defines some wildly used functions
 * @version 0.1
 * @date 2024-05-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "types.hpp"

namespace CORE_NAMESPACE {

/**
 * @brief Get closes granularity aligned number.
 * @param operand Number to align
 * @param granularity Granularity (usually 4 or 8)
 * @return uint64 Number at or before \p operand thats aligned with given \p
 * granularity.
 */
inline uint64 get_aligned(uint64 operand, uint64 granularity) {
    return ((operand + (granularity - 1)) & ~(granularity - 1));
}

} // namespace CORE_NAMESPACE
