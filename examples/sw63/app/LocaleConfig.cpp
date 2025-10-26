#include "LocaleConfig.hpp"

void LocaleConfig::SetLanguage(Language language)
{
    language_ = language;
}

LocaleConfig::Language LocaleConfig::GetLanguage() const
{
    return language_;
}

LocaleConfig::TimeCoefficients LocaleConfig::GetTimeCoefficients() const
{
    switch (language_)
    {
    case Language::CZECH:
    case Language::HUNGARIAN:
        return {1, 1, 1};
    case Language::GERMAN:
    case Language::POLISH:
        return {0, 1, 1};
    case Language::ENGLISH:
    default:
        return {0, 0, 1};
    }
}

FrameSequence LocaleConfig::GetSequence(TimeType time_type) const
{
    switch (time_type)
    {
    case TimeType::EXACT:
        // Exact time is the same for all languages
        return {ClockFrame::CLOCKFACE, ClockFrame::HOURS_NUM, ClockFrame::HOURS, ClockFrame::PM};

    case TimeType::PAST:
        switch (language_)
        {
        case Language::HUNGARIAN:
            return {ClockFrame::AFTER, ClockFrame::MINUTES_NUM, ClockFrame::MINUTES,
                    ClockFrame::CLOCKFACE, ClockFrame::HOURS_NUM, ClockFrame::HOURS, ClockFrame::PM};
        default:
            return {ClockFrame::CLOCKFACE, ClockFrame::HOURS_NUM, ClockFrame::HOURS,
                    ClockFrame::AFTER, ClockFrame::MINUTES_NUM, ClockFrame::MINUTES, ClockFrame::PM};
        }

    case TimeType::TO:
        switch (language_)
        {
        case Language::HUNGARIAN:
            return {ClockFrame::MINUTES_NUM, ClockFrame::MINUTES, ClockFrame::BEFORE,
                    ClockFrame::CLOCKFACE, ClockFrame::HOURS_NUM, ClockFrame::HOURS, ClockFrame::PM};
        default:
            return {ClockFrame::BEFORE, ClockFrame::MINUTES_NUM, ClockFrame::MINUTES,
                    ClockFrame::CLOCKFACE, ClockFrame::HOURS_NUM, ClockFrame::HOURS, ClockFrame::PM};
        }

    default:
        return {};
    }
}

LocaleConfig::TimeParameters LocaleConfig::FixTime(const TimeParameters &params) const
{
    TimeParameters fixed_params = params;

    // The hours can be 0-12
    // Normally would be 0-11 but there are coefficients applied for some languages

    // Wrap hours around to 0-11
    fixed_params.hours = fixed_params.hours % 12;

    // Display 0 as 12
    if (fixed_params.hours == 0)
    {
        fixed_params.hours = 12;
    }

    return fixed_params;
}

LocaleConfig::TimeParameters LocaleConfig::ProcessTime(int hh, int mm) const
{
    TimeCoefficients coefficients = GetTimeCoefficients();
    uint32_t minutes = mm;
    uint32_t hours = hh % 12;
    bool pm = ((hh % 24) >= 12);

    // Determine animation type and parameters based on minutes
    if (minutes == 0)
    {
        return FixTime({TimeType::EXACT, hours, 0, Display::ClockFace::UP, pm});
    }
    else if (minutes < 11)
    {
        return FixTime({TimeType::PAST, hours, minutes, Display::ClockFace::UP, pm});
    }
    else if (minutes < 15)
    {
        return FixTime({TimeType::TO, hours + coefficients.after_coef, 15 - minutes, Display::ClockFace::RIGHT, pm});
    }
    else if (minutes == 15)
    {
        return FixTime({TimeType::EXACT, hours + coefficients.after_coef, 0, Display::ClockFace::RIGHT, pm});
    }
    else if (minutes < 20)
    {
        return FixTime({TimeType::PAST, hours + coefficients.after_coef, minutes - 15, Display::ClockFace::RIGHT, pm});
    }
    else if (minutes < 30)
    {
        return FixTime({TimeType::TO, hours + coefficients.half_coef, 30 - minutes, Display::ClockFace::DOWN, pm});
    }
    else if (minutes == 30)
    {
        return FixTime({TimeType::EXACT, hours + coefficients.half_coef, 0, Display::ClockFace::DOWN, pm});
    }
    else if (minutes < 41)
    {
        return FixTime({TimeType::PAST, hours + coefficients.half_coef, minutes - 30, Display::ClockFace::DOWN, pm});
    }
    else if (minutes < 45)
    {
        return FixTime({TimeType::TO, hours + coefficients.to_coef, 45 - minutes, Display::ClockFace::LEFT, pm});
    }
    else if (minutes == 45)
    {
        return FixTime({TimeType::EXACT, hours + coefficients.to_coef, 0, Display::ClockFace::LEFT, pm});
    }
    else if (minutes < 50)
    {
        return FixTime({TimeType::PAST, hours + coefficients.to_coef, minutes - 45, Display::ClockFace::LEFT, pm});
    }
    else
    {
        return FixTime({TimeType::TO, hours + 1, 60 - minutes, Display::ClockFace::UP, pm});
    }
}