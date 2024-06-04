#pragma once
#include <chrono>
class Timer
{
private:
    using clock = std::chrono::steady_clock;
    using time_point = clock::time_point;

    time_point start_time;

public:
    Timer() : start_time(clock::now()) {}
    
    void reset() {
        start_time = clock::now();
    }

    double elapsed_seconds() const {
        return std::chrono::duration_cast<std::chrono::duration<double>>(clock::now() - start_time).count();
    }
};


