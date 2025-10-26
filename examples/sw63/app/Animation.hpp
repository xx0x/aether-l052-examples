#pragma once

#include "LocaleConfig.hpp"
#include "app/Display.hpp"
#include <cstdint>

class Animation
{
public:
    enum class Type
    {
        INTRO,
        TIME,
        CHARGE,
        COUNT
    };

    // Returns delay in milliseconds until next frame
    virtual uint32_t ProcessNextFrame() = 0;

    virtual void Reset()
    {
        current_step_ = 0;
        finished_ = false;
    }

    void SetTime(const LocaleConfig::TimeParameters &time_params)
    {
        time_params_ = time_params;
    }

    bool IsFinished() const
    {
        return finished_;
    }

protected:
    LocaleConfig::TimeParameters time_params_;
    uint32_t current_step_ = 0;
    bool finished_ = false;
};