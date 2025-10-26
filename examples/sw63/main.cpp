#include "stm32l0xx_hal.h"
#include "stm32l052xx.h"
#include <bitset>
#include "app/App.hpp"

// Global variables
volatile bool button_just_pressed = false;

// Function prototypes
void SystemClock_Config(void);
void GPIO_Init(void);
void Error_Handler(void);

/**
 * @brief Main program entry point
 * @retval int
 */
int main(void)
{
    // Reset of all peripherals, Initializes the Flash interface and the Systick
    HAL_Init();

    // Configure the system clock to use HSI (16MHz)
    SystemClock_Config();

    // Enable power control clock
    __HAL_RCC_PWR_CLK_ENABLE();

    // Additional power optimizations
    // Disable power voltage detector (PVD) to save power
    HAL_PWR_DisablePVD();

    // Initialize GPIO
    GPIO_Init();

    // Initialize App
    App app;
    app.Init();

    // Main loop
    while (1)
    {
        // Clear button pressed flag and any pending EXTI interrupt
        button_just_pressed = false;
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);

        app.Loop();
    }
}

/**
 * @brief System Clock Configuration using internal HSI oscillator
 * @details Configures the system clock to run at 16MHz using HSI
 *          This is crystalless configuration suitable for low-power applications
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {};

    // Configure the main internal regulator output voltage
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    // Initialize the RCC Oscillators according to the specified parameters
    // Use HSI (High Speed Internal) 16MHz oscillator
    // Explicitly disable LSE, LSI, and RTC for minimum power consumption
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_LSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.LSEState = RCC_LSE_OFF;      // Disable LSE for power saving
    RCC_OscInitStruct.LSIState = RCC_LSI_OFF;      // Disable LSI for power saving
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE; // Don't use PLL for simplicity

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    // Initialize the CPU, AHB and APB bus clocks
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1; // 16MHz
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  // 16MHz
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  // 16MHz

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();
    }

    // Ensure RTC is completely disabled
    __HAL_RCC_RTC_DISABLE();
}

/**
 * @brief GPIO Initialization for LED on PA15
 * @details Configures PA15 as digital output for LED control
 */
void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {};

    // GPIO Ports Clock Enable
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Configure GPIO pin Output Level - start with LED OFF
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);

    // Configure GPIO pin PA15 for LED
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // Push-pull output
    GPIO_InitStruct.Pull = GPIO_NOPULL;          // No pull-up/pull-down
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // Low speed is sufficient for LED
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure GPIO pin PA00 for BUTTON with interrupt
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING; // Interrupt on falling edge (button press)
    GPIO_InitStruct.Pull = GPIO_PULLUP;          // Pull-up resistor
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Enable EXTI0 interrupt for button
    HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
}

/**
 * @brief Error Handler
 * @details This function is executed in case of error occurrence
 *          In a real application, you might want to implement proper error handling
 */
void Error_Handler(void)
{
    // User can add his own implementation to report the HAL error return state
    __disable_irq();
    while (1)
    {
        // Infinite loop in case of error
        // You could blink LED in a different pattern to indicate error
    }
}

/**
 * @brief SysTick interrupt handler
 * @note This callback is called every 1ms by SysTick interrupt
 *       Required for HAL_Delay() to work properly
 */
extern "C" void SysTick_Handler(void)
{
    HAL_IncTick();
}

/**
 * @brief Hard Fault interrupt handler
 * @note Basic fault handler - in production code you'd want more sophisticated handling
 */
extern "C" void HardFault_Handler(void)
{
    while (1)
    {
        // Stay here in case of Hard Fault
    }
}

/**
 * @brief Non-Maskable Interrupt handler
 */
extern "C" void NMI_Handler(void)
{
    // Handle NMI if needed
}

/**
 * @brief EXTI0_1 interrupt handler for button wakeup
 * @note Handles external interrupt on PA0 (button) to wake up from stop mode
 */
extern "C" void EXTI0_1_IRQHandler(void)
{
    // Check if the interrupt is from PA0
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET)
    {
        // Clear the interrupt flag
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);

        // Set the global button pressed flag
        button_just_pressed = true;

        // The system will automatically exit from stop mode
        // No additional code needed here for basic wakeup
    }
}