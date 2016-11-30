################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/diag/Trace.c \
../system/src/diag/trace_impl.c 

OBJS += \
./system/src/diag/Trace.o \
./system/src/diag/trace_impl.o 

C_DEPS += \
./system/src/diag/Trace.d \
./system/src/diag/trace_impl.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/diag/%.o: ../system/src/diag/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fsingle-precision-constant -DSTM32F030x6 -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/include" -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/system/include/stm32f0xx" -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/system/include/cmsis/device" -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/system/include/cmsis" -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/system/include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


