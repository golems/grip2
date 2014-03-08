
/**
 * \file Timer.h
 * \brief Crossplatform clock class that get nanosecond precision on
 * Linux and Mac, and microsecond precision on Windows.
 */

#ifndef TIMER_H_DEF
#define TIMER_H_DEF

#ifdef WIN32             // Windows system specific
#include <windows.h>
#elif defined(__APPLE__) // Machintosh
#include <mach/mach.h>
#include <mach/mach_time.h>
#else                    // Linux based system specific
#include <time.h>
#endif

namespace grip {

inline double getTime() {
#ifdef WIN32
    LARGE_INTEGER time, freq;
    QueryPerformanceCounter(&time);
    QueryPerformanceFrequency(&freq);
    return (1000LL * time.QuadPart) / freq.QuadPart;
#elif defined(__APPLE__)
    uint64_t time;
    return double(*(uint64_t*)AbsoluteToNanoseconds(*(AbsoluteTime*)(mach_absolute_time()))) * 1e-9;
#else
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time.tv_sec + (1e-9)*time.tv_nsec;
#endif
}

} // end namespace grip

#endif // TIMER_H_DEF
