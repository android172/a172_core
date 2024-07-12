#include "timer.hpp"

#include "logger.hpp"
#include "platform/platform.hpp"

namespace CORE_NAMESPACE {

Timer Timer::global_timer {};

void Timer::start() {
    _running    = true;
    _last_start = platform::get_absolute_time();
}
void Timer::stop() {
    _running = false;
    _accumulated += platform::get_absolute_time() - _last_start;
}
void Timer::reset() {
    _very_beginning  = platform::get_absolute_time();
    _last_checkpoint = _very_beginning;
    _last_start      = _very_beginning;
    _accumulated     = 0.0;
}

float64 Timer::log_delta(const String& message) {
    if (!_running) return 0.0;
    const auto dt = delta();
    Logger::log(message, dt, "ms");
    return dt;
}
float64 Timer::delta() {
    if (!_running) return 0.0;
    const auto current = platform::get_absolute_time();
    const auto delta   = (current - _last_checkpoint) * 1000;
    _last_checkpoint   = current;
    return delta;
}
float64 Timer::elapsed() const {
    if (!_running) return 0.0;
    const auto current = platform::get_absolute_time();
    const auto elapsed = (current - _last_start) * 1000;
    return (elapsed + _accumulated) * 1000;
}
float64 Timer::total() const {
    if (!_running) return 0.0;
    float64 current = platform::get_absolute_time();
    return (current - _very_beginning) * 1000;
}

} // namespace CORE_NAMESPACE