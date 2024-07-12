/**
 * @file platform.hpp
 * @author Android172 (android172unity@gmail.com)
 * @brief Provides abstraction layer for platform specific functionality.
 * Accessed via `platform` namespace.
 * @version 0.1
 * @date 2024-07-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <string>

#include "common/types.hpp"

namespace CORE_NAMESPACE {

class InputSystem;

/**
 * @brief Namespace representing the platform layer of the application.
 * Provides a platform agnostic way to access certain functionalities of the
 * system. Individual supported platforms should provide their own .cpp
 * implementation files.
 */
namespace platform {
    // Not important for the rest of the app
    namespace __details {
        const constexpr int n = 1;
    };

    /// @brief True if little endian byte configuration is used by this system
    const bool is_little_endian = *(char*) &__details::n == 1;

    /// @brief Get current internal clock time in seconds
    /// @return float64 Time in seconds
    float64 get_absolute_time();

    /// @brief Suspends the application process for the specified amount of time
    /// @param ms Time to sleep in miliseconds
    void sleep(uint64 ms);

    /**
     * @brief A platform agnostic Console I/O class. Can only be used if the
     * platform supports a console.
     */
    class Console {
      private:
      public:
        /**
         * @brief Construct a new Console object
         */
        Console();
        ~Console();

        /**
         * @brief Write a message to the console
         *
         * @param message Message contents
         * @param kind Determins message look and importance
         * @param new_line Adds a new line after the message
         */
        static void write(
            std::string message, uint32 kind = 0, bool new_line = true
        );
        /**
         * @brief Read plain text from console
         *
         * @return std::string Read text
         */
        static std::string read();
    };
}; // namespace platform

} // namespace CORE_NAMESPACE