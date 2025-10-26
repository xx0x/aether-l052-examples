#include "AnimationIntro.hpp"
#include "app/App.hpp"

uint32_t AnimationIntro::ProcessNextFrame()
{
    constexpr uint32_t kTotalSteps = 20;

    if (state_.current_step >= kTotalSteps)
    {
        state_.finished = true;
        return 0;
    }

    App::display.Clear();

    uint32_t delay_ms = 100; // Fast intro animation

    if (state_.current_step < 12)
    {
        // Animate through numbers and LEDs
        if (state_.current_step % 2 == 0)
            App::display.SetTopLed(Display::TopLed::AFTER);
        if (state_.current_step % 5 == 0)
            App::display.SetTopLed(Display::TopLed::BEFORE);
        if (state_.current_step % 3 == 0)
            App::display.SetTopLed(Display::TopLed::HOURS);
        if (state_.current_step % 4 == 0)
            App::display.SetTopLed(Display::TopLed::MINUTES);

        App::display.SetNumber(state_.current_step + 1);
    }
    else if (state_.current_step < 16)
    {
        // Cycle through clock faces
        auto face = static_cast<Display::ClockFace>(state_.current_step - 12);
        App::display.SetClockFace(face);
        App::display.SetPm(state_.current_step % 2 == 0);
    }
    else
    {
        // Show language indicator
        uint32_t lang_num = static_cast<uint32_t>(App::locale.GetLanguage()) + 1;
        App::display.SetNumber(state_.current_step % 2 == 0 ? lang_num : 0);
    }

    App::display.Update();
    state_.current_step++;

    return delay_ms;
}