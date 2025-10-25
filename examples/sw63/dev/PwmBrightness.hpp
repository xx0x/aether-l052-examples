#pragma once

#include "stm32l0xx_hal.h"

/**
 * @brief Class to manage PWM brightness control
 */
class PwmBrightness
{
public:
    static constexpr uint16_t kResolution = 1023; // 10-bit max value

    /*
    // Unused for now
    struct Config
    {
        GPIO_TypeDef *port;
        uint16_t pin;
        uint32_t alternate;
        TIM_TypeDef *tim_instance;
        uint32_t tim_channel;
        uint16_t resolution;
    };
    */

    /**
     * @brief Initializes PWM for brightness control on TIM22_CH1 (PA6)
     * @return True if initialization was successful, false otherwise
     */
    bool Init()
    {
        TIM_OC_InitTypeDef sConfigOC = {};
        GPIO_InitTypeDef GPIO_InitStruct = {};

        // Enable TIM22 clock
        __HAL_RCC_TIM22_CLK_ENABLE();

        // Configure PA6 as TIM22_CH1 (AF5)
        GPIO_InitStruct.Pin = GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_TIM22;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        // Configure TIM22 for PWM
        // With 16MHz clock, prescaler 0, and period 1023:
        // PWM frequency = 16,000,000 / (0+1) / (1023+1) = ~15.6 kHz
        htim_.Instance = TIM22;
        htim_.Init.Prescaler = 0; // No prescaling for maximum speed
        htim_.Init.CounterMode = TIM_COUNTERMODE_UP;
        htim_.Init.Period = kResolution;
        htim_.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        htim_.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

        if (HAL_TIM_PWM_Init(&htim_) != HAL_OK)
        {
            return false;
        }

        // Configure PWM channel 1
        sConfigOC.OCMode = TIM_OCMODE_PWM1;
        sConfigOC.Pulse = 0; // Start with 0% duty cycle
        sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

        if (HAL_TIM_PWM_ConfigChannel(&htim_, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
        {
            return false;
        }

        // Start PWM generation
        if (HAL_TIM_PWM_Start(&htim_, TIM_CHANNEL_1) != HAL_OK)
        {
            return false;
        }

        // Initialization successful
        initialized_ = true;
        return true;
    }

    /**
     * @brief Sets brightness level from 0 to PwmBrightness::kResolution
     * @param brightness Brightness level (0 = minimum, PwmBrightness::kResolution = full brightness)
     */
    void Set(uint16_t brightness)
    {
        if (!initialized_)
        {
            return;
        }

        // Clamp brightness to 10-bit range
        if (brightness > kResolution)
        {
            brightness = kResolution;
        }

        // Invert brightness for OE pin (kResolution - brightness)
        // This way: 0 = LEDs off, kResolution = LEDs full brightness
        uint16_t inverted_brightness = kResolution - brightness;

        // Update PWM duty cycle
        __HAL_TIM_SET_COMPARE(&htim_, TIM_CHANNEL_1, inverted_brightness);
    }

    /**
     * @brief Sets the lowest brightness
     */
    void Min()
    {
        Set(0);
    }

    /**
     * @brief Sets the full brightness
     */
    void Max()
    {
        Set(kResolution);
    }

private:
    TIM_HandleTypeDef htim_;
    bool initialized_ = false;
};
