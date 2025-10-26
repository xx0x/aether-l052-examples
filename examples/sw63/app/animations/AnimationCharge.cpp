#include "AnimationCharge.hpp"
#include "app/App.hpp"

uint32_t AnimationCharge::ProcessNextFrame()
{
    constexpr uint32_t kTotalSteps = 13; // 12 numbers + 1 full cycle

    if (state_.current_step >= kTotalSteps)
    {
        // Restart animation for continuous charging indicator
        state_.current_step = 0;
    }

    App::display.Clear();

    if (state_.current_step == 0)
    {
        // Show all numbers briefly to indicate full
        App::display.SetNumber(12);
    }
    else
    {
        // Progressive fill animation
        App::display.SetNumber(state_.current_step);
    }

    App::display.Update();
    state_.current_step++;

    return 150; // Moderate speed for charge animation
}