################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_DPipe.c" \
"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_Pos.c" \
"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_PwmHl.c" \
"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_Timer.c" 

COMPILED_SRCS += \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_DPipe.src" \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_Pos.src" \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_PwmHl.src" \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_Timer.src" 

C_DEPS += \
"./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_DPipe.d" \
"./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_Pos.d" \
"./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_PwmHl.d" \
"./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_Timer.d" 

OBJS += \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_DPipe.o" \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_Pos.o" \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_PwmHl.o" \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_Timer.o" 


# Each subdirectory must supply rules for building sources it contributes
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_DPipe.src":"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_DPipe.c" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_DPipe.o":"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_DPipe.src" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_Pos.src":"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_Pos.c" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_Pos.o":"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_Pos.src" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_PwmHl.src":"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_PwmHl.c" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_PwmHl.o":"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_PwmHl.src" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_Timer.src":"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_Timer.c" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_Timer.o":"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_Timer.src" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-Seekfree_TC377_Opensource_Library-2f-libraries-2f-infineon_libraries-2f-Service-2f-CpuGeneric-2f-StdIf

clean-Seekfree_TC377_Opensource_Library-2f-libraries-2f-infineon_libraries-2f-Service-2f-CpuGeneric-2f-StdIf:
	-$(RM) ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_DPipe.d ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_DPipe.o ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_DPipe.src ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_Pos.d ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_Pos.o ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_Pos.src ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_PwmHl.d ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_PwmHl.o ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_PwmHl.src ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_Timer.d ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_Timer.o ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Service/CpuGeneric/StdIf/IfxStdIf_Timer.src

.PHONY: clean-Seekfree_TC377_Opensource_Library-2f-libraries-2f-infineon_libraries-2f-Service-2f-CpuGeneric-2f-StdIf

