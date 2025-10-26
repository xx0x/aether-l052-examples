#pragma once

#include "dev/ShiftRegister.hpp"
#include "dev/PwmBrightness.hpp"
#include "lib/EnumTools.hpp"

class Display
{
public:
    static constexpr uint16_t kMaxBrightness = PwmBrightness::kResolution;

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

    void Init()
    {
        // Initialize PWM brightness control
        brightness_.Init();

        // Shift register for LEDs
        display_register_.Init({
            .data = {GPIOA, GPIO_PIN_7},  // MOSI - PA7
            .clock = {GPIOA, GPIO_PIN_5}, // SCK - PA5
            .latch = {GPIOA, GPIO_PIN_8}  // LATCH - PA8
        });

        SetBrightness(kMaxBrightness / 4); // Start with 25% brightness
        Clear();
        Update();
    }

    void SetBrightness(uint16_t brightness)
    {
        brightness_.Set(brightness);
    }

    void Clear()
    {
        data_.reset();
    }

    void Update()
    {
        display_register_.Write(data_);
    }

    void SetClockFace(ClockFace face)
    {
        data_.set(static_cast<size_t>(Led::UP), ClockFace::UP == face);
        data_.set(static_cast<size_t>(Led::RIGHT), ClockFace::RIGHT == face);
        data_.set(static_cast<size_t>(Led::DOWN), ClockFace::DOWN == face);
        data_.set(static_cast<size_t>(Led::LEFT), ClockFace::LEFT == face);
    }

    void SetPm(bool on)
    {
        data_.set(static_cast<size_t>(Led::PM), on);
    }

    void SetTopLed(TopLed led)
    {
        data_.set(static_cast<size_t>(Led::HOURS), TopLed::HOURS == led);
        data_.set(static_cast<size_t>(Led::MINUTES), TopLed::MINUTES == led);
        data_.set(static_cast<size_t>(Led::AFTER), TopLed::AFTER == led);
        data_.set(static_cast<size_t>(Led::BEFORE), TopLed::BEFORE == led);
    }

    void SetNumber(size_t number)
    {
        for (auto led : kNumLeds)
        {
            data_.set(static_cast<size_t>(led), false);
        }
        if (number == 0 || number > kNumLedCount)
        {
            return;
        }

        switch (num_mode_)
        {
        case NumMode::SINGLE:
            data_.set(static_cast<size_t>(kNumLeds[number - 1]), true);
            break;

        case NumMode::SINGLE_REVERSED:
            data_.set(static_cast<size_t>(kRevNumLeds[number - 1]), true);
            break;

        case NumMode::BAR:
            for (size_t i = 0; i < number; ++i)
            {
                data_.set(static_cast<size_t>(kNumLeds[i]), true);
            }
            break;

        case NumMode::BAR_REVERSED:
            for (size_t i = 0; i < number; ++i)
            {
                data_.set(static_cast<size_t>(kRevNumLeds[i]), true);
            }
            break;
        }
    }

    void TestProcedure()
    {
        for (int i = 0; i <= 12; i++)
        {
            Clear();
            SetNumber(i);
            Update();
            HAL_Delay(200);
        }

        for (auto top_led : EnumRange<Display::TopLed>())
        {
            Clear();
            SetTopLed(top_led);
            Update();
            HAL_Delay(200);
        }

        for (auto clockface : EnumRange<Display::ClockFace>())
        {
            Clear();
            SetClockFace(clockface);
            Update();
            HAL_Delay(200);
        }

        for (int i = 0; i < 3; i++)
        {
            Clear();
            SetPm(true);
            Update();
            HAL_Delay(200);
            SetPm(false);
            Update();
            HAL_Delay(200);
        }
    }

private:
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

    static constexpr size_t kLedCount = 24;
    std::bitset<kLedCount> data_;
    ShiftRegister<kLedCount> display_register_;
    PwmBrightness brightness_;

    static constexpr size_t kNumLedCount = 12;
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
