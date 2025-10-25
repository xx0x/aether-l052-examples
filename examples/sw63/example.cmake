# Example name
set(EXAMPLE_NAME sw63)

# Example-specific sources (relative to this directory)
set(EXAMPLE_SOURCES
    main.cpp
)

# Example-specific HAL dependencies
set(EXAMPLE_HAL_SOURCES
    ${HAL_DRIVER_PATH}/Src/stm32l0xx_hal.c
    ${HAL_DRIVER_PATH}/Src/stm32l0xx_hal_cortex.c
    ${HAL_DRIVER_PATH}/Src/stm32l0xx_hal_dma.c
    ${HAL_DRIVER_PATH}/Src/stm32l0xx_hal_flash.c
    ${HAL_DRIVER_PATH}/Src/stm32l0xx_hal_flash_ex.c
    ${HAL_DRIVER_PATH}/Src/stm32l0xx_hal_gpio.c
    ${HAL_DRIVER_PATH}/Src/stm32l0xx_hal_pwr.c
    ${HAL_DRIVER_PATH}/Src/stm32l0xx_hal_pwr_ex.c
    ${HAL_DRIVER_PATH}/Src/stm32l0xx_hal_rcc.c
    ${HAL_DRIVER_PATH}/Src/stm32l0xx_hal_rcc_ex.c
    ${HAL_DRIVER_PATH}/Src/stm32l0xx_hal_tim.c
)

# Example-specific includes (relative to this directory) 
set(EXAMPLE_INCLUDES
    .
)

# Additional compile definitions (if any)
set(EXAMPLE_DEFINITIONS
    # Add any example-specific definitions here
)