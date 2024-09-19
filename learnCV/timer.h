#ifndef TIMER_H
#define TIMER_H

#include <ctime>

class Timer {
    clock_t cpu_start;
    clock_t cpu_end;

public:
    Timer() {
        cpu_start = cpu_end = 0;
    }

    void start() {
        cpu_start = clock();
    }

    void end() {
        cpu_end = clock();
    }

    long getms() const {
        return (cpu_end - cpu_start) / (CLOCKS_PER_SEC / 1000);
    }
};

static Timer static_timer_;

#define TEST_TIME(code) \
    static_timer_.start();\
    code;\
    static_timer_.end();\
    printf("Time: %ld ms\n", static_timer_.getms())


#endif //TIMER_H
