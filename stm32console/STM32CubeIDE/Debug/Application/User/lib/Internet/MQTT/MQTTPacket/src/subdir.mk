################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTConnectClient.c \
../Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTConnectServer.c \
../Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTDeserializePublish.c \
../Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTFormat.c \
../Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTPacket.c \
../Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTSerializePublish.c \
../Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTSubscribeClient.c \
../Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTSubscribeServer.c \
../Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.c \
../Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.c 

OBJS += \
./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTConnectClient.o \
./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTConnectServer.o \
./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTDeserializePublish.o \
./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTFormat.o \
./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTPacket.o \
./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTSerializePublish.o \
./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTSubscribeClient.o \
./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTSubscribeServer.o \
./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.o \
./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.o 

C_DEPS += \
./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTConnectClient.d \
./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTConnectServer.d \
./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTDeserializePublish.d \
./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTFormat.d \
./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTPacket.d \
./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTSerializePublish.d \
./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTSubscribeClient.d \
./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTSubscribeServer.d \
./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.d \
./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/lib/Internet/MQTT/MQTTPacket/src/%.o Application/User/lib/Internet/MQTT/MQTTPacket/src/%.su: ../Application/User/lib/Internet/MQTT/MQTTPacket/src/%.c Application/User/lib/Internet/MQTT/MQTTPacket/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../../Core/Inc -I../../lib/Ethernet -I../../lib/Internet/DHCP -I../../lib/Internet/DNS -I../../Drivers/STM32F4xx_HAL_Driver/Inc -I../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../Drivers/CMSIS/Include -I../../lib/modbus/inc -I../../lib/Internet/SNTP -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User-2f-lib-2f-Internet-2f-MQTT-2f-MQTTPacket-2f-src

clean-Application-2f-User-2f-lib-2f-Internet-2f-MQTT-2f-MQTTPacket-2f-src:
	-$(RM) ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTConnectClient.d ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTConnectClient.o ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTConnectClient.su ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTConnectServer.d ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTConnectServer.o ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTConnectServer.su ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTDeserializePublish.d ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTDeserializePublish.o ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTDeserializePublish.su ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTFormat.d ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTFormat.o ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTFormat.su ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTPacket.d ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTPacket.o ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTPacket.su ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTSerializePublish.d ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTSerializePublish.o ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTSerializePublish.su ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTSubscribeClient.d ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTSubscribeClient.o ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTSubscribeClient.su ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTSubscribeServer.d ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTSubscribeServer.o ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTSubscribeServer.su ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.d ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.o ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.su ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.d ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.o ./Application/User/lib/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.su

.PHONY: clean-Application-2f-User-2f-lib-2f-Internet-2f-MQTT-2f-MQTTPacket-2f-src

