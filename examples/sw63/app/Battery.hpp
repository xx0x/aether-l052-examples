#pragma once
#include <algorithm>
#include "dev/AdcInput.hpp"
#include "dev/Pin.hpp"
#include "lib/math_utils.hpp"
#include "stm32l0xx_hal.h"

class Battery
{
private:
    // Voltage divider resistors (in ohms)
    static constexpr float R1 = 100000.0f; // 100kΩ (high side)
    static constexpr float R2 = 220000.0f; // 220kΩ (low side to ground)

    // Voltage divider ratio (measurement point voltage / battery voltage)
    static constexpr float VOLTAGE_DIVIDER_RATIO = R2 / (R1 + R2);

public:
    void Init()
    {
        // Initialize battery ADC (PA3)
        battery_adc_.Init({.port = GPIOA,
                           .pin = GPIO_PIN_3,
                           .channel = ADC_CHANNEL_3});

        // Initialize GPIO for battery measurement enable (PB1)
        GPIO_InitTypeDef GPIO_InitStruct = {
            .Pin = GPIO_PIN_1,
            .Mode = GPIO_MODE_OUTPUT_PP,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
            .Alternate = 0,
        };
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // Enable measurement
        Pin::EnablePort({GPIOB, GPIO_PIN_1});
    }

    float GetLevel()
    {
        // Read battery voltage via ADC
        uint32_t adc_value = battery_adc_.ReadSingleShot();

        // Convert ADC value to voltage at measurement point
        // ADC: 12-bit (0-4095), 0-3.3V range
        float measured_voltage = (static_cast<float>(adc_value) / static_cast<float>(AdcInput::kResolution)) * 3.3f;

        // Convert to actual battery voltage using voltage divider ratio
        // Divider: VBAT - R1 - measurement - R2 - GND
        float battery_voltage = measured_voltage / VOLTAGE_DIVIDER_RATIO;

        // Map battery voltage range to 0.0-1.0
        // 4.2V = full (1.0), 2.8V = empty (0.0)
        float level = mapf(battery_voltage, 2.8f, 4.2f, 0.0f, 1.0f);

        // Clamp to valid range
        return std::clamp(level, 0.0f, 1.0f);
    }

    void DeInit()
    {
        battery_adc_.DeInit();
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_1);
    }

private:
    AdcInput battery_adc_;
};
