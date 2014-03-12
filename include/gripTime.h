
/**
 * \file Timer.h
 * \brief Cross-platform clock class that get nanosecond precision on
 * Linux and Mac, and microsecond precision on Windows.
 */

#ifndef TIMER_H_DEF
#define TIMER_H_DEF

/// Include platform-specific headers
#ifdef WIN32             // Windows system specific
#include <windows.h>
#elif defined(__APPLE__) // Machintosh
#include <mach/mach.h>
#include <mach/mach_time.h>
#else                    // Linux based system specific
#include <time.h>
#endif

/**
 * \namespace grip
 * \brief Namespace for stuff that is used for grip
 */
namespace grip {

    /**
     * \brief Cross-platform inline function for getting the current time in seconds with a resolutions
     * of nanoseconds for Linux and Machintosh, and microseconds for Windows.
     * \return double The time in seconds
     */
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
