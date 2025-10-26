#include "AnimationCharge.hpp"
#include "app/App.hpp"

uint32_t AnimationCharge::ProcessNextFrame()
{
    constexpr uint32_t kTotalSteps = 13; // 12 numbers + 1 full cycle

    if (current_step_ >= kTotalSteps)
    {
        // Restart animation for continuous charging indicator
        current_step_ = 0;
    }

    App::display.Clear();

    if (current_step_ == 0)
    {
        // Show all numbers briefly to indicate full
        App::display.SetNumber(12);
    }
    else
    {
        // Progressive fill animation
        App::display.SetNumber(current_step_);
    }

    App::display.Update();
    current_step_++;

    return 150; // Moderate speed for charge animation
}