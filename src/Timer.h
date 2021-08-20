#pragma once

#include <iostream>
#include <chrono>

const std::string _SOLUTIONDIR = _SOLUTIONDIR_;

// console log time taken by certain code block
// pass name of block to constructor
class Timer
{
public:
    Timer(const char* name)
        : m_Name(name), m_Stopped(false)
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        if (!m_Stopped)
            Stop();
    }

    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
        m_Stopped = true;
        auto duration = (end - start);
        double ms = duration * 0.001;
        std::cout << m_Name << " took: " << ms << " ms" << std::endl;
    }
private:
    const char* m_Name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
    bool m_Stopped;
};