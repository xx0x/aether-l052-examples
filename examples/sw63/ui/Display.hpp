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
        if (number == 0 || number > 12)
        {
            data_.set(static_cast<size_t>(Led::NUM_1), false);
            data_.set(static_cast<size_t>(Led::NUM_2), false);
            data_.set(static_cast<size_t>(Led::NUM_3), false);
            data_.set(static_cast<size_t>(Led::NUM_4), false);
            data_.set(static_cast<size_t>(Led::NUM_5), false);
            data_.set(static_cast<size_t>(Led::NUM_6), false);
            data_.set(static_cast<size_t>(Led::NUM_7), false);
            data_.set(static_cast<size_t>(Led::NUM_8), false);
            data_.set(static_cast<size_t>(Led::NUM_9), false);
            data_.set(static_cast<size_t>(Led::NUM_10), false);
            data_.set(static_cast<size_t>(Led::NUM_11), false);
            data_.set(static_cast<size_t>(Led::NUM_12), false);
            return;
        }

        if (num_mode_ == NumMode::BAR)
        {
            data_.set(static_cast<size_t>(Led::NUM_1), number >= 1);
            data_.set(static_cast<size_t>(Led::NUM_2), number >= 2);
            data_.set(static_cast<size_t>(Led::NUM_3), number >= 3);
            data_.set(static_cast<size_t>(Led::NUM_4), number >= 4);
            data_.set(static_cast<size_t>(Led::NUM_5), number >= 5);
            data_.set(static_cast<size_t>(Led::NUM_6), number >= 6);
            data_.set(static_cast<size_t>(Led::NUM_7), number >= 7);
            data_.set(static_cast<size_t>(Led::NUM_8), number >= 8);
            data_.set(static_cast<size_t>(Led::NUM_9), number >= 9);
            data_.set(static_cast<size_t>(Led::NUM_10), number >= 10);
            data_.set(static_cast<size_t>(Led::NUM_11), number >= 11);
            data_.set(static_cast<size_t>(Led::NUM_12), number >= 12);
        }
        else if (num_mode_ == NumMode::BAR_REVERSED)
        {
            data_.set(static_cast<size_t>(Led::NUM_12), number >= 1);
            data_.set(static_cast<size_t>(Led::NUM_11), number >= 2);
            data_.set(static_cast<size_t>(Led::NUM_10), number >= 3);
            data_.set(static_cast<size_t>(Led::NUM_9), number >= 4);
            data_.set(static_cast<size_t>(Led::NUM_8), number >= 5);
            data_.set(static_cast<size_t>(Led::NUM_7), number >= 6);
            data_.set(static_cast<size_t>(Led::NUM_6), number >= 7);
            data_.set(static_cast<size_t>(Led::NUM_5), number >= 8);
            data_.set(static_cast<size_t>(Led::NUM_4), number >= 9);
            data_.set(static_cast<size_t>(Led::NUM_3), number >= 10);
            data_.set(static_cast<size_t>(Led::NUM_2), number >= 11);
            data_.set(static_cast<size_t>(Led::NUM_1), number >= 12);
        }
        else if (num_mode_ == NumMode::SINGLE)
        {
            data_.set(static_cast<size_t>(Led::NUM_1), number == 1);
            data_.set(static_cast<size_t>(Led::NUM_2), number == 2);
            data_.set(static_cast<size_t>(Led::NUM_3), number == 3);
            data_.set(static_cast<size_t>(Led::NUM_4), number == 4);
            data_.set(static_cast<size_t>(Led::NUM_5), number == 5);
            data_.set(static_cast<size_t>(Led::NUM_6), number == 6);
            data_.set(static_cast<size_t>(Led::NUM_7), number == 7);
            data_.set(static_cast<size_t>(Led::NUM_8), number == 8);
            data_.set(static_cast<size_t>(Led::NUM_9), number == 9);
            data_.set(static_cast<size_t>(Led::NUM_10), number == 10);
            data_.set(static_cast<size_t>(Led::NUM_11), number == 11);
            data_.set(static_cast<size_t>(Led::NUM_12), number == 12);
        }
        else if (num_mode_ == NumMode::SINGLE_REVERSED)
        {
            data_.set(static_cast<size_t>(Led::NUM_12), number == 1);
            data_.set(static_cast<size_t>(Led::NUM_11), number == 2);
            data_.set(static_cast<size_t>(Led::NUM_10), number == 3);
            data_.set(static_cast<size_t>(Led::NUM_9), number == 4);
            data_.set(static_cast<size_t>(Led::NUM_8), number == 5);
            data_.set(static_cast<size_t>(Led::NUM_7), number == 6);
            data_.set(static_cast<size_t>(Led::NUM_6), number == 7);
            data_.set(static_cast<size_t>(Led::NUM_5), number == 8);
            data_.set(static_cast<size_t>(Led::NUM_4), number == 9);
            data_.set(static_cast<size_t>(Led::NUM_3), number == 10);
            data_.set(static_cast<size_t>(Led::NUM_2), number == 11);
            data_.set(static_cast<size_t>(Led::NUM_1), number == 12);
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

    static constexpr size_t kNumLeds = 24;
    std::bitset<kNumLeds> data_;
    ShiftRegister<kNumLeds> display_register_;
    PwmBrightness brightness_;

    NumMode num_mode_ = NumMode::BAR_REVERSED;
};
