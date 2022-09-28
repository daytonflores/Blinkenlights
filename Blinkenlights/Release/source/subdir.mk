################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/led.c \
../source/main.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/touch.c 

C_DEPS += \
./source/led.d \
./source/main.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/touch.d 

OBJS += \
./source/led.o \
./source/main.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/touch.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DNDEBUG -D__REDLIB__ -DSDK_DEBUGCONSOLE=1 -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\Blinkenlights\board" -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\Blinkenlights\source" -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\Blinkenlights" -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\Blinkenlights\drivers" -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\Blinkenlights\CMSIS" -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\Blinkenlights\utilities" -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\Blinkenlights\startup" -O0 -fno-common -g -Wall -Werror -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/led.d ./source/led.o ./source/main.d ./source/main.o ./source/mtb.d ./source/mtb.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/touch.d ./source/touch.o

.PHONY: clean-source

