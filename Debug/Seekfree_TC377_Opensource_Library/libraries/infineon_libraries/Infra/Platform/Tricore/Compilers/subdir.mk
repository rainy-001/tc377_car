################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerDcc.c" \
"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGhs.c" \
"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGnuc.c" \
"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerTasking.c" 

COMPILED_SRCS += \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerDcc.src" \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGhs.src" \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGnuc.src" \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerTasking.src" 

C_DEPS += \
"./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerDcc.d" \
"./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGhs.d" \
"./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGnuc.d" \
"./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerTasking.d" 

OBJS += \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerDcc.o" \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGhs.o" \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGnuc.o" \
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerTasking.o" 


# Each subdirectory must supply rules for building sources it contributes
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerDcc.src":"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerDcc.c" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerDcc.o":"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerDcc.src" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGhs.src":"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGhs.c" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGhs.o":"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGhs.src" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGnuc.src":"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGnuc.c" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGnuc.o":"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGnuc.src" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerTasking.src":"../Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerTasking.c" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc37x "-fC:/Users/86166/Desktop/E01_gpio_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerTasking.o":"Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerTasking.src" "Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-Seekfree_TC377_Opensource_Library-2f-libraries-2f-infineon_libraries-2f-Infra-2f-Platform-2f-Tricore-2f-Compilers

clean-Seekfree_TC377_Opensource_Library-2f-libraries-2f-infineon_libraries-2f-Infra-2f-Platform-2f-Tricore-2f-Compilers:
	-$(RM) ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerDcc.d ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerDcc.o ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerDcc.src ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGhs.d ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGhs.o ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGhs.src ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGnuc.d ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGnuc.o ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGnuc.src ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerTasking.d ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerTasking.o ./Seekfree_TC377_Opensource_Library/libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerTasking.src

.PHONY: clean-Seekfree_TC377_Opensource_Library-2f-libraries-2f-infineon_libraries-2f-Infra-2f-Platform-2f-Tricore-2f-Compilers

