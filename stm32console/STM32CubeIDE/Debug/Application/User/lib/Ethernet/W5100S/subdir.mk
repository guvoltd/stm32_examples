################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/User/lib/Ethernet/W5100S/w5100s.c 

OBJS += \
./Application/User/lib/Ethernet/W5100S/w5100s.o 

C_DEPS += \
./Application/User/lib/Ethernet/W5100S/w5100s.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/lib/Ethernet/W5100S/%.o Application/User/lib/Ethernet/W5100S/%.su: ../Application/User/lib/Ethernet/W5100S/%.c Application/User/lib/Ethernet/W5100S/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../../Core/Inc -I../../lib/Ethernet -I../../lib/Ethernet/W5500 -I../../lib/Internet/DHCP -I../../lib/Internet/DNS -I../../Drivers/STM32F4xx_HAL_Driver/Inc -I../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../Drivers/CMSIS/Include -I../../lib/modbus/inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User-2f-lib-2f-Ethernet-2f-W5100S

clean-Application-2f-User-2f-lib-2f-Ethernet-2f-W5100S:
	-$(RM) ./Application/User/lib/Ethernet/W5100S/w5100s.d ./Application/User/lib/Ethernet/W5100S/w5100s.o ./Application/User/lib/Ethernet/W5100S/w5100s.su

.PHONY: clean-Application-2f-User-2f-lib-2f-Ethernet-2f-W5100S

