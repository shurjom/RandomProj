################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Introductory\ Stuff.cpp 

OBJS += \
./src/Introductory\ Stuff.o 

CPP_DEPS += \
./src/Introductory\ Stuff.d 


# Each subdirectory must supply rules for building sources it contributes
src/Introductory\ Stuff.o: ../src/Introductory\ Stuff.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/Introductory Stuff.d" -MT"src/Introductory\ Stuff.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


