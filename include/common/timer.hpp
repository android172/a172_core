#pragma once

#include "string.hpp"

namespace CORE_NAMESPACE {

class Timer {
  public:
    Timer() { reset(); }
    ~Timer() {}

    void start();
    void stop();
    void reset();

    float64 log_delta(const String& message);
    float64 delta();
    float64 elapsed() const;
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