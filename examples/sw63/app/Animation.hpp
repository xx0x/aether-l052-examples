#pragma once

#include "app/Display.hpp"
#include "LocaleConfig.hpp"
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

    struct AnimationState
    {
        LocaleConfig::TimeParameters time_params;
        uint32_t current_step = 0;
        bool finished = false;
    };

    virtual ~Animation() = default;
    virtual void Reset()
    {
        state_.current_step = 0;
        state_.finished = false;
    }

    virtual void SetTime(const LocaleConfig::TimeParameters& time_params)
    {
        state_.time_params = time_params;
    }

    bool IsFinished() const
    {
        return state_.finished;
    }

    // Returns delay in milliseconds until next frame
    virtual uint32_t ProcessNextFrame() = 0;

protected:
    AnimationState state_;
};