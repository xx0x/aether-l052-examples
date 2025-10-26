#pragma once

#include "../Animation.hpp"
#include "../LocaleConfig.hpp"
#include "../TimingsConfig.hpp"
#include "ClockFrame.hpp"

class AnimationTime : public Animation
{
public:
    enum class TimeType
    {
        EXACT,
        PAST,
        TO
    };

    AnimationTime(TimeType type) : time_type_(type) {}

    uint32_t ProcessNextFrame() override;
    void Reset() override;

private:
    TimeType time_type_;
    FrameSequence sequence_;

    FrameSequence GetSequenceForType();
    uint32_t ProcessFrameElement(ClockFrame frame);
};