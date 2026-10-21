################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Infra.c" \
"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc0.c" \
"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc1.c" \
"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc2.c" 

COMPILED_SRCS += \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Infra.src" \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc0.src" \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc1.src" \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc2.src" 

C_DEPS += \
"./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Infra.d" \
"./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc0.d" \
"./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc1.d" \
"./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc2.d" 

OBJS += \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Infra.o" \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc0.o" \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc1.o" \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc2.o" 


# Each subdirectory must supply rules for building sources it contributes
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Infra.src":"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Infra.c" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Infra.o":"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Infra.src" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc0.src":"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc0.c" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc0.o":"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc0.src" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc1.src":"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc1.c" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc1.o":"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc1.src" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc2.src":"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc2.c" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc2.o":"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc2.src" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-Seekfree_TC377_Opensource_Library-2f-libraries-2f-infineon_libraries-2f-Infra-2f-Ssw-2f-TC37A-2f-Tricore

clean-Seekfree_TC377_Opensource_Library-2f-libraries-2f-infineon_libraries-2f-Infra-2f-Ssw-2f-TC37A-2f-Tricore:
	-$(RM) ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Infra.d ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Infra.o ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Infra.src ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc0.d ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc0.o ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc0.src ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc1.d ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc1.o ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc1.src ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc2.d ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc2.o ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Ssw/TC37A/Tricore/Ifx_Ssw_Tc2.src

.PHONY: clean-Seekfree_TC377_Opensource_Library-2f-libraries-2f-infineon_libraries-2f-Infra-2f-Ssw-2f-TC37A-2f-Tricore

