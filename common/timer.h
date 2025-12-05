#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>
#include <time.h>

static inline void start_timer(struct timespec* t) {
    clock_gettime(CLOCK_MONOTONIC, t);
}

static inline void end_timer(const char* block_name, const struct timespec* t) {
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &end);

    long long start_ns = (long long)t->tv_sec * 1000000000LL + t->tv_nsec;
    long long end_ns = (long long)end.tv_sec * 1000000000LL + end.tv_nsec;
    long long diff_ns = end_ns - start_ns;

    printf("%s time: ", block_name);
    if(diff_ns < 1000LL) printf("%lld ns\n", diff_ns);
    else if(diff_ns < 1000000LL) printf("%.3f µs\n", diff_ns / 1000.0);
    else if(diff_ns < 1000000000LL) printf("%.3f ms\n", diff_ns / 1000000.0);
    else printf("%.6f s\n", diff_ns / 1000000000.0);
}

#define TIMED(name)                                                          \
    struct timespec EXT_CONCAT_(_timer_, __LINE__);                          \
    for(int i_ = (start_timer(&EXT_CONCAT_(_timer_, __LINE__)), 0); i_ != 1; \
        (end_timer((name), &EXT_CONCAT_(_timer_, __LINE__)), i_ = 1))

#endif  // TIMER_H
