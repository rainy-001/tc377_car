################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_clock.c" \
"../Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_debug.c" \
"../Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_fifo.c" \
"../Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_font.c" \
"../Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_function.c" \
"../Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_interrupt.c" 

COMPILED_SRCS += \
"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_clock.src" \
"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_debug.src" \
"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_fifo.src" \
"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_font.src" \
"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_function.src" \
"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_interrupt.src" 

C_DEPS += \
"./Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_clock.d" \
"./Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_debug.d" \
"./Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_fifo.d" \
"./Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_font.d" \
"./Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_function.d" \
"./Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_interrupt.d" 

OBJS += \
"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_clock.o" \
"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_debug.o" \
"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_fifo.o" \
"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_font.o" \
"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_function.o" \
"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_interrupt.o" 


# Each subdirectory must supply rules for building sources it contributes
"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_clock.src":"../Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_clock.c" "Seekfree_TC377_Opensource_Library/libraries/zf_common/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_clock.o":"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_clock.src" "Seekfree_TC377_Opensource_Library/libraries/zf_common/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_debug.src":"../Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_debug.c" "Seekfree_TC377_Opensource_Library/libraries/zf_common/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_debug.o":"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_debug.src" "Seekfree_TC377_Opensource_Library/libraries/zf_common/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_fifo.src":"../Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_fifo.c" "Seekfree_TC377_Opensource_Library/libraries/zf_common/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_fifo.o":"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_fifo.src" "Seekfree_TC377_Opensource_Library/libraries/zf_common/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_font.src":"../Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_font.c" "Seekfree_TC377_Opensource_Library/libraries/zf_common/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_font.o":"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_font.src" "Seekfree_TC377_Opensource_Library/libraries/zf_common/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_function.src":"../Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_function.c" "Seekfree_TC377_Opensource_Library/libraries/zf_common/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_function.o":"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_function.src" "Seekfree_TC377_Opensource_Library/libraries/zf_common/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_interrupt.src":"../Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_interrupt.c" "Seekfree_TC377_Opensource_Library/libraries/zf_common/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_interrupt.o":"Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_interrupt.src" "Seekfree_TC377_Opensource_Library/libraries/zf_common/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-Seekfree_TC377_Opensource_Library-2f-libraries-2f-zf_common

clean-Seekfree_TC377_Opensource_Library-2f-libraries-2f-zf_common:
	-$(RM) ./Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_clock.d ./Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_clock.o ./Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_clock.src ./Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_debug.d ./Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_debug.o ./Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_debug.src ./Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_fifo.d ./Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_fifo.o ./Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_fifo.src ./Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_font.d ./Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_font.o ./Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_font.src ./Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_function.d ./Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_function.o ./Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_function.src ./Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_interrupt.d ./Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_interrupt.o ./Seekfree_TC377_Opensource_Library/libraries/zf_common/zf_common_interrupt.src

.PHONY: clean-Seekfree_TC377_Opensource_Library-2f-libraries-2f-zf_common

