################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /home/maximilian/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./lcg.obj" \
"./main_elmar.obj" \
"./MSP430FR5xx_6xx/adc12_b.obj" \
"./MSP430FR5xx_6xx/aes256.obj" \
"./MSP430FR5xx_6xx/comp_e.obj" \
"./MSP430FR5xx_6xx/crc.obj" \
"./MSP430FR5xx_6xx/crc32.obj" \
"./MSP430FR5xx_6xx/cs.obj" \
"./MSP430FR5xx_6xx/dma.obj" \
"./MSP430FR5xx_6xx/esi.obj" \
"./MSP430FR5xx_6xx/eusci_a_spi.obj" \
"./MSP430FR5xx_6xx/eusci_a_uart.obj" \
"./MSP430FR5xx_6xx/eusci_b_i2c.obj" \
"./MSP430FR5xx_6xx/eusci_b_spi.obj" \
"./MSP430FR5xx_6xx/framctl.obj" \
"./MSP430FR5xx_6xx/framctl_a.obj" \
"./MSP430FR5xx_6xx/gpio.obj" \
"./MSP430FR5xx_6xx/lcd_c.obj" \
"./MSP430FR5xx_6xx/mpu.obj" \
"./MSP430FR5xx_6xx/mpy32.obj" \
"./MSP430FR5xx_6xx/pmm.obj" \
"./MSP430FR5xx_6xx/ram.obj" \
"./MSP430FR5xx_6xx/ref_a.obj" \
"./MSP430FR5xx_6xx/rtc_b.obj" \
"./MSP430FR5xx_6xx/rtc_c.obj" \
"./MSP430FR5xx_6xx/sfr.obj" \
"./MSP430FR5xx_6xx/sysctl.obj" \
"./MSP430FR5xx_6xx/timer_a.obj" \
"./MSP430FR5xx_6xx/timer_b.obj" \
"./MSP430FR5xx_6xx/tlv.obj" \
"./MSP430FR5xx_6xx/wdt_a.obj" \
"../lnk_msp430fr5969.cmd" \
$(GEN_CMDS__FLAG) \
-llibmpu_init.a \
-llibmath.a \
-llibc.a \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include MSP430FR5xx_6xx/subdir_vars.mk
-include subdir_rules.mk
-include MSP430FR5xx_6xx/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
msp430-2fsk.out \

EXE_OUTPUTS__QUOTED += \
"msp430-2fsk.out" \

BIN_OUTPUTS += \
msp430-2fsk.hex \

BIN_OUTPUTS__QUOTED += \
"msp430-2fsk.hex" \


# All Target
all: msp430-2fsk.out secondary-outputs

# Tool invocations
msp430-2fsk.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: MSP430 Linker'
	"/home/maximilian/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --advice:power=all --advice:hw_config=all --define=__MSP430FR5969__ --define=_MPU_ENABLE -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 -z -m"msp430-2fsk.map" --heap_size=160 --stack_size=160 --cinit_hold_wdt=on -i"/home/maximilian/ti/ccsv7/ccs_base/msp430/include" -i"/home/maximilian/ti/ccsv7/ccs_base/msp430/lib/5xx_6xx_FRxx" -i"/home/maximilian/ti/ccsv7/ccs_base/msp430/lib/FR59xx" -i"/home/maximilian/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/lib" -i"/home/maximilian/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/include" --priority --reread_libs --define=_MPU_ENABLE --diag_wrap=off --display_error_number --warn_sections --xml_link_info="msp430-2fsk_linkInfo.xml" --use_hw_mpy=F5 --rom_model -o "msp430-2fsk.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

msp430-2fsk.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: MSP430 Hex Utility'
	"/home/maximilian/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/bin/hex430" --exclude=.eeprom --memwidth=8 --romwidth=8 --intel -o "msp430-2fsk.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "lcg.obj" "main_elmar.obj" "MSP430FR5xx_6xx/adc12_b.obj" "MSP430FR5xx_6xx/aes256.obj" "MSP430FR5xx_6xx/comp_e.obj" "MSP430FR5xx_6xx/crc.obj" "MSP430FR5xx_6xx/crc32.obj" "MSP430FR5xx_6xx/cs.obj" "MSP430FR5xx_6xx/dma.obj" "MSP430FR5xx_6xx/esi.obj" "MSP430FR5xx_6xx/eusci_a_spi.obj" "MSP430FR5xx_6xx/eusci_a_uart.obj" "MSP430FR5xx_6xx/eusci_b_i2c.obj" "MSP430FR5xx_6xx/eusci_b_spi.obj" "MSP430FR5xx_6xx/framctl.obj" "MSP430FR5xx_6xx/framctl_a.obj" "MSP430FR5xx_6xx/gpio.obj" "MSP430FR5xx_6xx/lcd_c.obj" "MSP430FR5xx_6xx/mpu.obj" "MSP430FR5xx_6xx/mpy32.obj" "MSP430FR5xx_6xx/pmm.obj" "MSP430FR5xx_6xx/ram.obj" "MSP430FR5xx_6xx/ref_a.obj" "MSP430FR5xx_6xx/rtc_b.obj" "MSP430FR5xx_6xx/rtc_c.obj" "MSP430FR5xx_6xx/sfr.obj" "MSP430FR5xx_6xx/sysctl.obj" "MSP430FR5xx_6xx/timer_a.obj" "MSP430FR5xx_6xx/timer_b.obj" "MSP430FR5xx_6xx/tlv.obj" "MSP430FR5xx_6xx/wdt_a.obj" 
	-$(RM) "lcg.d" "main_elmar.d" "MSP430FR5xx_6xx/adc12_b.d" "MSP430FR5xx_6xx/aes256.d" "MSP430FR5xx_6xx/comp_e.d" "MSP430FR5xx_6xx/crc.d" "MSP430FR5xx_6xx/crc32.d" "MSP430FR5xx_6xx/cs.d" "MSP430FR5xx_6xx/dma.d" "MSP430FR5xx_6xx/esi.d" "MSP430FR5xx_6xx/eusci_a_spi.d" "MSP430FR5xx_6xx/eusci_a_uart.d" "MSP430FR5xx_6xx/eusci_b_i2c.d" "MSP430FR5xx_6xx/eusci_b_spi.d" "MSP430FR5xx_6xx/framctl.d" "MSP430FR5xx_6xx/framctl_a.d" "MSP430FR5xx_6xx/gpio.d" "MSP430FR5xx_6xx/lcd_c.d" "MSP430FR5xx_6xx/mpu.d" "MSP430FR5xx_6xx/mpy32.d" "MSP430FR5xx_6xx/pmm.d" "MSP430FR5xx_6xx/ram.d" "MSP430FR5xx_6xx/ref_a.d" "MSP430FR5xx_6xx/rtc_b.d" "MSP430FR5xx_6xx/rtc_c.d" "MSP430FR5xx_6xx/sfr.d" "MSP430FR5xx_6xx/sysctl.d" "MSP430FR5xx_6xx/timer_a.d" "MSP430FR5xx_6xx/timer_b.d" "MSP430FR5xx_6xx/tlv.d" "MSP430FR5xx_6xx/wdt_a.d" 
	-@echo 'Finished clean'
	-@echo ' '

secondary-outputs: $(BIN_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

