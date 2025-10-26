#pragma once

#include "animations/ClockFrame.hpp"
#include "app/Display.hpp"

class LocaleConfig
{
public:
    /**
     * @brief Supported languages for the clock.
     * @details When adding a new language, ensure to update LocaleConfig.cpp:
     *          - GetTimeCoefficients() with appropriate coefficients
     *          - GetSequence() with the correct frame sequences
     *          - ProcessTime() if any special handling is needed (hopefully not needed)
     */
    enum class Language
    {
        CZECH,
        ENGLISH,
        GERMAN,
        POLISH,
        HUNGARIAN,
        COUNT
    };

    /**
     * @brief Each language has its own coefficients for quarter/half/to adjustments.
     */
    struct TimeCoefficients
    {
        uint32_t after_coef; // how "quarter after" modifies hours
        uint32_t half_coef;  // how "half past" modifies hours
        uint32_t to_coef;    // how "quarter to" modifies hours
    };

    /**
     * @brief Types of time expressions passed back and forth.
     */
    enum class TimeType
    {
        EXACT,
        PAST,
        TO
    };

    /**
     * @brief Structure to hold processed time parameters.
     */
    struct TimeParameters
    {
        TimeType time_type;
        uint32_t hours;
        uint32_t minutes;
        Display::ClockFace face;
        bool pm;
    };

    /**
     * @brief Default language for the clock.
     */
    static constexpr Language kDefaultLanguage = static_cast<Language>(0);

    /**
     * @brief     Constructor for LocaleConfig.
     */
    LocaleConfig() : language_(kDefaultLanguage) {}

    /**
     * @brief     Set the current language.
     * @param     language  The language to set.
     */
    void SetLanguage(Language language);

    /**
     * @brief     Get the current language.
     * @return    The current language.
     */
    Language GetLanguage() const;

    /**
     * @brief     Get the sequence of frames for the given time type and language.
     * @param     time_type  The type of time (EXACT, PAST, TO).
     * @return    A FrameSequence containing the frames to display.
     */
    FrameSequence GetSequence(TimeType time_type) const;

    /**
     * @brief     Process the hh:mm time to "human-like" time parameters.
     * @details   This function takes hours and minutes, and returns the appropriate
     *            TimeParameters for the current language.
     * @param     hours  Hours in 24-hour format.
     * @param     minutes  Minutes in 60-minute format.
     * @return    Processed TimeParameters structure.
     */
    TimeParameters ProcessTime(const uint32_t hours, const uint32_t minutes) const;

    /**
     * @brief     Ensure hours are in the correct range the display can show (1-12).
     * @details   The hours can be 0-12
     *            Normally would be 0-11 but there are coefficients applied for some languages
     * @param     params  Input TimeParameters structure.
     * @return    Fixed TimeParameters structure.
     */
    TimeParameters FixTime(const TimeParameters &params) const;

private:
    /**
     * @brief     Get the time coefficients for the current language.
     * @return    TimeCoefficients structure with the coefficients.
     */
    TimeCoefficients GetTimeCoefficients() const;

    /**
     * @brief Current language setting.
     */
    Language language_;
};