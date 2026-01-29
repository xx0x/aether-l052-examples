#include "AnimationCharge.hpp"
#include "app/App.hpp"

uint32_t AnimationCharge::ProcessNextFrame()
{
    static constexpr uint32_t kLedCount = 12;
    uint32_t total_steps = kLedCount;

    if (visual_style_ == VisualStyle::BAR)
    {
        // 12 numbers + 1 full cycle
        total_steps += 1;
    }

    if (current_step_ >= total_steps)
    {
        // Restart animation for continuous charging indicator
        current_step_ = 0;
    }

    App::display.Clear();

    // Bar fill type animation
    switch (visual_style_)
    {
    case VisualStyle::BAR:
        App::display.SetNumber(std::clamp(current_step_ + 1ul, 1ul, kLedCount));
        break;
    case VisualStyle::SINGLE:
        App::display.Clear();
        App::display.SetNumberLed(current_step_ + 1, true);
        break;
    case VisualStyle::SNAKE:
        App::display.Clear();
        App::display.SetNumberLed(current_step_ + 1, true);
        App::display.SetNumberLed((current_step_ + 1) % kLedCount + 1, true);
        App::display.SetNumberLed((current_step_ + 2) % kLedCount + 1, true);
        break;
    }

    App::display.Update();
    current_step_++;

    return 150; // Moderate speed for charge animation
}