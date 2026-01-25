#include "App.hpp"
#include "dev/System.hpp"
#include "stm32l0xx_hal.h"

void App::Init()
{
    display.Init();
    auto rtc_success = rtc.Init();
    if (!rtc_success)
    {
        while (1)
        {
            display.Clear();
            display.SetPm(true); // Indicate error with PM LED
            display.Update();
            HAL_Delay(500);
            display.Clear();
            display.SetPm(false);
            display.Update();
            HAL_Delay(500);
        }
    }
    // Set test date (we don't care about the date here, just time)
    rtc.SetDateTime({0, 45, 13, 1, 18, 8, 2025});

    DisplayTime();
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
        // if (HAL_GetTick() - last_animation_update_ >= 2000)
        //{
        //    DisplayTime();
        //}
        Sleep();

        DisplayTime();
    }
}

void App::DisplayTime()
{
    auto now = rtc.GetDateTime();
    if (!now)
    {
        return; // Failed to get time
    }

    // Use LocaleConfig to process the time and determine animation parameters
    auto time_params = locale.ProcessTime(now->hour, now->minute);

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

void App::Sleep()
{
    display.DeInit();

    System::Sleep();

    display.Init();

    // Small delay for button debouncing after wakeup
    HAL_Delay(50);
}