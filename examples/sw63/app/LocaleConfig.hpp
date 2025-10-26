#pragma once

#include "animations/ClockFrame.hpp"

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

    struct TimeCoefficients
    {
        int32_t after_coef; // how "quarter after" modifies hours
        int32_t half_coef;  // how "half past" modifies hours
        int32_t to_coef;    // how "quarter to" modifies hours
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

    FrameSequence GetPastSequence() const
    {
        switch (language_)
        {
        case Language::HUNGARIAN:
            return {ClockFrame::AFTER, ClockFrame::MINUTES_NUM, ClockFrame::MINUTES,
                    ClockFrame::CLOCKFACE, ClockFrame::HOURS_NUM, ClockFrame::HOURS, ClockFrame::PM};
        default:
            return {ClockFrame::CLOCKFACE, ClockFrame::HOURS_NUM, ClockFrame::HOURS,
                    ClockFrame::AFTER, ClockFrame::MINUTES_NUM, ClockFrame::MINUTES, ClockFrame::PM};
        }
    }

    FrameSequence GetToSequence() const
    {
        switch (language_)
        {
        case Language::HUNGARIAN:
            return {ClockFrame::MINUTES_NUM, ClockFrame::MINUTES, ClockFrame::BEFORE,
                    ClockFrame::CLOCKFACE, ClockFrame::HOURS_NUM, ClockFrame::HOURS, ClockFrame::PM};
        default:
            return {ClockFrame::BEFORE, ClockFrame::MINUTES_NUM, ClockFrame::MINUTES,
                    ClockFrame::CLOCKFACE, ClockFrame::HOURS_NUM, ClockFrame::HOURS, ClockFrame::PM};
        }
    }

    FrameSequence GetExactSequence() const
    {
        // Exact time is the same for all languages
        return {ClockFrame::CLOCKFACE, ClockFrame::HOURS_NUM, ClockFrame::HOURS, ClockFrame::PM};
    }

private:
    Language language_;
};