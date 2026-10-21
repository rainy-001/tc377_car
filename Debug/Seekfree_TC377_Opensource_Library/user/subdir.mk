################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Seekfree_TC377_Opensource_Library/user/cpu0_main.c" \
"../Seekfree_TC377_Opensource_Library/user/cpu1_main.c" \
"../Seekfree_TC377_Opensource_Library/user/cpu2_main.c" \
"../Seekfree_TC377_Opensource_Library/user/isr.c" 

COMPILED_SRCS += \
"Seekfree_TC377_Opensource_Library/user/cpu0_main.src" \
"Seekfree_TC377_Opensource_Library/user/cpu1_main.src" \
"Seekfree_TC377_Opensource_Library/user/cpu2_main.src" \
"Seekfree_TC377_Opensource_Library/user/isr.src" 

C_DEPS += \
"./Seekfree_TC377_Opensource_Library/user/cpu0_main.d" \
"./Seekfree_TC377_Opensource_Library/user/cpu1_main.d" \
"./Seekfree_TC377_Opensource_Library/user/cpu2_main.d" \
"./Seekfree_TC377_Opensource_Library/user/isr.d" 

OBJS += \
"Seekfree_TC377_Opensource_Library/user/cpu0_main.o" \
"Seekfree_TC377_Opensource_Library/user/cpu1_main.o" \
"Seekfree_TC377_Opensource_Library/user/cpu2_main.o" \
"Seekfree_TC377_Opensource_Library/user/isr.o" 


# Each subdirectory must supply rules for building sources it contributes
"Seekfree_TC377_Opensource_Library/user/cpu0_main.src":"../Seekfree_TC377_Opensource_Library/user/cpu0_main.c" "Seekfree_TC377_Opensource_Library/user/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/user/cpu0_main.o":"Seekfree_TC377_Opensource_Library/user/cpu0_main.src" "Seekfree_TC377_Opensource_Library/user/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/user/cpu1_main.src":"../Seekfree_TC377_Opensource_Library/user/cpu1_main.c" "Seekfree_TC377_Opensource_Library/user/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/user/cpu1_main.o":"Seekfree_TC377_Opensource_Library/user/cpu1_main.src" "Seekfree_TC377_Opensource_Library/user/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/user/cpu2_main.src":"../Seekfree_TC377_Opensource_Library/user/cpu2_main.c" "Seekfree_TC377_Opensource_Library/user/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/user/cpu2_main.o":"Seekfree_TC377_Opensource_Library/user/cpu2_main.src" "Seekfree_TC377_Opensource_Library/user/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/user/isr.src":"../Seekfree_TC377_Opensource_Library/user/isr.c" "Seekfree_TC377_Opensource_Library/user/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/user/isr.o":"Seekfree_TC377_Opensource_Library/user/isr.src" "Seekfree_TC377_Opensource_Library/user/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-Seekfree_TC377_Opensource_Library-2f-user

clean-Seekfree_TC377_Opensource_Library-2f-user:
	-$(RM) ./Seekfree_TC377_Opensource_Library/user/cpu0_main.d ./Seekfree_TC377_Opensource_Library/user/cpu0_main.o ./Seekfree_TC377_Opensource_Library/user/cpu0_main.src ./Seekfree_TC377_Opensource_Library/user/cpu1_main.d ./Seekfree_TC377_Opensource_Library/user/cpu1_main.o ./Seekfree_TC377_Opensource_Library/user/cpu1_main.src ./Seekfree_TC377_Opensource_Library/user/cpu2_main.d ./Seekfree_TC377_Opensource_Library/user/cpu2_main.o ./Seekfree_TC377_Opensource_Library/user/cpu2_main.src ./Seekfree_TC377_Opensource_Library/user/isr.d ./Seekfree_TC377_Opensource_Library/user/isr.o ./Seekfree_TC377_Opensource_Library/user/isr.src

.PHONY: clean-Seekfree_TC377_Opensource_Library-2f-user

