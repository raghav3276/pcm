################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/pcm.c \
../src/pcm_apps.c \
../src/pcm_mem.c \
../src/pcm_threads.c 

OBJS += \
./src/pcm.o \
./src/pcm_apps.o \
./src/pcm_mem.o \
./src/pcm_threads.o 

C_DEPS += \
./src/pcm.d \
./src/pcm_apps.d \
./src/pcm_mem.d \
./src/pcm_threads.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


