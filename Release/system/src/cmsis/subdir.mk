################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/cmsis/system_stm32f0xx.c 

S_UPPER_SRCS += \
../system/src/cmsis/startup_stm32f030x6.S 

OBJS += \
./system/src/cmsis/startup_stm32f030x6.o \
./system/src/cmsis/system_stm32f0xx.o 

C_DEPS += \
./system/src/cmsis/system_stm32f0xx.d 

S_UPPER_DEPS += \
./system/src/cmsis/startup_stm32f030x6.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/cmsis/%.o: ../system/src/cmsis/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fsingle-precision-constant -x assembler-with-cpp -DSTM32F030x6 -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/include" -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/system/include" -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/system/include/cmsis" -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/system/include/stm32f0xx" -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/system/include/cmsis/device" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/src/cmsis/%.o: ../system/src/cmsis/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fsingle-precision-constant -DSTM32F030x6 -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/include" -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/system/include/stm32f0xx" -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/system/include/cmsis/device" -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/system/include/cmsis" -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/system/include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


