#include "Display.hpp"

void Display::Init()
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

void Display::SetBrightness(uint16_t brightness)
{
    brightness_.Set(brightness);
}

void Display::Clear()
{
    data_.reset();
}

void Display::Update()
{
    display_register_.Write(data_);
}

void Display::SetClockFace(ClockFace face)
{
    data_.set(static_cast<size_t>(Led::UP), ClockFace::UP == face);
    data_.set(static_cast<size_t>(Led::RIGHT), ClockFace::RIGHT == face);
    data_.set(static_cast<size_t>(Led::DOWN), ClockFace::DOWN == face);
    data_.set(static_cast<size_t>(Led::LEFT), ClockFace::LEFT == face);
}

void Display::SetPm(bool on)
{
    data_.set(static_cast<size_t>(Led::PM), on);
}

void Display::SetTopLed(TopLed led)
{
    data_.set(static_cast<size_t>(Led::HOURS), TopLed::HOURS == led);
    data_.set(static_cast<size_t>(Led::MINUTES), TopLed::MINUTES == led);
    data_.set(static_cast<size_t>(Led::AFTER), TopLed::AFTER == led);
    data_.set(static_cast<size_t>(Led::BEFORE), TopLed::BEFORE == led);
}

void Display::SetNumber(size_t number)
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

void Display::SetLed(Led led, bool on)
{
    data_.set(static_cast<size_t>(led), on);
}

void Display::TestProcedure()
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