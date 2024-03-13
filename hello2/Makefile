# https://tech.davis-hansson.com/p/make/
SHELL := bash
.ONESHELL:
.SHELLFLAGS := -eu -o pipefail -c
.DELETE_ON_ERROR:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

TARGET := hello2

PREFIX := arm-none-eabi-
CC := $(PREFIX)gcc
SZ := $(PREFIX)size
OD := $(PREFIX)objdump
OC := $(PREFIX)objcopy

OUT_DIR := out
SRC_DIR := src
INC_DIR := inc
LIB_DIR := ../lib
EXT_DIR := ../ext

BSP_DIR := $(EXT_DIR)/STM32CubeG4/Drivers/BSP/STM32G4xx_Nucleo
CMSIS_DIR := $(EXT_DIR)/STM32CubeG4/Drivers/CMSIS
HAL_DIR := $(EXT_DIR)/STM32CubeG4/Drivers/STM32G4xx_HAL_Driver

LD_SRCS := $(SRC_DIR)/gcc/STM32G431RBTX_FLASH.ld

AS_SRCS := $(SRC_DIR)/gcc/startup_stm32g431rbtx.s

CC_SRCS := \
	$(HAL_DIR)/Src/stm32g4xx_hal.c \
	$(HAL_DIR)/Src/stm32g4xx_hal_cortex.c \
	$(HAL_DIR)/Src/stm32g4xx_hal_pwr.c \
	$(HAL_DIR)/Src/stm32g4xx_hal_pwr_ex.c \
	$(HAL_DIR)/Src/stm32g4xx_hal_rcc.c \
	$(HAL_DIR)/Src/stm32g4xx_hal_rcc_ex.c \
	$(SRC_DIR)/gcc/syscalls.c \
	$(SRC_DIR)/gcc/sysmem.c \
	$(SRC_DIR)/gcc/system_stm32g4xx.c \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/stm32g4xx_hal_msp.c \
	$(SRC_DIR)/stm32g4xx_it.c \

INCS := \
	"-I$(BSP_DIR)" \
	"-I$(CMSIS_DIR)/Core/Include" \
	"-I$(CMSIS_DIR)/Device/ST/STM32G4xx/Include" \
	"-I$(HAL_DIR)/Inc" \
	"-I$(HAL_DIR)/Inc/Legacy" \
	"-I$(INC_DIR)" \

DEFS := \
	-DNDEBUG \
	-DSTM32G431xx \
	-DUSE_FULL_LL_DRIVER \
	-DUSE_HAL_DRIVER \

STD_FLAGS := -std=gnu11
STD_FLAGS += -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
STD_FLAGS += -ffunction-sections -fdata-sections -fstack-usage
WRN_FLAGS := -Wall -Wextra -Werror
OPT_FLAGS := -Og -g3

AS_FLAGS := $(STD_FLAGS) $(OPT_FLAGS)
AS_FLAGS += -x assembler-with-cpp

CC_FLAGS := $(STD_FLAGS) $(WRN_FLAGS) $(OPT_FLAGS)
CC_FLAGS += -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))"
CC_FLAGS += $(INCS) $(DEFS)

LD_FLAGS := $(STD_FLAGS) $(OPT_FLAGS)
LD_FLAGS += -T$(LD_SRCS) -Wl,-Map=$(OUT_DIR)/$(TARGET).map,--cref
LD_FLAGS += --specs=nano.specs --specs=nosys.specs
LD_FLAGS += -Wl,--gc-sections -static
LD_FLAGS += -Wl,--start-group -lc -lm -Wl,--end-group
LD_FLAGS += -Wl,--no-warn-rwx-segment

AS_OBJS := $(patsubst %.s,%.o,$(AS_SRCS))
CC_OBJS := $(patsubst %.c,%.o,$(CC_SRCS))

.PHONY: all
all: $(OUT_DIR)/$(TARGET).elf

$(OUT_DIR)/$(TARGET).elf: $(AS_OBJS) $(CC_OBJS)
	@echo "LD [$(TARGET).elf]"
	@$(CC) $(LD_FLAGS) $(patsubst %.o,$(OUT_DIR)/%.o,$(^F)) -o $@
	@$(SZ) -B -d $@
	@$(OC) -O binary $@ $(OUT_DIR)/$(TARGET).bin
	@echo "DONE"

$(AS_OBJS): %.o: %.s | $(OUT_DIR)
	@echo "AS [$(notdir $<)]"
	@$(CC) $(AS_FLAGS) -MMD -MP -MF"$(OUT_DIR)/$(@F:%.o=%.d)" -MT"$@" -c $< -o $(OUT_DIR)/$(@F)

$(CC_OBJS): %.o: %.c | $(OUT_DIR)
	@echo "CC [$(notdir $<)]"
	@$(CC) $(CC_FLAGS) -MMD -MP -MF"$(OUT_DIR)/$(@F:%.o=%.d)" -MT"$@" -c $< -o $(OUT_DIR)/$(@F)

$(OUT_DIR):
	@echo "MD [$(OUT_DIR)]"
	@mkdir -p $(OUT_DIR)

.PHONY: clean
clean:
	@echo "RM [$(OUT_DIR)]"
	@rm -rf $(OUT_DIR)

-include $(wildcard $(OUT_DIR)/*.d)
