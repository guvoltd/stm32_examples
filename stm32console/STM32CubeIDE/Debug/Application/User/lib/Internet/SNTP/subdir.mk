################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/User/lib/Internet/SNTP/sntp.c 

OBJS += \
./Application/User/lib/Internet/SNTP/sntp.o 

C_DEPS += \
./Application/User/lib/Internet/SNTP/sntp.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/lib/Internet/SNTP/%.o Application/User/lib/Internet/SNTP/%.su: ../Application/User/lib/Internet/SNTP/%.c Application/User/lib/Internet/SNTP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../../Core/Inc -I../../lib/Ethernet -I../../lib/Internet/DHCP -I../../lib/Internet/DNS -I../../Drivers/STM32F4xx_HAL_Driver/Inc -I../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User-2f-lib-2f-Internet-2f-SNTP

clean-Application-2f-User-2f-lib-2f-Internet-2f-SNTP:
	-$(RM) ./Application/User/lib/Internet/SNTP/sntp.d ./Application/User/lib/Internet/SNTP/sntp.o ./Application/User/lib/Internet/SNTP/sntp.su

.PHONY: clean-Application-2f-User-2f-lib-2f-Internet-2f-SNTP
