#include "App.hpp"
#include "stm32l0xx_hal.h"

void App::Init()
{
    display.Init();
    // display.TestProcedure();
}

void App::Loop()
{
    uint32_t current_time = HAL_GetTick();

    // Process animations if one is active
    if (!animation_runner.IsFinished())
    {
        // Check if it's time for the next animation frame
        if (current_time >= last_animation_update_ + animation_delay_remaining_)
        {
            // Handle pause between frames for time display animations
            if (animation_runner.ShouldPauseBetweenFrames())
            {
                display.Clear();
                display.Update();
                HAL_Delay(timings.GetTimePause());
            }

            animation_delay_remaining_ = animation_runner.ProcessNextFrame();
            last_animation_update_ = HAL_GetTick();

            if (animation_runner.IsFinished())
            {
                display.Clear();
                display.Update();
                HAL_Delay(1000); // sleep here in the future
            }
        }
    }
    else
    {
        // Animation finished, display time again after a delay
        static uint32_t last_time_display = 0;
        if (current_time >= last_time_display + 2000) // Display time every 2 seconds
        {
            DisplayTime();
            last_time_display = current_time;
        }
    }
}

void App::DisplayTime()
{
    // Example time - in real implementation, get from RTC
    int hh = 4;
    int mm = 20;

    // Get language-specific coefficients
    auto coefficients = locale.GetTimeCoefficients();

    uint32_t minutes = mm;
    uint32_t hours = hh % 12;
    bool pm = (hh >= 12);

    // Determine animation type and parameters based on minutes
    if (minutes == 0)
    {
        animation_runner.SetAnimation(Animation::Type::EXACT, hours, 0,
                                      Display::ClockFace::UP, pm);
    }
    else if (minutes < 11)
    {
        animation_runner.SetAnimation(Animation::Type::PAST, hours, minutes,
                                      Display::ClockFace::UP, pm);
    }
    else if (minutes < 15)
    {
        animation_runner.SetAnimation(Animation::Type::TO, hours + coefficients.after_coef,
                                      15 - minutes, Display::ClockFace::RIGHT, pm);
    }
    else if (minutes == 15)
    {
        animation_runner.SetAnimation(Animation::Type::EXACT, hours + coefficients.after_coef,
                                      0, Display::ClockFace::RIGHT, pm);
    }
    else if (minutes < 20)
    {
        animation_runner.SetAnimation(Animation::Type::PAST, hours + coefficients.after_coef,
                                      minutes - 15, Display::ClockFace::RIGHT, pm);
    }
    else if (minutes < 30)
    {
        animation_runner.SetAnimation(Animation::Type::TO, hours + coefficients.half_coef,
                                      30 - minutes, Display::ClockFace::DOWN, pm);
    }
    else if (minutes == 30)
    {
        animation_runner.SetAnimation(Animation::Type::EXACT, hours + coefficients.half_coef,
                                      0, Display::ClockFace::DOWN, pm);
    }
    else if (minutes < 41)
    {
        animation_runner.SetAnimation(Animation::Type::PAST, hours + coefficients.half_coef,
                                      minutes - 30, Display::ClockFace::DOWN, pm);
    }
    else if (minutes < 45)
    {
        animation_runner.SetAnimation(Animation::Type::TO, hours + coefficients.to_coef,
                                      45 - minutes, Display::ClockFace::LEFT, pm);
    }
    else if (minutes == 45)
    {
        animation_runner.SetAnimation(Animation::Type::EXACT, hours + coefficients.to_coef,
                                      0, Display::ClockFace::LEFT, pm);
    }
    else if (minutes < 50)
    {
        animation_runner.SetAnimation(Animation::Type::PAST, hours + coefficients.to_coef,
                                      minutes - 45, Display::ClockFace::LEFT, pm);
    }
    else
    {
        animation_runner.SetAnimation(Animation::Type::TO, hours + 1,
                                      60 - minutes, Display::ClockFace::UP, pm);
    }

    // Start the animation
    animation_delay_remaining_ = 0;
    last_animation_update_ = HAL_GetTick();
}

void App::StartIntroAnimation()
{
    animation_runner.SetAnimation(Animation::Type::INTRO);
    animation_delay_remaining_ = 0;
    last_animation_update_ = HAL_GetTick();
}