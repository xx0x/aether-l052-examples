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

    void SetLanguage(Language language);
    Language GetLanguage() const;
    FrameSequence GetSequence(TimeType time_type) const;
    TimeParameters ProcessTime(int hh, int mm) const;
    TimeParameters FixTime(const TimeParameters &params) const;

private:
    TimeCoefficients GetTimeCoefficients() const;
    Language language_;
};