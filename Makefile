# ---------------------------------------------------------
# 		M	A	K	E	F	I	L	E
# ---------------------------------------------------------
# Author: HungPNQ
# Version: 1.0
# ChangeLog :
#	25-07-2023 - Generate form STM32CubeMX and adjusting
# ---------------------------------------------------------

#### Compiler path
GCC_PATH 		= $(HOME)/SoftInst/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux/gcc-arm-none-eabi-10.3-2021.10
PROGRAMER_PATH 	= $(HOME)/SoftInst/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer.sh

# The command for calling the compiler.
CC		=	$(GCC_PATH)/bin/arm-none-eabi-gcc
CPP		=	$(GCC_PATH)/bin/arm-none-eabi-g++
AR		=	$(GCC_PATH)/bin/arm-none-eabi-ar
AS		=	$(GCC_PATH)/bin/arm-none-eabi-gcc -x assembler-with-cpp
LD		=	$(GCC_PATH)/bin/arm-none-eabi-ld
CP		=	$(GCC_PATH)/bin/arm-none-eabi-objcopy
SZ		=	$(GCC_PATH)/bin/arm-none-eabi-size

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

### Utilities define
Print = @echo "~"

#|---------------------------------------------------------------------------------------------------|
#| OPTIMIZE LEVELS                                                                                   |
#|------------|----------------------------------|--------------|---------|------------|-------------|
#|   option   | optimization levels              |execution time|code size|memory usage|complile time|
#|------------|----------------------------------|--------------|---------|------------|-------------|
#|   -O0      | compilation time                 |     (+)      |   (+)   |     (-)    |    (-)      |
#| -O1 || -O  | code size && execution time      |     (-)      |   (-)   |     (+)    |    (+)      |
#|   -O2      | more code size && execution time |     (--)     |         |     (+)    |    (++)     |
#|   -O3      | more code size && execution time |     (---)    |         |     (+)    |    (+++)    |
#|   -Os      | code size                        |              |   (--)  |            |    (++)     |
#|  -Ofast    | O3 with none math cals           |     (---)    |         |     (+)    |    (+++)    |
#|------------|----------------------------------|--------------|---------|------------|-------------|
OPTIMIZE_OPTION = -g -O3

# App start address, that need sync with declare in linker file and interrupt vector table.
APP_START_ADDR_VAL = 0x08000000
APP_ADDR_OPTION = -DAPP_START_ADDR=$(APP_START_ADDR_VAL)

# Boot start address
BOOT_START_ADDR_VAL = 0x08000000
BOOT_ADDR_OPTION = -DBOOT_START_ADDR=$(BOOT_START_ADDR_VAL)

# Release
RELEASE_OPTION = -URELEASE

# Warning
WARNING_OPTION = -Wno-error

#### Target & build
BUILD_DIR = build
TARGET = appProgram

### Sources include
-include Sources/UsrProg/Makefile.mk
-include Sources/Core/Makefile.mk
-include Sources/Drivers/Makefile.mk
-include Sources/Middlewares/Makefile.mk

# ASM sources
ASM_SOURCES = Sources/startup_stm32f103xe.s

#### Compiler flags
# CPU
CPU = -mcpu=cortex-m3

# FPU
# NONE for Cortex-M0/M0+/M3
# float-abi

# MCU
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

COMPILER_FLAGS +=	$(MCU)				\
					-ffunction-sections	\
					-fdata-sections		\
					-fstack-usage		\
					-MD					\
					-Wall				\
					-c

GENERAL_FLAGS +=	$(RELEASE_OPTION)		\
					$(APP_ADDR_OPTION)		\
					$(BOOT_ADDR_OPTION)		\
					-DUSE_HAL_DRIVER 		\
					-DSTM32F103xE

### Synthetic compiler flags
CFLAGS +=	$(GENERAL_FLAGS)	\
			$(COMPILER_FLAGS)	\
			-std=c99

CPPFLAGS +=	$(GENERAL_FLAGS)	\
			$(COMPILER_FLAGS)	\
			-std=c++11			\
			-fno-rtti			\
			-fno-exceptions		\
			-fno-use-cxa-atexit

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"
CPPFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#### LDFLAGS
# Linker Script
LDSCRIPT = Sources/STM32F103RCTx_FLASH.ld

# Libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

#### Default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

### Build the application
# List of C objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# List of CPP objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))
# List of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(Print) CC $@
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR)
	$(Print) CXX $@
	@$(CPP) -c $(CPPFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(Print) AS $@
	@$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo CREATE $@
	@$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo CREATE $@
	@$(BIN) $< $@
	
$(BUILD_DIR):
	@echo CREATE $@ folder
	@mkdir $@

.PHONY: flash
flash: all
	$(Print) BURNING $(BUILD_DIR)/$(TARGET).bin to target
	$(PROGRAMER_PATH) -c port=SWD -w $(BUILD_DIR)/$(TARGET).bin $(APP_START_ADDR_VAL) -rst

.PHONY: help
help:
	@echo [ Help command ]
	@echo ----------------
	@echo 1- 'make' to build project.
	@echo 2- 'make clean' to remove build folder.

.PHONY: clean
clean:
	@echo REMOVE $@ folder
	@rm -fR $(BUILD_DIR)
  
#### Dependencie
-include $(wildcard $(BUILD_DIR)/*.d)
