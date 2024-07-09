#include "platform/platform.hpp"
#if PLATFORM == LINUX

#    include <iostream>

#    if _POSIX_C_SOURCE >= 199309L
#        include <time.h>
#    else
#        include <unistd.h>
#    endif

#    include "multithreading/parallel.hpp"

namespace CORE_NAMESPACE {

namespace platform {
    float64 get_absolute_time() {
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        return now.tv_sec + now.tv_nsec * 1e-9;
    }

    void sleep(uint64 ms) {
#    if _POSIX_C_SOURCE >= 199309L
        struct timespec ts;
        ts.tv_sec  = ms / 1000;
        ts.tv_nsec = (ms % 1000) * 1000000;
        nanosleep(&ts, 0);
#    else
        if (ms >= 1000) sleep(ms / 1000);
        usleep((ms % 1000) * 1000);
#    endif
    }

    // /////// //
    // Console //
    // /////// //

    Console::Console() {}
    Console::~Console() {}

    void Console::write(std::string message, uint32 kind, bool new_line) {
        const char* color_string[] { "0",    "0;41", "1;31", "1;33",
                                     "1;32", "1;34", "1;30" };

        static parallel::Mutex mutex {};
        mutex.lock();
        std::cout << "\033[" << color_string[kind] << "m" << message
                  << "\033[0m";
        if (new_line) std::cout << std::endl;
        mutex.unlock();
    }

} // namespace platform

} // namespace CORE_NAMESPACE

#endif