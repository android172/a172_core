/**
 * @file defines.hpp
 * @author Android172 (android172unity@gmail.com)
 * @brief Defines types, values and functions used by all core components
 * @version 0.1
 * @date 2024-05-07
 *
 * @copyright Copyright (c) 2024
 */

#pragma once

#ifndef CORE_NAMESPACE
#    define CORE_NAMESPACE a172
#endif // !CORE_NAMESPACE

// -----------------------------------------------------------------------------
// PLATFORMS
// -----------------------------------------------------------------------------
// List of supported platforms
#define LINUX 1
#define WINDOWS32 2
#define WINDOWS64 3

// detect platform
#ifdef _WIN32 // Includes both 32 bit and 64 bit
#    ifdef _WIN64
#        define PLATFORM WINDOWS32
#    else
#        define PLATFORM WINDOWS64
#    endif
#else
#    if __linux__
#        define PLATFORM LINUX
#    elif __unix__
#        define PLATFORM UNIX
#    else
#        error "Cant compile on this platform.";
#    endif
#endif
