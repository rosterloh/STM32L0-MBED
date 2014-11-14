# ------------------------------------------------------------------------------
# set the arm toolchain
set(CMAKE_TOOLCHAIN_FILE ../gcc_arm_toolchain.cmake)

cmake_minimum_required(VERSION 3.0.1)

project (BlinkLed C CXX ASM)

# The version number.
set (BlinkLed_VERSION_MAJOR 1)
set (BlinkLed_VERSION_MINOR 0)
set (BlinkLed_VERSION_PATCH 0)
set (BlinkLed_VERSION_TWEAK 0)


# ------------------------------------------------------------------------------
# set the target processor and include settings for cmake
set(MBED_PATH ${BlinkLed_SOURCE_DIR}/../../mbed-src/libraries)
set(MBED_TARGET DISCO_L053C8)

# ------------------------------------------------------------------------------
# will add the include dirs and link the lib
#set(USE_RTOS true)
#set(USE_NET true)
#set(USE_USB true)
#set(USE_DSP true)

# ------------------------------------------------------------------------------
# for installing an serial console
#set(MBEDMOUNT /Volumes/MBED)
#set(SERCON /dev/tty.usbmodem1442)

add_sources(
  BlinkLed.cpp
)

include(${BlinkLed_SOURCE_DIR}/../arm_mbed.cmake)

message(STATUS "Source Directories")
foreach(src ${SRCS})
  message(STATUS "  ${src}")
endforeach()

# ------------------------------------------------------------------------------
# build the executable
add_executable(${PROJECT_NAME}.elf ${SRCS})
#target_link_libraries(${BIN} ${MBED_LIBS})

add_custom_command(
  TARGET ${PROJECT_NAME}.elf
  COMMAND arm-none-eabi-size ${PROJECT_NAME}.elf
  COMMAND arm-none-eabi-objcopy -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex # convert .elf to .hex (redundancy: only one of either .hex or .bin is needed)
  COMMAND arm-none-eabi-objcopy -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin # convert .elf to .hex
)
