#pragma once

#include <cstdint>

class TimingsConfig
{
public:
    TimingsConfig()
    {
        SetSpeed(4);
    }

    void SetSpeed(uint32_t speed)
    {
        if (speed < kSpeedCount)
        {
            time_hold_ = kHold[speed];
            time_hold_digits_ = kHoldDigits[speed];
            time_pause_ = kPause[speed];
        }
    }

    uint32_t GetHold() const
    {
        return time_hold_;
    }

    uint32_t GetHoldDigits() const
    {
        return time_hold_digits_;
    }

    uint32_t GetTimePause() const
    {
        return time_pause_;
    }

    static uint32_t GetSpeedCount()
    {
        return kSpeedCount;
    }

private:
    // Speed configuration arrays (6 speed levels: 0=slowest, 5=fastest)
    static constexpr uint32_t kSpeedCount = 6;
    static constexpr uint32_t kHoldDigits[kSpeedCount] = {1000, 800, 600, 400, 300, 150};
    static constexpr uint32_t kHold[kSpeedCount] = {800, 640, 480, 320, 240, 120};
    static constexpr uint32_t kPause[kSpeedCount] = {150, 120, 90, 60, 45, 23};

    uint32_t time_hold_ = kHold[0];
    uint32_t time_hold_digits_ = kHoldDigits[0];
    uint32_t time_pause_ = kPause[0];
};