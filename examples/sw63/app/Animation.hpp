#pragma once

#include "app/Display.hpp"
#include <cstdint>

class Animation
{
public:
    enum class Type
    {
        INTRO,
        EXACT,
        TO,
        PAST,
        CHARGE,
        COUNT
    };

    struct AnimationState
    {
        uint32_t hours = 0;
        uint32_t minutes = 0;
        Display::ClockFace face = Display::ClockFace::UP;
        bool pm = false;
        uint32_t current_step = 0;
        bool finished = false;
    };

    virtual ~Animation() = default;
    virtual void Reset()
    {
        state_.current_step = 0;
        state_.finished = false;
    }

    virtual void SetTime(uint32_t hours, uint32_t minutes, Display::ClockFace face, bool pm)
    {
        if (hours == 0)
        {
            hours = 12;
        }
        state_.hours = hours;
        state_.minutes = minutes;
        state_.face = face;
        state_.pm = pm;
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