# ------------------------------------------------------------------------------
# Compiler variables for MBED
# This file should be included towards the end of your CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.10)


if(NOT DEFINED MBED_PATH)
  message(FATAL_ERROR "MBED_PATH is not defined. Please set in CMakeLists.txt")
endif()

# ------------------------------------------------------------------------------
# custom target for copying to mbed device
#add_custom_target(upload
#  arm-none-eabi-objcopy -O binary ${BIN} ${BIN}.bin
#  COMMAND cp ${BIN}.bin ${MBEDMOUNT}
#)

# ------------------------------------------------------------------------------
# custom target for opening serial console
#add_custom_target(sercon
#  command screen ${SERCON} 9600
#)

# ------------------------------------------------------------------------------
# setup processor settings add aditional boards here
#  LPC1768, LPC11U24, NRF51822, K64F

# TARGET -> has to be set in CMakeLists.txt
#
# MBED_VENDOR -> CPU Manufacturer
#
message(STATUS "building for ${MBED_TARGET}")
# the settings for mbed is really messed up ;)
if(MBED_TARGET MATCHES "LPC1768")
  set(MBED_VENDOR "NXP")
  set(MBED_FAMILY "LPC176X")
  set(MBED_CPU "MBED_LPC1768")
  set(MBED_CORE "cortex-m3")
  set(MBED_INSTRUCTIONSET "M3")

  set(MBED_STARTUP "startup_LPC17xx.o")
  set(MBED_SYSTEM "system_LPC17xx.o")
  set(MBED_LINK_TARGET ${MBED_TARGET})

elseif(MBED_TARGET MATCHES "LPC11U24")
  set(MBED_VENDOR "NXP")
  set(MBED_FAMILY "LPC11UXX")
  set(MBED_CPU "LPC11U24_401")
  set(MBED_CORE "cortex-m0")
  set(MBED_INSTRUCTIONSET "M0")

  set(MBED_STARTUP "startup_LPC11xx.o")
  set(MBED_SYSTEM "system_LPC11Uxx.o")
  set(MBED_LINK_TARGET ${MBED_TARGET})

elseif(MBED_TARGET MATCHES "DISCO_L053C8")
  set(MBED_VENDOR "STM")
  set(MBED_FAMILY "STM32L0")
  set(MBED_CPU "STM32L053C8")
  set(MBED_CORE "cortex-m0plus")
  set(MBED_INSTRUCTIONSET "M0P")

  set(MBED_STARTUP "startup_stm32l053xx.o")
  set(MBED_SYSTEM "system_stm32l0xx.o")
  set(MBED_LINK_TARGET "STM32L0xx")

elseif(MBED_TARGET MATCHES "RBLAB_NRF51822")
  set(MBED_VENDOR "NORDIC")
  set(MBED_FAMILY "MCU_NRF51822")
  set(MBED_CPU "RBLAB_NRF51822")
  set(MBED_CORE "cortex-m0")
  set(MBED_INSTRUCTIONSET "M0")

  set(MBED_STARTUP "startup_NRF51822.o")
  set(MBED_SYSTEM "system_nrf51822.o")
  set(MBED_LINK_TARGET "NRF51822")

else()
   message(FATAL_ERROR "No MBED_TARGET specified or available. Full stop :(")
endif()

message("Building for ${MBED_VENDOR} ${MBED_TARGET}")

# ------------------------------------------------------------------------------
# compiler settings
add_definitions("-DTARGET_${MBED_VENDOR}")
add_definitions("-DTARGET_${MBED_FAMILY}")
add_definitions("-DTARGET_${MBED_CPU}")
add_definitions("-DTARGET_${MBED_TARGET}")
add_definitions("-DTARGET_${MBED_INSTRUCTIONSET}")
if(MBED_INSTRUCTIONSET MATCHES "M0P")
add_definitions("-D__CORTEX_${MBED_INSTRUCTIONSET}LUS")
add_definitions("-DARM_MATH_${MBED_INSTRUCTIONSET}LUS")
else()
add_definitions("-D__CORTEX_${MBED_INSTRUCTIONSET}")
add_definitions("-DARM_MATH_${MBED_INSTRUCTIONSET}")
endif()
add_definitions("-DTOOLCHAIN_GCC_ARM")
add_definitions("-DTOOLCHAIN_GCC")
add_definitions("-D__MBED__=1")
add_definitions("-DMBED_USERNAME=rosterloh84")

add_definitions("-mcpu=${MBED_CORE}")
add_definitions(
  -Os
  -mthumb
  -Wall
  -Wextra
  -Wno-unused-parameter
  -Wno-missing-field-initializers
  -Wno-error=switch
  -Wno-switch
  -fmessage-length=0
  -fno-builtin
  -ffunction-sections
  -fdata-sections
  -fno-delete-null-pointer-checks
  -fomit-frame-pointer
  -fno-common
  -funsigned-bitfields
  -c
  -g
  -MMD
  -MP
) # -msoft-float

# Language specifc compiler flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x -fno-rtti -fno-exceptions -fno-threadsafe-statics")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -std=gnu99 -Wno-pointer-sign -Wno-pointer-to-int-cast")
set(CMAKE_ASM_FLAGS "${COMMON_COMPILE_FLAGS} -x assembler-with-cpp")

# ------------------------------------------------------------------------------
# linker settings
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS -T${MBED_PATH}/mbed/TARGET_${MBED_TARGET}/${TOOLCHAIN}/${MBED_LINK_TARGET}.ld)
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS} -Wl,--gc-sections -Wl,--wrap,main -Wl,-Map=${PROJECT_NAME}.map -mcpu=${MBED_CORE} -mthumb --specs=nano.specs -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys")

# ------------------------------------------------------------------------------
# setup mbed files which will be needed for all projects
file(GLOB MBED_SRC_SOURCES
       ${MBED_SRC_PATH}/common/*.c
       ${MBED_SRC_PATH}/common/*.cpp
       ${MBED_SRC_PATH}/targets/cmsis/TARGET_${MBED_VENDOR}/TARGET_${MBED_FAMILY}/*.c
       ${MBED_SRC_PATH}/targets/cmsis/TARGET_${MBED_VENDOR}/TARGET_${MBED_FAMILY}/TARGET_${MBED_TARGET}/*.c
       ${MBED_SRC_PATH}/targets/hal/TARGET_${MBED_VENDOR}/TARGET_${MBED_TARGET}/*.c
)
add_sources(${MBED_SRC_SOURCES})

# ------------------------------------------------------------------------------
# libraries for mbed
set(MBED_LIBS mbed stdc++ supc++ m gcc g c nosys rdimon)

# ------------------------------------------------------------------------------
# mbed
include_directories("${MBED_PATH}/mbed/")
include_directories("${MBED_PATH}/mbed/api/")
include_directories("${MBED_PATH}/mbed/common/")
include_directories("${MBED_PATH}/mbed/hal/")
include_directories("${MBED_PATH}/mbed/targets/")
include_directories("${MBED_PATH}/mbed/targets/cmsis/")
include_directories("${MBED_PATH}/mbed/targets/cmsis/TARGET_${MBED_VENDOR}/")
include_directories("${MBED_PATH}/mbed/targets/cmsis/TARGET_${MBED_VENDOR}/TARGET_${MBED_FAMILY}/")
include_directories("${MBED_PATH}/mbed/targets/cmsis/TARGET_${MBED_VENDOR}/TARGET_${MBED_FAMILY}/TARGET_${MBED_TARGET}/")
include_directories("${MBED_PATH}/mbed/targets/cmsis/TARGET_${MBED_VENDOR}/TARGET_${MBED_FAMILY}/TARGET_${MBED_TARGET}/${TOOLCHAIN}/")
include_directories("${MBED_PATH}/mbed/targets/hal/TARGET_${MBED_VENDOR}/TARGET_${MBED_TARGET}/")

link_directories("${MBED_PATH}/mbed/targets/cmsis/TARGET_${MBED_VENDOR}/TARGET_${MBED_FAMILY}/TARGET_${MBED_TARGET}/${TOOLCHAIN}")

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
  include_directories("${MBED_PATH}/rtos/rtos")
  include_directories("${MBED_PATH}/rtos/rtx/TARGET_CORTEX_M")
  include_directories("${MBED_PATH}/rtos/rtx/TARGET_CORTEX_M/TARGET_${MBED_INSTRUCTIONSET}/")
  include_directories("${MBED_PATH}/rtos/rtx/TARGET_CORTEX_M/TARGET_${MBED_INSTRUCTIONSET}/${TOOLCHAIN}")

  link_directories("${MBED_PATH}/rtos/rtx/TARGET_CORTEX_M/TARGET_${MBED_INSTRUCTIONSET}/${TOOLCHAIN}")
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
