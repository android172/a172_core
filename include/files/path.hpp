/**
 * @file path.hpp
 * @author Android172 (android172unity@gmail.com)
 * @brief Defines path type. Used for file management
 * @version 0.1
 * @date 2024-07-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "common/defines.hpp"

#include <filesystem>

namespace CORE_NAMESPACE {

/**
 * @brief Path type. Denotes location of resources in local file system.
 */
class Path : public std::filesystem::path {
  public:
    using std::filesystem::path::path;
};

} // namespace CORE_NAMESPACE