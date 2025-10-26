#include "AnimationTime.hpp"
#include "app/App.hpp"

uint32_t AnimationTime::ProcessNextFrame()
{
    if (sequence_.empty())
    {
        sequence_ = App::locale.GetSequence(time_params_.time_type);
    }
    if (current_step_ >= sequence_.size())
    {
        finished_ = true;
        return 0;
    }

    App::display.Clear();
    uint32_t delay_ms = ProcessFrameElement(sequence_[current_step_]);
    App::display.Update();

    current_step_++;
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
        App::display.SetClockFace(time_params_.face);
        return App::timings.GetHold();
    case ClockFrame::HOURS_NUM:
        App::display.SetNumber(time_params_.hours);
        return App::timings.GetHoldDigits();
    case ClockFrame::HOURS:
        App::display.SetTopLed(Display::TopLed::HOURS);
        return App::timings.GetHold();
    case ClockFrame::MINUTES_NUM:
        App::display.SetNumber(time_params_.minutes);
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
        if (time_params_.pm)
        {
            App::display.SetPm(true);
            return App::timings.GetHold();
        }
        return 0;
    default:
        return 0;
    }
}