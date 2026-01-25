#pragma once

#include "dev/AdcInput.hpp"
#include "dev/PwmBrightness.hpp"
#include "dev/ShiftRegister.hpp"
#include "lib/EnumTools.hpp"

class Display
{
public:
    static constexpr uint16_t kMaxBrightness = PwmBrightness::kResolution;

    // Change this based on the actual LED wiring
    enum class Led
    {
        LEFT,
        DOWN,
        RIGHT,
        UP,
        PM,
        UNUSED2,
        UNUSED1,
        UNUSED0,
        BEFORE,
        AFTER,
        HOURS,
        MINUTES,
        NUM_12,
        NUM_11,
        NUM_10,
        NUM_9,
        NUM_1,
        NUM_2,
        NUM_3,
        NUM_4,
        NUM_8,
        NUM_7,
        NUM_6,
        NUM_5,
    };

    enum class ClockFace
    {
        UP,
        RIGHT,
        DOWN,
        LEFT,
        COUNT,
    };

    enum class TopLed
    {
        BEFORE,
        AFTER,
        HOURS,
        MINUTES,
        COUNT,
    };

    enum class NumMode
    {
        SINGLE,
        SINGLE_REVERSED,
        BAR,
        BAR_REVERSED, // OG behavior
        COUNT
    };

    void Init();

    void DeInit();

    void TriggerAutoBrightness();

    void Clear();

    void Update();

    void SetClockFace(ClockFace face);

    void SetPm(bool on);

    void SetTopLed(TopLed led);

    void SetNumber(size_t number);

    void SetLed(Led led, bool on);

    void TestProcedure();

private:
    void SetPower(bool on);

    static constexpr size_t kLedCount = 24;
    static constexpr size_t kNumLedCount = 12;

    static constexpr Pin kDisplayEnablePin = {GPIOA, GPIO_PIN_1};

    std::bitset<kLedCount> data_;
    ShiftRegister<kLedCount> display_register_;
    PwmBrightness brightness_;
    AdcInput ambient_light_;
    NumMode num_mode_ = NumMode::BAR_REVERSED;

    // Lookup tables for different modes
    // Don't change the order of these arrays.
    static constexpr Led kNumLeds[kNumLedCount] = {
        Led::NUM_1, Led::NUM_2, Led::NUM_3, Led::NUM_4, Led::NUM_5, Led::NUM_6,
        Led::NUM_7, Led::NUM_8, Led::NUM_9, Led::NUM_10, Led::NUM_11, Led::NUM_12};

    static constexpr Led kRevNumLeds[kNumLedCount] = {
        Led::NUM_12, Led::NUM_11, Led::NUM_10, Led::NUM_9, Led::NUM_8, Led::NUM_7,
        Led::NUM_6, Led::NUM_5, Led::NUM_4, Led::NUM_3, Led::NUM_2, Led::NUM_1};
};
