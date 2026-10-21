################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_adc.c" \
"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_delay.c" \
"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_dma.c" \
"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_encoder.c" \
"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_exti.c" \
"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_flash.c" \
"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_gpio.c" \
"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_pit.c" \
"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_pwm.c" \
"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_soft_iic.c" \
"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_soft_spi.c" \
"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_spi.c" \
"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_timer.c" \
"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_uart.c" 

COMPILED_SRCS += \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_adc.src" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_delay.src" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_dma.src" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_encoder.src" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_exti.src" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_flash.src" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_gpio.src" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_pit.src" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_pwm.src" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_soft_iic.src" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_soft_spi.src" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_spi.src" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_timer.src" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_uart.src" 

C_DEPS += \
"./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_adc.d" \
"./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_delay.d" \
"./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_dma.d" \
"./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_encoder.d" \
"./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_exti.d" \
"./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_flash.d" \
"./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_gpio.d" \
"./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_pit.d" \
"./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_pwm.d" \
"./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_soft_iic.d" \
"./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_soft_spi.d" \
"./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_spi.d" \
"./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_timer.d" \
"./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_uart.d" 

OBJS += \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_adc.o" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_delay.o" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_dma.o" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_encoder.o" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_exti.o" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_flash.o" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_gpio.o" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_pit.o" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_pwm.o" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_soft_iic.o" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_soft_spi.o" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_spi.o" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_timer.o" \
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_uart.o" 


# Each subdirectory must supply rules for building sources it contributes
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_adc.src":"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_adc.c" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_adc.o":"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_adc.src" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_delay.src":"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_delay.c" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_delay.o":"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_delay.src" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_dma.src":"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_dma.c" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_dma.o":"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_dma.src" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_encoder.src":"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_encoder.c" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_encoder.o":"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_encoder.src" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_exti.src":"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_exti.c" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_exti.o":"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_exti.src" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_flash.src":"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_flash.c" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_flash.o":"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_flash.src" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_gpio.src":"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_gpio.c" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_gpio.o":"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_gpio.src" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_pit.src":"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_pit.c" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_pit.o":"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_pit.src" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_pwm.src":"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_pwm.c" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_pwm.o":"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_pwm.src" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_soft_iic.src":"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_soft_iic.c" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_soft_iic.o":"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_soft_iic.src" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_soft_spi.src":"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_soft_spi.c" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_soft_spi.o":"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_soft_spi.src" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_spi.src":"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_spi.c" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_spi.o":"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_spi.src" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_timer.src":"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_timer.c" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_timer.o":"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_timer.src" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_uart.src":"../Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_uart.c" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_uart.o":"Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_uart.src" "Seekfree_TC377_Opensource_Library/libraries/zf_driver/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-Seekfree_TC377_Opensource_Library-2f-libraries-2f-zf_driver

clean-Seekfree_TC377_Opensource_Library-2f-libraries-2f-zf_driver:
	-$(RM) ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_adc.d ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_adc.o ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_adc.src ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_delay.d ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_delay.o ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_delay.src ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_dma.d ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_dma.o ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_dma.src ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_encoder.d ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_encoder.o ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_encoder.src ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_exti.d ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_exti.o ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_exti.src ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_flash.d ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_flash.o ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_flash.src ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_gpio.d ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_gpio.o ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_gpio.src ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_pit.d ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_pit.o ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_pit.src ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_pwm.d ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_pwm.o ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_pwm.src ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_soft_iic.d ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_soft_iic.o ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_soft_iic.src ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_soft_spi.d ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_soft_spi.o ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_soft_spi.src ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_spi.d ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_spi.o ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_spi.src ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_timer.d ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_timer.o ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_timer.src ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_uart.d ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_uart.o ./Seekfree_TC377_Opensource_Library/libraries/zf_driver/zf_driver_uart.src

.PHONY: clean-Seekfree_TC377_Opensource_Library-2f-libraries-2f-zf_driver

