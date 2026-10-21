################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Seekfree_TC377_Opensource_Library/code/encoder.c" \
"../Seekfree_TC377_Opensource_Library/code/image.c" \
"../Seekfree_TC377_Opensource_Library/code/image_transport.c" \
"../Seekfree_TC377_Opensource_Library/code/imu.c" \
"../Seekfree_TC377_Opensource_Library/code/init.c" \
"../Seekfree_TC377_Opensource_Library/code/menu.c" \
"../Seekfree_TC377_Opensource_Library/code/motor.c" \
"../Seekfree_TC377_Opensource_Library/code/scheduler.c" \
"../Seekfree_TC377_Opensource_Library/code/servo.c" \
"../Seekfree_TC377_Opensource_Library/code/tc377_port.c" \
"../Seekfree_TC377_Opensource_Library/code/vision_client.c" \
"../Seekfree_TC377_Opensource_Library/code/vision_server.c" \
"../Seekfree_TC377_Opensource_Library/code/vision_shared.c" \
"../Seekfree_TC377_Opensource_Library/code/vofa.c" \
"../Seekfree_TC377_Opensource_Library/code/voltage.c" 

COMPILED_SRCS += \
"Seekfree_TC377_Opensource_Library/code/encoder.src" \
"Seekfree_TC377_Opensource_Library/code/image.src" \
"Seekfree_TC377_Opensource_Library/code/image_transport.src" \
"Seekfree_TC377_Opensource_Library/code/imu.src" \
"Seekfree_TC377_Opensource_Library/code/init.src" \
"Seekfree_TC377_Opensource_Library/code/menu.src" \
"Seekfree_TC377_Opensource_Library/code/motor.src" \
"Seekfree_TC377_Opensource_Library/code/scheduler.src" \
"Seekfree_TC377_Opensource_Library/code/servo.src" \
"Seekfree_TC377_Opensource_Library/code/tc377_port.src" \
"Seekfree_TC377_Opensource_Library/code/vision_client.src" \
"Seekfree_TC377_Opensource_Library/code/vision_server.src" \
"Seekfree_TC377_Opensource_Library/code/vision_shared.src" \
"Seekfree_TC377_Opensource_Library/code/vofa.src" \
"Seekfree_TC377_Opensource_Library/code/voltage.src" 

C_DEPS += \
"./Seekfree_TC377_Opensource_Library/code/encoder.d" \
"./Seekfree_TC377_Opensource_Library/code/image.d" \
"./Seekfree_TC377_Opensource_Library/code/image_transport.d" \
"./Seekfree_TC377_Opensource_Library/code/imu.d" \
"./Seekfree_TC377_Opensource_Library/code/init.d" \
"./Seekfree_TC377_Opensource_Library/code/menu.d" \
"./Seekfree_TC377_Opensource_Library/code/motor.d" \
"./Seekfree_TC377_Opensource_Library/code/scheduler.d" \
"./Seekfree_TC377_Opensource_Library/code/servo.d" \
"./Seekfree_TC377_Opensource_Library/code/tc377_port.d" \
"./Seekfree_TC377_Opensource_Library/code/vision_client.d" \
"./Seekfree_TC377_Opensource_Library/code/vision_server.d" \
"./Seekfree_TC377_Opensource_Library/code/vision_shared.d" \
"./Seekfree_TC377_Opensource_Library/code/vofa.d" \
"./Seekfree_TC377_Opensource_Library/code/voltage.d" 

OBJS += \
"Seekfree_TC377_Opensource_Library/code/encoder.o" \
"Seekfree_TC377_Opensource_Library/code/image.o" \
"Seekfree_TC377_Opensource_Library/code/image_transport.o" \
"Seekfree_TC377_Opensource_Library/code/imu.o" \
"Seekfree_TC377_Opensource_Library/code/init.o" \
"Seekfree_TC377_Opensource_Library/code/menu.o" \
"Seekfree_TC377_Opensource_Library/code/motor.o" \
"Seekfree_TC377_Opensource_Library/code/scheduler.o" \
"Seekfree_TC377_Opensource_Library/code/servo.o" \
"Seekfree_TC377_Opensource_Library/code/tc377_port.o" \
"Seekfree_TC377_Opensource_Library/code/vision_client.o" \
"Seekfree_TC377_Opensource_Library/code/vision_server.o" \
"Seekfree_TC377_Opensource_Library/code/vision_shared.o" \
"Seekfree_TC377_Opensource_Library/code/vofa.o" \
"Seekfree_TC377_Opensource_Library/code/voltage.o" 


# Each subdirectory must supply rules for building sources it contributes
"Seekfree_TC377_Opensource_Library/code/encoder.src":"../Seekfree_TC377_Opensource_Library/code/encoder.c" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/encoder.o":"Seekfree_TC377_Opensource_Library/code/encoder.src" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/image.src":"../Seekfree_TC377_Opensource_Library/code/image.c" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/image.o":"Seekfree_TC377_Opensource_Library/code/image.src" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/image_transport.src":"../Seekfree_TC377_Opensource_Library/code/image_transport.c" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/image_transport.o":"Seekfree_TC377_Opensource_Library/code/image_transport.src" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/imu.src":"../Seekfree_TC377_Opensource_Library/code/imu.c" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/imu.o":"Seekfree_TC377_Opensource_Library/code/imu.src" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/init.src":"../Seekfree_TC377_Opensource_Library/code/init.c" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/init.o":"Seekfree_TC377_Opensource_Library/code/init.src" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/menu.src":"../Seekfree_TC377_Opensource_Library/code/menu.c" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/menu.o":"Seekfree_TC377_Opensource_Library/code/menu.src" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/motor.src":"../Seekfree_TC377_Opensource_Library/code/motor.c" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/motor.o":"Seekfree_TC377_Opensource_Library/code/motor.src" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/scheduler.src":"../Seekfree_TC377_Opensource_Library/code/scheduler.c" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/scheduler.o":"Seekfree_TC377_Opensource_Library/code/scheduler.src" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/servo.src":"../Seekfree_TC377_Opensource_Library/code/servo.c" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/servo.o":"Seekfree_TC377_Opensource_Library/code/servo.src" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/tc377_port.src":"../Seekfree_TC377_Opensource_Library/code/tc377_port.c" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/tc377_port.o":"Seekfree_TC377_Opensource_Library/code/tc377_port.src" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/vision_client.src":"../Seekfree_TC377_Opensource_Library/code/vision_client.c" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/vision_client.o":"Seekfree_TC377_Opensource_Library/code/vision_client.src" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/vision_server.src":"../Seekfree_TC377_Opensource_Library/code/vision_server.c" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/vision_server.o":"Seekfree_TC377_Opensource_Library/code/vision_server.src" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/vision_shared.src":"../Seekfree_TC377_Opensource_Library/code/vision_shared.c" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/vision_shared.o":"Seekfree_TC377_Opensource_Library/code/vision_shared.src" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/vofa.src":"../Seekfree_TC377_Opensource_Library/code/vofa.c" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/vofa.o":"Seekfree_TC377_Opensource_Library/code/vofa.src" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/voltage.src":"../Seekfree_TC377_Opensource_Library/code/voltage.c" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/code/voltage.o":"Seekfree_TC377_Opensource_Library/code/voltage.src" "Seekfree_TC377_Opensource_Library/code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-Seekfree_TC377_Opensource_Library-2f-code

clean-Seekfree_TC377_Opensource_Library-2f-code:
	-$(RM) ./Seekfree_TC377_Opensource_Library/code/encoder.d ./Seekfree_TC377_Opensource_Library/code/encoder.o ./Seekfree_TC377_Opensource_Library/code/encoder.src ./Seekfree_TC377_Opensource_Library/code/image.d ./Seekfree_TC377_Opensource_Library/code/image.o ./Seekfree_TC377_Opensource_Library/code/image.src ./Seekfree_TC377_Opensource_Library/code/image_transport.d ./Seekfree_TC377_Opensource_Library/code/image_transport.o ./Seekfree_TC377_Opensource_Library/code/image_transport.src ./Seekfree_TC377_Opensource_Library/code/imu.d ./Seekfree_TC377_Opensource_Library/code/imu.o ./Seekfree_TC377_Opensource_Library/code/imu.src ./Seekfree_TC377_Opensource_Library/code/init.d ./Seekfree_TC377_Opensource_Library/code/init.o ./Seekfree_TC377_Opensource_Library/code/init.src ./Seekfree_TC377_Opensource_Library/code/menu.d ./Seekfree_TC377_Opensource_Library/code/menu.o ./Seekfree_TC377_Opensource_Library/code/menu.src ./Seekfree_TC377_Opensource_Library/code/motor.d ./Seekfree_TC377_Opensource_Library/code/motor.o ./Seekfree_TC377_Opensource_Library/code/motor.src ./Seekfree_TC377_Opensource_Library/code/scheduler.d ./Seekfree_TC377_Opensource_Library/code/scheduler.o ./Seekfree_TC377_Opensource_Library/code/scheduler.src ./Seekfree_TC377_Opensource_Library/code/servo.d ./Seekfree_TC377_Opensource_Library/code/servo.o ./Seekfree_TC377_Opensource_Library/code/servo.src ./Seekfree_TC377_Opensource_Library/code/tc377_port.d ./Seekfree_TC377_Opensource_Library/code/tc377_port.o ./Seekfree_TC377_Opensource_Library/code/tc377_port.src ./Seekfree_TC377_Opensource_Library/code/vision_client.d ./Seekfree_TC377_Opensource_Library/code/vision_client.o ./Seekfree_TC377_Opensource_Library/code/vision_client.src ./Seekfree_TC377_Opensource_Library/code/vision_server.d ./Seekfree_TC377_Opensource_Library/code/vision_server.o ./Seekfree_TC377_Opensource_Library/code/vision_server.src ./Seekfree_TC377_Opensource_Library/code/vision_shared.d ./Seekfree_TC377_Opensource_Library/code/vision_shared.o ./Seekfree_TC377_Opensource_Library/code/vision_shared.src ./Seekfree_TC377_Opensource_Library/code/vofa.d ./Seekfree_TC377_Opensource_Library/code/vofa.o ./Seekfree_TC377_Opensource_Library/code/vofa.src ./Seekfree_TC377_Opensource_Library/code/voltage.d ./Seekfree_TC377_Opensource_Library/code/voltage.o ./Seekfree_TC377_Opensource_Library/code/voltage.src

.PHONY: clean-Seekfree_TC377_Opensource_Library-2f-code

