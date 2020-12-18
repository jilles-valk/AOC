#pragma once
#include <iostream>
#include <chrono>

struct ScopeBasedTimer
{
    std::chrono::time_point<std::chrono::steady_clock> start, end;
    std::chrono::duration<float> duration;

    ScopeBasedTimer()
    {
        start = std::chrono::high_resolution_clock::now();
    }

    ~ScopeBasedTimer()
    {
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;

        float dur = 0;
        std::string unit;

        if (std::chrono::duration_cast<std::chrono::minutes>(duration).count() > 10.0f)
        {
            dur = std::chrono::duration_cast<std::chrono::minutes>(duration).count();
            unit = "minutes";
        }
        else if (std::chrono::duration_cast<std::chrono::seconds>(duration).count() > 10.0f)
        {
            dur = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
            unit = "seconds";
        }
        else if (std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() > 10.0f)
        {
            dur = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            unit = "milliseconds";
        }
        else if (std::chrono::duration_cast<std::chrono::microseconds>(duration).count() > 10.0f)
        {
            dur = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
            unit = "microseconds";
        }
        else
        {
            dur = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
            unit = "nanoseconds";
        }

        std::cout << "Operation took " << dur << " " << unit << std::endl;
    }
};