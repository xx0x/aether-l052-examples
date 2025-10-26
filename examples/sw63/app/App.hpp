#pragma once

#include "app/AnimationRunner.hpp"
#include "app/Display.hpp"
#include "app/LocaleConfig.hpp"
#include "app/TimingsConfig.hpp"
#include <cstdint>

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

private:
    // Animation state
    inline static uint32_t animation_delay_remaining_ = 0;
    inline static uint32_t last_animation_update_ = 0;
};
