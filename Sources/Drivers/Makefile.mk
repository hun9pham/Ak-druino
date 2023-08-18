### C flags
CFLAGS += -ISources/Drivers/STM32F1xx_HAL_Driver/Inc
CFLAGS += -ISources/Drivers/CMSIS/Device/ST/STM32F1xx/Include
CFLAGS += -ISources/Drivers/CMSIS/Include
CFLAGS += -ISources/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy

### CPP flags
CPPFLAGS += -ISources/Drivers/STM32F1xx_HAL_Driver/Inc
CPPFLAGS += -ISources/Drivers/CMSIS/Device/ST/STM32F1xx/Include
CPPFLAGS += -ISources/Drivers/CMSIS/Include
CPPFLAGS += -ISources/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy

### Sources
C_SOURCES += Sources/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c
C_SOURCES += Sources/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c
C_SOURCES += Sources/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c
C_SOURCES += Sources/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c
C_SOURCES += Sources/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c
C_SOURCES += Sources/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_iwdg.c
C_SOURCES += Sources/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c
C_SOURCES += Sources/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c
C_SOURCES += Sources/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.c
C_SOURCES += Sources/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c
C_SOURCES += Sources/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c