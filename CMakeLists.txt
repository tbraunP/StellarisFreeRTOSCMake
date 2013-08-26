cmake_minimum_required(VERSION 2.8)
project(blinieCPPCMake)
SET(TARGET "blinieCPPCMake")

#custom command to use objcopy to create .bin files out of ELF files
function(make_firmware INPUT)
              add_custom_command(TARGET ${INPUT}
                      COMMAND arm-none-eabi-objcopy -O binary ${INPUT} ${TARGET}.bin
                      COMMENT "objcopying to make mbed compatible firmware")
              set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${TARGET}.bin)
endfunction(make_firmware)


INCLUDE(CMakeForceCompiler)
 
SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
CMAKE_FORCE_C_COMPILER(arm-none-eabi-gcc GNU)
CMAKE_FORCE_CXX_COMPILER(arm-none-eabi-g++ GNU)
 
find_program(LINKER arm-none-eabi-ld)
find_program(OBJCOPY arm-none-eabi-objcopy)
find_program(OBJDUMP arm-none-eabi-objdump)
SET(CMAKE_AR arm-none-eabi-ar)
SET(CMAKE_RANLIB arm-none-eabi-ranlib)

# Set compiler and linker options
SET(LIBS_FLAG "-Dgcc -DPART_LM4F120H5QR -DTARGET_IS_BLIZZARD_RA1 -D__VFP_FP__")
SET(COMMON_FLAGS "-O0 -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -ffunction-sections -fdata-sections -fno-common -fmessage-length=0 -fsingle-precision-constant -Wall ${LIBS_FLAG}")
SET(CMAKE_CXX_FLAGS "${COMMON_FLAGS}  -fno-rtti  -fno-exceptions -std=gnu++0x")
SET(CMAKE_C_FLAGS "${COMMON_FLAGS} -std=gnu99")
SET(CMAKE_EXE_LINKER_FLAGS "-lm -Wl,-Map=${TARGET}.map,--cref -Wl,--gc-sections \"-T${CMAKE_SOURCE_DIR}/lib/StellarisCMSIS/StellarisLaunchPad.ld\" -static")
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${TARGET}.map)

#remove -rdynamic to link static
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS) 
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

# Debug Options
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS}  -g")
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g")

# add libs in subdirectory
add_subdirectory (lib/StellarisCMSIS)
add_subdirectory (lib/StellarisDriverLib)
add_subdirectory (lib/FreeRTOSV7.5.2)

# add -I compile headers for LIBS
include_directories("lib/StellarisCMSIS/inc")
include_directories("lib/StellarisDriverLib/inc")

# freertos include files
include_directories("lib/FreeRTOSV7.5.2/FreeRTOS/Source/include")
include_directories("lib/FreeRTOSV7.5.2/FreeRTOS/Source/portable/GCC/ARM_CM4F")

include_directories("inc")
include_directories("src")

# add source files from folder
AUX_SOURCE_DIRECTORY("src/" SRCS)
AUX_SOURCE_DIRECTORY("src/syscalls/" SRCSCalls)

# define target
add_executable(${TARGET}.elf ${SRCS} ${SRCSCalls})
target_link_libraries (${TARGET}.elf m c StellarisCMSIS StellarisDriverLib FREERTOS_752)

make_firmware(${TARGET}.elf)
