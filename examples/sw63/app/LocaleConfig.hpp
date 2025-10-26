#pragma once

#include "animations/ClockFrame.hpp"
#include "app/Display.hpp"

class LocaleConfig
{
public:
    enum class Language
    {
        CZECH,
        ENGLISH,
        GERMAN,
        POLISH,
        HUNGARIAN,
        COUNT
    };

    enum class TimeType
    {
        EXACT,
        PAST,
        TO
    };

    struct TimeCoefficients
    {
        uint32_t after_coef; // how "quarter after" modifies hours
        uint32_t half_coef;  // how "half past" modifies hours
        uint32_t to_coef;    // how "quarter to" modifies hours
    };

    struct TimeParameters
    {
        TimeType time_type;
        uint32_t hours;
        uint32_t minutes;
        Display::ClockFace face;
        bool pm;
    };

    static constexpr Language kDefaultLanguage = Language::CZECH;

    LocaleConfig() : language_(kDefaultLanguage) {}

    void SetLanguage(Language language)
    {
        language_ = language;
    }

    Language GetLanguage() const
    {
        return language_;
    }

    TimeCoefficients GetTimeCoefficients() const
    {
        switch (language_)
        {
        case Language::CZECH:
        case Language::HUNGARIAN:
            return {1, 1, 1};
        case Language::ENGLISH:
            return {0, 0, 1};
        case Language::GERMAN:
        case Language::POLISH:
            return {0, 1, 1};
        default:
            return {0, 0, 1};
        }
    }

    FrameSequence GetSequence(TimeType time_type) const
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

    TimeParameters ProcessTime(int hh, int mm) const
    {
        TimeCoefficients coefficients = GetTimeCoefficients();
        uint32_t minutes = mm;
        uint32_t hours = hh % 12;
        bool pm = (hh >= 12);

        // Determine animation type and parameters based on minutes
        if (minutes == 0)
        {
            return {TimeType::EXACT, hours, 0, Display::ClockFace::UP, pm};
        }
        else if (minutes < 11)
        {
            return {TimeType::PAST, hours, minutes, Display::ClockFace::UP, pm};
        }
        else if (minutes < 15)
        {
            return {TimeType::TO, hours + coefficients.after_coef, 15 - minutes, Display::ClockFace::RIGHT, pm};
        }
        else if (minutes == 15)
        {
            return {TimeType::EXACT, hours + coefficients.after_coef, 0, Display::ClockFace::RIGHT, pm};
        }
        else if (minutes < 20)
        {
            return {TimeType::PAST, hours + coefficients.after_coef, minutes - 15, Display::ClockFace::RIGHT, pm};
        }
        else if (minutes < 30)
        {
            return {TimeType::TO, hours + coefficients.half_coef, 30 - minutes, Display::ClockFace::DOWN, pm};
        }
        else if (minutes == 30)
        {
            return {TimeType::EXACT, hours + coefficients.half_coef, 0, Display::ClockFace::DOWN, pm};
        }
        else if (minutes < 41)
        {
            return {TimeType::PAST, hours + coefficients.half_coef, minutes - 30, Display::ClockFace::DOWN, pm};
        }
        else if (minutes < 45)
        {
            return {TimeType::TO, hours + coefficients.to_coef, 45 - minutes, Display::ClockFace::LEFT, pm};
        }
        else if (minutes == 45)
        {
            return {TimeType::EXACT, hours + coefficients.to_coef, 0, Display::ClockFace::LEFT, pm};
        }
        else if (minutes < 50)
        {
            return {TimeType::PAST, hours + coefficients.to_coef, minutes - 45, Display::ClockFace::LEFT, pm};
        }
        else
        {
            return {TimeType::TO, hours + 1, 60 - minutes, Display::ClockFace::UP, pm};
        }
    }

private:
    Language language_;
};