################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ArrayTester.cpp \
../array.cpp \
../main.cpp \
../part1.cpp \
../part2.cpp 

OBJS += \
./ArrayTester.o \
./array.o \
./main.o \
./part1.o \
./part2.o 

CPP_DEPS += \
./ArrayTester.d \
./array.d \
./main.d \
./part1.d \
./part2.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


