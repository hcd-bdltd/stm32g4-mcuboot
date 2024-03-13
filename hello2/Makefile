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

OUT_DIR := ./out
SRC_DIR := ./src
INC_DIR := ./inc
LIB_DIR := ../lib
EXT_DIR := ../ext

LD_SRCS :=

AS_SRCS :=

CC_SRCS :=

INCS :=

DEFS :=

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

AS_OBJS := $(addprefix $(OUT_DIR)/, $(patsubst %.S,%.o, $(AS_SRCS)))
CC_OBJS := $(addprefix $(OUT_DIR)/, $(patsubst %.c,%.o, $(CC_SRCS)))

.PHONY: all
all: $(OUT_DIR)/$(TARGET).elf

$(OUT_DIR)/$(TARGET).elf: $(AS_OBJS) $(CC_OBJS)
	@echo "LD [$(TARGET).elf]"
	@$(CC) $(LD_FLAGS) $^ -o $@
	@$(SZ) $@
	@$(OC) -O binary $@ $(OUT_DIR)/$(TARGET).bin
	@echo "DONE"

$(AS_OBJS): $(OUT_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	@echo "AS [$<]"
	@$(CC) $(AS_FLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c $< -o $@

$(CC_OBJS): $(OUT_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "CC [$<]"
	@$(CC) $(CC_FLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c $< -o $@

.PHONY: clean
clean:
	@echo "RM [$(OUT_DIR)]"
	@rm -rf $(OUT_DIR)

-include $(wildcard $(OUT_DIR)/*.d)
