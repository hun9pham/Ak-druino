### C flags
CFLAGS += -ISources/Core/Inc

### CPP flags
CPPFLAGS += -ISources/Core/Inc

### Sources
C_SOURCES += Sources/Core/Src/main.c
C_SOURCES += Sources/Core/Src/freertos.c
C_SOURCES += Sources/Core/Src/stm32f1xx_hal_msp.c
C_SOURCES += Sources/Core/Src/stm32f1xx_it.c
C_SOURCES += Sources/Core/Src/system_stm32f1xx.c
C_SOURCES += Sources/Core/Src/sys_cfgCtl.c
C_SOURCES += Sources/Core/Src/sys_io.c
C_SOURCES += Sources/Core/Src/sys_dbg.c