### C flags
CFLAGS += -ISources/Middlewares/Third_Party/FreeRTOS/Source
CFLAGS += -ISources/Middlewares/Third_Party/FreeRTOS/Source/include
CFLAGS += -ISources/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
CFLAGS += -ISources/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3

### CPP flags
CPPFLAGS += -ISources/Middlewares/Third_Party/FreeRTOS/Source
CPPFLAGS += -ISources/Middlewares/Third_Party/FreeRTOS/Source/include
CPPFLAGS += -ISources/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
CPPFLAGS += -ISources/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3

### Sources
C_SOURCES += Sources/Middlewares/Third_Party/FreeRTOS/Source/croutine.c
C_SOURCES += Sources/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
C_SOURCES += Sources/Middlewares/Third_Party/FreeRTOS/Source/list.c
C_SOURCES += Sources/Middlewares/Third_Party/FreeRTOS/Source/queue.c
C_SOURCES += Sources/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
C_SOURCES += Sources/Middlewares/Third_Party/FreeRTOS/Source/tasks.c
C_SOURCES += Sources/Middlewares/Third_Party/FreeRTOS/Source/timers.c
C_SOURCES += Sources/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
C_SOURCES += Sources/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
C_SOURCES += Sources/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3/port.c
