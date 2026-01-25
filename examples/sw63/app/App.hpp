#pragma once

#include "app/AnimationRunner.hpp"
#include "app/Display.hpp"
#include "app/LocaleConfig.hpp"
#include "app/TimingsConfig.hpp"
#include <cstdint>
#include "dev/DS3231.hpp"
#include "stm32l0xx_hal.h"

// External I2C handle declaration
extern I2C_HandleTypeDef hi2c1;

class App
{
public:
    static void Init();
    static void Loop();

    static void DisplayTime();
    static void SetSpeed(uint32_t speed) { timings.SetSpeed(speed); }
    static void StartIntroAnimation();

    inline static Display display;
    inline static TimingsConfig timings;
    inline static LocaleConfig locale;
    inline static AnimationRunner animation_runner;
    inline static DS3231 rtc = DS3231(&hi2c1);

private:
    // Animation state
    inline static uint32_t animation_delay_remaining_ = 0;
    inline static uint32_t last_animation_update_ = 0;
};
