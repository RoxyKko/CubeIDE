################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/AD9528/ad9528.c 

C_DEPS += \
./Drivers/AD9528/ad9528.d 

OBJS += \
./Drivers/AD9528/ad9528.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/AD9528/%.o Drivers/AD9528/%.su: ../Drivers/AD9528/%.c Drivers/AD9528/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-AD9528

clean-Drivers-2f-AD9528:
	-$(RM) ./Drivers/AD9528/ad9528.d ./Drivers/AD9528/ad9528.o ./Drivers/AD9528/ad9528.su

.PHONY: clean-Drivers-2f-AD9528

