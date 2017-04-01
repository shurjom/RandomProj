################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CXX_SRCS += \
../bzutil.cxx \
../chunk.cxx \
../sentence.cxx \
../token.cxx \
../ttm_bzfile.cxx \
../ttm_sqlite3.cxx \
../ttm_sqlite3_row.cxx \
../ttm_sqlite3_stmt.cxx 

OBJS += \
./bzutil.o \
./chunk.o \
./sentence.o \
./token.o \
./ttm_bzfile.o \
./ttm_sqlite3.o \
./ttm_sqlite3_row.o \
./ttm_sqlite3_stmt.o 

CXX_DEPS += \
./bzutil.d \
./chunk.d \
./sentence.d \
./token.d \
./ttm_bzfile.d \
./ttm_sqlite3.d \
./ttm_sqlite3_row.d \
./ttm_sqlite3_stmt.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -D_POSIX_C_SOURCE=200809L -U__STRICT_ANSI__ -I"C:\Users\Kristofer\Downloads\sqlite-preprocessed-3081002" -I"C:\Users\ohnob\Downloads\sqlite-preprocessed-3081002" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


