################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/ad7606/ad7606.c 

OBJS += \
./Drivers/ad7606/ad7606.o 

C_DEPS += \
./Drivers/ad7606/ad7606.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/ad7606/%.o Drivers/ad7606/%.su: ../Drivers/ad7606/%.c Drivers/ad7606/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx '-D__FPU_PRESENT = 1U' -D_TARGET_FPU_VFP -DARM_MATH_CM4 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/CubeIDE2/AD7606_Demo/Drivers/DSP" -I"D:/CubeIDE2/AD7606_Demo/Drivers/ad7606" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-ad7606

clean-Drivers-2f-ad7606:
	-$(RM) ./Drivers/ad7606/ad7606.d ./Drivers/ad7606/ad7606.o ./Drivers/ad7606/ad7606.su

.PHONY: clean-Drivers-2f-ad7606

