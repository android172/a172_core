/**
 * @file timer.hpp
 * @author Android172 (android172unity@gmail.com)
 * @brief Provides timer functionality. Used to keep track of running time
 * during benchmarking, or program logic.
 * @version 0.1
 * @date 2024-07-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "string.hpp"

namespace CORE_NAMESPACE {

/**
 * @brief Defines timer class. Keeps track of time. Works as a stopwatch, can be
 * started, stoped, reset. Special functionality for logging time between ticks
 * with a custom message. Ideal for performance benchmarking.
 *
 */
class Timer {
  public:
    /**
     * @brief Construct a new Timer object. By default timed time is rest.
     */
    Timer() { reset(); }
    ~Timer() {}

    /**
     * @brief Starts this timer.
     */
    void start();
    /**
     * @brief Stops this timer.
     */
    void stop();
    /**
     * @brief Timer time is reset. All tracked values are reset.
     */
    void reset();

    /**
     * @brief Outputs provided message, also noting on time passed since last
     * check. Note: any call to @b delta or @b log_delta will reset internal
     * time since last check, after returning it.
     *
     * @param message Lines of text logged right before time in miliseconds.
     * @return float64 Time since last check in miliseconds
     */
    float64 log_delta(const String& message);
    /**
     * @brief Computes time passed since the last delta check. Note: any call to
     * @b delta or @b log_delta will reset internal time since last check, after
     * returning it.
     *
     * @return float64 Time since last check in miliseconds
     */
    float64 delta();
    /**
     * @brief Returns total running elapsed time since timers initial start.
     * Wont include stopped time.
     *
     * @return float64 Total cumulative runtime in miliseconds
     */
    float64 elapsed() const;
    /**
     * @brief Returns time total since counting begun.
     *
     * @return float64 Total time passed from first ever start until now in
     * miliseconds
     */
    float64 total() const;

    // TODO: most likely temporary for now
    static Timer global_timer;

  private:
    bool    _running         = false;
    float64 _last_checkpoint = 0.0;
    float64 _last_start      = 0.0;
    float64 _very_beginning  = 0.0;
    float64 _accumulated     = 0.0;
};

} // namespace CORE_NAMESPACE