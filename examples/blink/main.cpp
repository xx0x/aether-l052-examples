/**
 * @file main.cpp
 * @brief Arduino-style blink example for STM32L052K8U3
 * @details Simple blink program that toggles LED on PA15 every 500ms
 *          Uses internal HSI oscillator (16MHz) and HAL library
 * 
 * Hardware: STM32L052K8U3 (32KB Flash, 8KB RAM)
 * LED: Connected to PA15
 * Crystal: Crystalless design using internal HSI
 */

#include "stm32l0xx_hal.h"
#include "stm32l052xx.h"

// Function prototypes
void SystemClock_Config(void);
void GPIO_Init(void);
void delay(uint32_t ms);
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

    // Initialize GPIO for LED
    GPIO_Init();

    // Main loop - Arduino style
    while (1)
    {
        // Turn LED ON
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
        delay(100);  // Wait 100ms

        // Turn LED OFF  
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
        delay(900);  // Wait 900ms
    }
}

/**
 * @brief System Clock Configuration using internal HSI oscillator
 * @details Configures the system clock to run at 16MHz using HSI
 *          This is crystalless configuration suitable for low-power applications
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // Configure the main internal regulator output voltage
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    // Initialize the RCC Oscillators according to the specified parameters
    // Use HSI (High Speed Internal) 16MHz oscillator
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;  // Don't use PLL for simplicity
    
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    // Initialize the CPU, AHB and APB bus clocks  
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | 
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;   // 16MHz
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;    // 16MHz  
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;    // 16MHz

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief GPIO Initialization for LED on PA15
 * @details Configures PA15 as digital output for LED control
 */
void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // GPIO Ports Clock Enable
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Configure GPIO pin Output Level - start with LED OFF
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);

    // Configure GPIO pin PA15 for LED
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;      // Push-pull output
    GPIO_InitStruct.Pull = GPIO_NOPULL;              // No pull-up/pull-down
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;     // Low speed is sufficient for LED
    
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
 * @brief Simple delay function using HAL_Delay
 * @param ms Delay time in milliseconds
 * @note This function uses HAL_Delay which relies on SysTick timer
 */
void delay(uint32_t ms)
{
    HAL_Delay(ms);
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