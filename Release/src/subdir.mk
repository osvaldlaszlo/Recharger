################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/recharger.c \
../src/stm32f0xx_hal_msp.c \
../src/stm32f0xx_it.c 

OBJS += \
./src/main.o \
./src/recharger.o \
./src/stm32f0xx_hal_msp.o \
./src/stm32f0xx_it.o 

C_DEPS += \
./src/main.d \
./src/recharger.d \
./src/stm32f0xx_hal_msp.d \
./src/stm32f0xx_it.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fsingle-precision-constant -DSTM32F030x6 -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/include" -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/system/include/stm32f0xx" -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/system/include/cmsis/device" -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/system/include/cmsis" -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/system/include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


