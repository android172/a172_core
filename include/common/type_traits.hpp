/**
 * @file type_traits.hpp
 * @author Android172 (android172unity@gmail.com)
 * @brief Defines some commonly used type traits
 * @version 0.1
 * @date 2024-05-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "defines.hpp"

#include <type_traits>

namespace CORE_NAMESPACE {

/**
 * @brief Trait indicating if type \b T has iterator. Can be used to, for
 * example, identify array like type.
 * @tparam T Type to check
 */
template<typename T, typename = std::void_t<>>
struct has_iterator //
    : std::false_type {};
template<typename T>
struct has_iterator<T, std::void_t<typename T::iterator>> //
    : std::true_type {};

} // namespace CORE_NAMESPACE