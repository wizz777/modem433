################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/433Modem.c \
../Src/Alligator.c \
../Src/CC1101.c \
../Src/Fun_Menu.c \
../Src/Menu.c \
../Src/Princeton.c \
../Src/PrincetonPT2240.c \
../Src/StarLine.c \
../Src/came.c \
../Src/dwt_stm32_delay.c \
../Src/flash.c \
../Src/fonts.c \
../Src/keelog.c \
../Src/keelog_func.c \
../Src/main.c \
../Src/nice.c \
../Src/ssd1306.c \
../Src/stm32f1xx_hal_msp.c \
../Src/stm32f1xx_it.c \
../Src/syscalls.c \
../Src/system_stm32f1xx.c 

OBJS += \
./Src/433Modem.o \
./Src/Alligator.o \
./Src/CC1101.o \
./Src/Fun_Menu.o \
./Src/Menu.o \
./Src/Princeton.o \
./Src/PrincetonPT2240.o \
./Src/StarLine.o \
./Src/came.o \
./Src/dwt_stm32_delay.o \
./Src/flash.o \
./Src/fonts.o \
./Src/keelog.o \
./Src/keelog_func.o \
./Src/main.o \
./Src/nice.o \
./Src/ssd1306.o \
./Src/stm32f1xx_hal_msp.o \
./Src/stm32f1xx_it.o \
./Src/syscalls.o \
./Src/system_stm32f1xx.o 

C_DEPS += \
./Src/433Modem.d \
./Src/Alligator.d \
./Src/CC1101.d \
./Src/Fun_Menu.d \
./Src/Menu.d \
./Src/Princeton.d \
./Src/PrincetonPT2240.d \
./Src/StarLine.d \
./Src/came.d \
./Src/dwt_stm32_delay.d \
./Src/flash.d \
./Src/fonts.d \
./Src/keelog.d \
./Src/keelog_func.d \
./Src/main.d \
./Src/nice.d \
./Src/ssd1306.d \
./Src/stm32f1xx_hal_msp.d \
./Src/stm32f1xx_it.d \
./Src/syscalls.d \
./Src/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F103xB -I"/Users/wizz/Downloads/433_modem/Inc" -I"/Users/wizz/Downloads/433_modem/Drivers/STM32F1xx_HAL_Driver/Inc" -I"/Users/wizz/Downloads/433_modem/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"/Users/wizz/Downloads/433_modem/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"/Users/wizz/Downloads/433_modem/Drivers/CMSIS/Include"  -O2 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


