################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/iLLD/TC37A/Tricore/Ccu6/TPwm/IfxCcu6_TPwm.c" 

COMPILED_SRCS += \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/iLLD/TC37A/Tricore/Ccu6/TPwm/IfxCcu6_TPwm.src" 

C_DEPS += \
"./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/iLLD/TC37A/Tricore/Ccu6/TPwm/IfxCcu6_TPwm.d" 

OBJS += \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/iLLD/TC37A/Tricore/Ccu6/TPwm/IfxCcu6_TPwm.o" 


# Each subdirectory must supply rules for building sources it contributes
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/iLLD/TC37A/Tricore/Ccu6/TPwm/IfxCcu6_TPwm.src":"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/iLLD/TC37A/Tricore/Ccu6/TPwm/IfxCcu6_TPwm.c" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/iLLD/TC37A/Tricore/Ccu6/TPwm/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/iLLD/TC37A/Tricore/Ccu6/TPwm/IfxCcu6_TPwm.o":"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/iLLD/TC37A/Tricore/Ccu6/TPwm/IfxCcu6_TPwm.src" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/iLLD/TC37A/Tricore/Ccu6/TPwm/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-Seekfree_TC377_Opensource_Library-2f-libraries-2f-infineon_libraries-2f-iLLD-2f-TC37A-2f-Tricore-2f-Ccu6-2f-TPwm

clean-Seekfree_TC377_Opensource_Library-2f-libraries-2f-infineon_libraries-2f-iLLD-2f-TC37A-2f-Tricore-2f-Ccu6-2f-TPwm:
	-$(RM) ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/iLLD/TC37A/Tricore/Ccu6/TPwm/IfxCcu6_TPwm.d ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/iLLD/TC37A/Tricore/Ccu6/TPwm/IfxCcu6_TPwm.o ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/iLLD/TC37A/Tricore/Ccu6/TPwm/IfxCcu6_TPwm.src

.PHONY: clean-Seekfree_TC377_Opensource_Library-2f-libraries-2f-infineon_libraries-2f-iLLD-2f-TC37A-2f-Tricore-2f-Ccu6-2f-TPwm

