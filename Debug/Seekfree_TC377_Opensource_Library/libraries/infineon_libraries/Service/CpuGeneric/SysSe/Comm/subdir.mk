################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Console.c" \
"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Shell.c" 

COMPILED_SRCS += \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Console.src" \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Shell.src" 

C_DEPS += \
"./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Console.d" \
"./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Shell.d" 

OBJS += \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Console.o" \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Shell.o" 


# Each subdirectory must supply rules for building sources it contributes
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Console.src":"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Console.c" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Console.o":"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Console.src" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Shell.src":"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Shell.c" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Shell.o":"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Shell.src" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-Seekfree_TC377_Opensource_Library-2f-libraries-2f-infineon_libraries-2f-Service-2f-CpuGeneric-2f-SysSe-2f-Comm

clean-Seekfree_TC377_Opensource_Library-2f-libraries-2f-infineon_libraries-2f-Service-2f-CpuGeneric-2f-SysSe-2f-Comm:
	-$(RM) ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Console.d ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Console.o ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Console.src ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Shell.d ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Shell.o ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/SysSe/Comm/Ifx_Shell.src

.PHONY: clean-Seekfree_TC377_Opensource_Library-2f-libraries-2f-infineon_libraries-2f-Service-2f-CpuGeneric-2f-SysSe-2f-Comm

