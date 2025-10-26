#include "AnimationTime.hpp"
#include "app/App.hpp"

uint32_t AnimationTime::ProcessNextFrame()
{
    if (sequence_.empty())
    {
        sequence_ = App::locale.GetSequence(state_.time_params.time_type);
    }
    if (state_.current_step >= sequence_.size())
    {
        state_.finished = true;
        return 0;
    }

    App::display.Clear();
    uint32_t delay_ms = ProcessFrameElement(sequence_[state_.current_step]);
    App::display.Update();

    state_.current_step++;
    return delay_ms;
}

void AnimationTime::Reset()
{
    Animation::Reset();
    sequence_.clear();
}

uint32_t AnimationTime::ProcessFrameElement(ClockFrame frame)
{
    switch (frame)
    {
    case ClockFrame::CLOCKFACE:
        App::display.SetClockFace(state_.time_params.face);
        return App::timings.GetHold();
    case ClockFrame::HOURS_NUM:
        App::display.SetNumber(state_.time_params.hours);
        return App::timings.GetHoldDigits();
    case ClockFrame::HOURS:
        App::display.SetTopLed(Display::TopLed::HOURS);
        return App::timings.GetHold();
    case ClockFrame::MINUTES_NUM:
        App::display.SetNumber(state_.time_params.minutes);
        return App::timings.GetHoldDigits();
    case ClockFrame::MINUTES:
        App::display.SetTopLed(Display::TopLed::MINUTES);
        return App::timings.GetHold();
    case ClockFrame::AFTER:
        App::display.SetTopLed(Display::TopLed::AFTER);
        return App::timings.GetHold();
    case ClockFrame::BEFORE:
        App::display.SetTopLed(Display::TopLed::BEFORE);
        return App::timings.GetHold();
    case ClockFrame::PM:
        if (state_.time_params.pm)
        {
            App::display.SetPm(true);
            return App::timings.GetHold();
        }
        return 0;
    default:
        return 0;
    }
}