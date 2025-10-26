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
                HAL_Delay(timings.GetSpeed().pause);
            }

            // Process the next frame and get the delay until the next update
            animation_delay_remaining_ = animation_runner.ProcessNextFrame();
            last_animation_update_ = HAL_GetTick();

            // If the animation finished, clear the display
            if (animation_runner.IsFinished())
            {
                display.Clear();
                display.Update();
            }
        }
    }
    else
    {
        // Display time every 2 seconds
        if (HAL_GetTick() - last_animation_update_ >= 2000)
        {
            DisplayTime();
        }
    }
}

void App::DisplayTime()
{
    // Example time - in real implementation, get from RTC
    int hh = 4;
    int mm = 20;

    // Use LocaleConfig to process the time and determine animation parameters
    auto time_params = locale.ProcessTime(hh, mm);

    // Set the time animation with the processed parameters
    animation_runner.SetAnimation(Animation::Type::TIME, time_params);

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