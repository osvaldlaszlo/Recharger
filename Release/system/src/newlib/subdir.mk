################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../system/src/newlib/_cxx.cpp 

C_SRCS += \
../system/src/newlib/_exit.c \
../system/src/newlib/_sbrk.c \
../system/src/newlib/_startup.c \
../system/src/newlib/_syscalls.c \
../system/src/newlib/assert.c 

OBJS += \
./system/src/newlib/_cxx.o \
./system/src/newlib/_exit.o \
./system/src/newlib/_sbrk.o \
./system/src/newlib/_startup.o \
./system/src/newlib/_syscalls.o \
./system/src/newlib/assert.o 

C_DEPS += \
./system/src/newlib/_exit.d \
./system/src/newlib/_sbrk.d \
./system/src/newlib/_startup.d \
./system/src/newlib/_syscalls.d \
./system/src/newlib/assert.d 

CPP_DEPS += \
./system/src/newlib/_cxx.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/newlib/%.o: ../system/src/newlib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fsingle-precision-constant -DSTM32F030x6 -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/include" -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/system/include" -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/system/include/cmsis" -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/system/include/stm32f0xx" -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/system/include/cmsis/device" -std=gnu++11 -fabi-version=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/src/newlib/%.o: ../system/src/newlib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fsingle-precision-constant -DSTM32F030x6 -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/include" -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/system/include/stm32f0xx" -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/system/include/cmsis/device" -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/system/include/cmsis" -I"/media/c/Data/STM32Toolchain/projects/Recharger-Eclipse/system/include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


