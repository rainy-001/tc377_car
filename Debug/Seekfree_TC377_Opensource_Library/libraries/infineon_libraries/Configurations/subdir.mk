################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/Ifx_Cfg_Ssw.c" \
"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/Ifx_Cfg_SswBmhd.c" 

COMPILED_SRCS += \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/Ifx_Cfg_Ssw.src" \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/Ifx_Cfg_SswBmhd.src" 

C_DEPS += \
"./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/Ifx_Cfg_Ssw.d" \
"./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/Ifx_Cfg_SswBmhd.d" 

OBJS += \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/Ifx_Cfg_Ssw.o" \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/Ifx_Cfg_SswBmhd.o" 


# Each subdirectory must supply rules for building sources it contributes
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/Ifx_Cfg_Ssw.src":"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/Ifx_Cfg_Ssw.c" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/Ifx_Cfg_Ssw.o":"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/Ifx_Cfg_Ssw.src" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/Ifx_Cfg_SswBmhd.src":"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/Ifx_Cfg_SswBmhd.c" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/Ifx_Cfg_SswBmhd.o":"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/Ifx_Cfg_SswBmhd.src" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-Seekfree_TC377_Opensource_Library-2f-libraries-2f-infineon_libraries-2f-Configurations

clean-Seekfree_TC377_Opensource_Library-2f-libraries-2f-infineon_libraries-2f-Configurations:
	-$(RM) ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/Ifx_Cfg_Ssw.d ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/Ifx_Cfg_Ssw.o ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/Ifx_Cfg_Ssw.src ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/Ifx_Cfg_SswBmhd.d ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/Ifx_Cfg_SswBmhd.o ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Configurations/Ifx_Cfg_SswBmhd.src

.PHONY: clean-Seekfree_TC377_Opensource_Library-2f-libraries-2f-infineon_libraries-2f-Configurations

