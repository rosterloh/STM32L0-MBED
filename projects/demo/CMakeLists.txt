# ------------------------------------------------------------------------------
# set the arm toolchain
INCLUDE(CMakeForceCompiler)

#-------------------------------------------------------------------------------
# Targeting an embedded system, no OS.
SET(CMAKE_SYSTEM_NAME Generic)

#-------------------------------------------------------------------------------
# specify the cross compiler, later on we will set the correct path
CMAKE_FORCE_C_COMPILER(arm-none-eabi-gcc GNU)
CMAKE_FORCE_CXX_COMPILER(arm-none-eabi-g++ GNU)

#-------------------------------------------------------------------------------
set(TOOLCHAIN TOOLCHAIN_GCC_ARM)

#-------------------------------------------------------------------------------
# define presets
set(USE_RTOS false)
set(USE_NET false)
set(USE_USB false)
set(USE_DSP false)

cmake_minimum_required(VERSION 3.0.1)

# The name of our project is "BlinkLed". CMakeLists files in this project can
# refer to the root source directory of the project as ${BlinkLed_SOURCE_DIR}
# and to the root binary directory of the project as ${BlinkLed_BINARY_DIR}.
project (BlinkLed C CXX ASM)

# The version number.
set (BlinkLed_VERSION_MAJOR 0)
set (BlinkLed_VERSION_MINOR 0)
set (BlinkLed_VERSION_PATCH 1)
set (BlinkLed_VERSION_TWEAK 0)

set(MBED_PATH ${BlinkLed_SOURCE_DIR}/../../mbed-src/libraries/mbed)
set(ARM_GCC_PATH "C:\ProgramData\chocolatey\lib\gcc-arm-embedded.4.8.2014.3\tools")

set(MBED_VENDOR "STM")
set(MBED_CPU "NUCLEO_L053R8")
set(MBED_CORE "cortex-m0")
set(MBED_INSTRUCTIONSET "M0")

set(MBED_STARTUP "startup_stm32l053xx.o")
set(MBED_SYSTEM "system_stm32l0xx.o")
set(MBED_LINK_TARGET ${MBED_CPU})

# ------------------------------------------------------------------------------
# compiler settings
SET(COMMON_FLAGS "${COMMON_FLAGS} -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fno-exceptions -fno-builtin -MMD -fno-delete-null-pointer-checks")
SET(COMMON_FLAGS "${COMMON_FLAGS} -mcpu=${MBED_CORE} -O2 -mthumb -fno-exceptions -msoft-float -ffunction-sections -fdata-sections -g -fno-common -fmessage-length=0")

SET(MBED_DEFINES "${MBED_DEFINES} -DTARGET_${MBED_CPU}")
SET(MBED_DEFINES "${MBED_DEFINES} -DTARGET_${MBED_INSTRUCTIONSET}")
SET(MBED_DEFINES "${MBED_DEFINES} -DTARGET_${MBED_VENDOR}")
SET(MBED_DEFINES "${MBED_DEFINES} -DTOOLCHAIN_GCC_ARM")
SET(MBED_DEFINES "${MBED_DEFINES} -DTOOLCHAIN_GCC")

SET(CMAKE_CXX_FLAGS "${COMMON_FLAGS} ${MBED_DEFINES} -std=gnu++0x")
SET(CMAKE_C_FLAGS "${COMMON_FLAGS} ${MBED_DEFINES} -std=gnu99")


# ------------------------------------------------------------------------------
# setup precompiled mbed files which will be needed for all projects
set(MBED_OBJECTS
  ${MBED_PATH}/TARGET_${MBED_TARGET}/${TOOLCHAIN}/${MBED_STARTUP}
  ${MBED_PATH}/TARGET_${MBED_TARGET}/${TOOLCHAIN}/${MBED_SYSTEM}
  ${MBED_PATH}/TARGET_${MBED_TARGET}/${TOOLCHAIN}/cmsis_nvic.o
  ${MBED_PATH}/TARGET_${MBED_TARGET}/${TOOLCHAIN}/retarget.o
  ${MBED_PATH}/TARGET_${MBED_TARGET}/${TOOLCHAIN}/board.o
)

# ------------------------------------------------------------------------------
# libraries for mbed
set(MBED_LIBS mbed stdc++ supc++ m gcc g c nosys rdimon)

# ------------------------------------------------------------------------------
# linker settings
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections -Wl,--wrap,main --specs=nano.specs  -u _printf_float -u _scanf_float")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \"-T${MBED_PATH}/mbed/TARGET_${MBED_TARGET}/${TOOLCHAIN}/${MBED_LINK_TARGET}.ld\" -static")

# ------------------------------------------------------------------------------
# mbed
include_directories("${MBED_PATH}/mbed/")
include_directories("${MBED_PATH}/mbed/TARGET_${MBED_TARGET}/")
include_directories("${MBED_PATH}/mbed/TARGET_${MBED_TARGET}/${TOOLCHAIN}")
include_directories("${MBED_PATH}/mbed/TARGET_${MBED_TARGET}/TARGET_${MBED_VENDOR}/TARGET_${MBED_FAMILY}/")
include_directories("${MBED_PATH}/mbed/TARGET_${MBED_TARGET}/TARGET_${MBED_VENDOR}/TARGET_${MBED_FAMILY}/TARGET_${MBED_CPU}")

link_directories("${MBED_PATH}/mbed/TARGET_${MBED_TARGET}/${TOOLCHAIN}")

# add networking
if(${USE_NET} STREQUAL "true")
  include_directories("${MBED_PATH}/net/eth/")
  include_directories("${MBED_PATH}/net/eth/EthernetInterface")
  include_directories("${MBED_PATH}/net/eth/Socket")
  include_directories("${MBED_PATH}/net/eth/TARGET_${MBED_TARGET}/")
  include_directories("${MBED_PATH}/net/eth/TARGET_${MBED_TARGET}/${TOOLCHAIN}")

  include_directories("${MBED_PATH}/net/eth/lwip")
  include_directories("${MBED_PATH}/net/eth/lwip/include")
  include_directories("${MBED_PATH}/net/eth/lwip/include/ipv4")
  include_directories("${MBED_PATH}/net/eth/lwip-sys")
  include_directories("${MBED_PATH}/net/eth/lwip-eth/arch/TARGET_${MBED_VENDOR}")

  link_directories("${MBED_PATH}/net/eth/TARGET_${MBED_TARGET}/${TOOLCHAIN}")
  set(MBED_LIBS ${MBED_LIBS} eth)

  # supress lwip warnings with 0x11
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-literal-suffix")

  set(USE_RTOS true)
endif()

# add rtos
if(${USE_RTOS} STREQUAL "true")
  include_directories("${MBED_PATH}/rtos/")
  include_directories("${MBED_PATH}/rtos/TARGET_${MBED_TARGET}/")
  include_directories("${MBED_PATH}/rtos/TARGET_${MBED_TARGET}/${TOOLCHAIN}")

  link_directories("${MBED_PATH}/rtos/TARGET_${MBED_TARGET}/${TOOLCHAIN}")
  set(MBED_LIBS ${MBED_LIBS} rtos rtx)
endif()

# add usb
if(${USE_USB} STREQUAL "true")
  include_directories("${MBED_PATH}/USBDevice/")
  include_directories("${MBED_PATH}/USBDevice/TARGET_${MBED_TARGET}/")
  include_directories("${MBED_PATH}/USBDevice/TARGET_${MBED_TARGET}/${TOOLCHAIN}")

  link_directories("${MBED_PATH}/usb/TARGET_${MBED_TARGET}/${TOOLCHAIN}")
  set(MBED_LIBS ${MBED_LIBS} USBDevice)
endif()

# add dsp
if(${USE_DSP} STREQUAL "true")
  include_directories("${MBED_PATH}/dsp/")
  include_directories("${MBED_PATH}/dsp/TARGET_${MBED_TARGET}/")
  include_directories("${MBED_PATH}/dsp/TARGET_${MBED_TARGET}/${TOOLCHAIN}")

  link_directories("${MBED_PATH}/dsp/TARGET_${MBED_TARGET}/${TOOLCHAIN}")
  set(MBED_LIBS ${MBED_LIBS} cmsis_dsp dsp)
endif()

# print all include directories
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
message(STATUS "Include Directories")
foreach(dir ${dirs})
  message(STATUS "  ${dir}")
endforeach()

# ------------------------------------------------------------------------------
# build the executable
set(BLINKLED_SRC
  example/BlinkLed.cpp
)

set(BIN
  BlinkLed
)

include(../arm_mbed.cmake)

# ------------------------------------------------------------------------------
# build the executable
add_executable(${BIN} ${BLINKLED_SRC} ${MBED_OBJECTS})
target_link_libraries(${BIN} ${MBED_LIBS})
