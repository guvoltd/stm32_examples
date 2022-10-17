################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/User/lib/Internet/MQTT/MQTTClient.c \
../Application/User/lib/Internet/MQTT/mqtt_interface.c 

OBJS += \
./Application/User/lib/Internet/MQTT/MQTTClient.o \
./Application/User/lib/Internet/MQTT/mqtt_interface.o 

C_DEPS += \
./Application/User/lib/Internet/MQTT/MQTTClient.d \
./Application/User/lib/Internet/MQTT/mqtt_interface.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/lib/Internet/MQTT/%.o Application/User/lib/Internet/MQTT/%.su: ../Application/User/lib/Internet/MQTT/%.c Application/User/lib/Internet/MQTT/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../../Core/Inc -I../../lib/Ethernet -I../../lib/Internet/DHCP -I../../lib/Internet/DNS -I../../Drivers/STM32F4xx_HAL_Driver/Inc -I../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User-2f-lib-2f-Internet-2f-MQTT

clean-Application-2f-User-2f-lib-2f-Internet-2f-MQTT:
	-$(RM) ./Application/User/lib/Internet/MQTT/MQTTClient.d ./Application/User/lib/Internet/MQTT/MQTTClient.o ./Application/User/lib/Internet/MQTT/MQTTClient.su ./Application/User/lib/Internet/MQTT/mqtt_interface.d ./Application/User/lib/Internet/MQTT/mqtt_interface.o ./Application/User/lib/Internet/MQTT/mqtt_interface.su

.PHONY: clean-Application-2f-User-2f-lib-2f-Internet-2f-MQTT

