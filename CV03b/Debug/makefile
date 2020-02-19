################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F0xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
CV03b.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
CV03b.list \

OBJCOPY_BIN += \
CV03b.bin \


# All Target
all: CV03b.elf secondary-outputs

# Tool invocations
CV03b.elf: $(OBJS) $(USER_OBJS) P:\MMIA\mmia\CV03b\STM32F030R8TX_FLASH.ld
	arm-none-eabi-gcc -o "CV03b.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0 -T"P:\MMIA\mmia\CV03b\STM32F030R8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="CV03b.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

CV03b.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "CV03b.list"
	@echo 'Finished building: $@'
	@echo ' '

CV03b.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "CV03b.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
