# https://tech.davis-hansson.com/p/make/
SHELL := bash
.ONESHELL:
.SHELLFLAGS := -eu -o pipefail -c
.DELETE_ON_ERROR:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

TARGET := boot

PREFIX := arm-none-eabi-
CC := $(PREFIX)gcc
SZ := $(PREFIX)size
OD := $(PREFIX)objdump
OC := $(PREFIX)objcopy

OUT_DIR := out
SRC_DIR := src
INC_DIR := inc
LIB_DIR := ../lib

BSP_DIR := $(LIB_DIR)/STM32CubeG4/Drivers/BSP/STM32G4xx_Nucleo
CMSIS_DIR := $(LIB_DIR)/STM32CubeG4/Drivers/CMSIS
HAL_DIR := $(LIB_DIR)/STM32CubeG4/Drivers/STM32G4xx_HAL_Driver
MCUBOOT_DIR := $(LIB_DIR)/mcuboot

LD_SRCS := $(SRC_DIR)/gcc/STM32G431RBTX_FLASH.ld

AS_SRCS := $(SRC_DIR)/gcc/startup_stm32g431rbtx.s

MCUBOOT_SRCS := \
	$(MCUBOOT_DIR)/boot/bootutil/src/boot_record.c \
	$(MCUBOOT_DIR)/boot/bootutil/src/bootutil_misc.c \
	$(MCUBOOT_DIR)/boot/bootutil/src/bootutil_public.c \
	$(MCUBOOT_DIR)/boot/bootutil/src/caps.c \
	$(MCUBOOT_DIR)/boot/bootutil/src/encrypted.c \
	$(MCUBOOT_DIR)/boot/bootutil/src/fault_injection_hardening_delay_rng_mbedtls.c \
	$(MCUBOOT_DIR)/boot/bootutil/src/fault_injection_hardening.c \
	$(MCUBOOT_DIR)/boot/bootutil/src/image_ecdsa.c \
	$(MCUBOOT_DIR)/boot/bootutil/src/image_ed25519.c \
	$(MCUBOOT_DIR)/boot/bootutil/src/image_rsa.c \
	$(MCUBOOT_DIR)/boot/bootutil/src/image_validate.c \
	$(MCUBOOT_DIR)/boot/bootutil/src/loader.c \
	$(MCUBOOT_DIR)/boot/bootutil/src/swap_misc.c \
	$(MCUBOOT_DIR)/boot/bootutil/src/swap_move.c \
	$(MCUBOOT_DIR)/boot/bootutil/src/swap_scratch.c \
	$(MCUBOOT_DIR)/boot/bootutil/src/tlv.c \

TINYCRYPT_SRCS := \

STM32_SRCS := \
	$(BSP_DIR)/stm32g4xx_nucleo.c \
	$(HAL_DIR)/Src/stm32g4xx_hal.c \
	$(HAL_DIR)/Src/stm32g4xx_hal_cortex.c \
	$(HAL_DIR)/Src/stm32g4xx_hal_exti.c \
	$(HAL_DIR)/Src/stm32g4xx_hal_gpio.c \
	$(HAL_DIR)/Src/stm32g4xx_hal_pwr.c \
	$(HAL_DIR)/Src/stm32g4xx_hal_pwr_ex.c \
	$(HAL_DIR)/Src/stm32g4xx_hal_rcc.c \
	$(HAL_DIR)/Src/stm32g4xx_hal_rcc_ex.c \
	$(HAL_DIR)/Src/stm32g4xx_hal_uart.c \
	$(HAL_DIR)/Src/stm32g4xx_hal_uart_ex.c \
	$(SRC_DIR)/gcc/syscalls.c \
	$(SRC_DIR)/gcc/sysmem.c \
	$(SRC_DIR)/gcc/system_stm32g4xx.c \
	$(SRC_DIR)/stm32g4xx_hal_msp.c \
	$(SRC_DIR)/stm32g4xx_it.c \

CC_SRCS := \
	$(MCUBOOT_SRCS) \
	$(TINYCRYPT_SRCS) \
	$(STM32_SRCS) \
	$(SRC_DIR)/main.c \

INCS := \
	"-I$(BSP_DIR)" \
	"-I$(CMSIS_DIR)/Core/Include" \
	"-I$(CMSIS_DIR)/Device/ST/STM32G4xx/Include" \
	"-I$(HAL_DIR)/Inc" \
	"-I$(HAL_DIR)/Inc/Legacy" \
	"-I$(INC_DIR)" \
	"-I$(MCUBOOT_DIR)/boot/bootutil/include" \
	"-I$(MCUBOOT_DIR)/ext/tinycrypt/lib/include" \

DEFS := \
	-DNDEBUG \
	-DSTM32G431xx \
	-DUSE_FULL_LL_DRIVER \
	-DUSE_HAL_DRIVER \

STD_FLAGS := -std=gnu11
STD_FLAGS += -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
STD_FLAGS += -ffunction-sections -fdata-sections -fstack-usage
WRN_FLAGS := -Wall -Wextra
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
