#pragma once

#include "../Animation.hpp"

class AnimationCharge : public Animation
{
public:
    uint32_t ProcessNextFrame() override;

private:
    enum class VisualStyle
    {
        BAR,
        SINGLE,
        SNAKE,
        COUNT
    };

    VisualStyle visual_style_ = VisualStyle::SNAKE;
};