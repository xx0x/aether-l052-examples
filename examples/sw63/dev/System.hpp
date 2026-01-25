#pragma once

#include "stm32l0xx_hal.h"

class System
{
public:
    // Static member variables
    inline static I2C_HandleTypeDef hi2c1;

    // Static methods
    static void Init();
    static void ClockConfig();
    static void GpioInit();
    static void I2cInit();
    static void ErrorHandler();
    static void Sleep();
    static inline void Delay(uint32_t ms)
    {
        HAL_Delay(ms);
    }

private:
    System() = delete; // Prevent instantiation
};