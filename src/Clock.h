#pragma once

#include <iostream>
#include <chrono>

class Clock
{
public:
    Clock() {};
    void init()
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }
    void reset() {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }
    // Returns sec
    auto lapTime()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
        auto duration = (end - start);
        double sec = duration * 0.00001;
        return sec;
    }
    void close() {};
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
};