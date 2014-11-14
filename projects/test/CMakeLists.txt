#
# CMake configuration
#
# Please refer to http://www.cmake.org/cmake/help/documentation.html
# You may also refer to http://www.cmake.org/cmake/help/syntax.html for a quick
# introduction to CMake's syntax.

cmake_minimum_required (VERSION 2.8)

# The name of our project is "TEST". CMakeLists files in this project can
# refer to the root source directory of the project as ${TEST}
# and to the root binary directory of the project as ${TEST}.
project (TEST)

# define some more paths to projects we depend on
set (MBED_SRC_PATH     ${TEST_SOURCE_DIR}/../../mbed-src/libraries/mbed)

# It's best to hide all the details of setting up the variable SRCS in a CMake
# macro. The macro can then be called in all the project CMake list files to add
# sources.
#
# The macro first computes the path of the source file relative to the project
# root for each argument. If the macro is invoked from inside a project sub
# directory the new value of the variable SRCS needs to be propagated to the
# parent folder by using the PARENT_SCOPE option.
#
# Source: http://stackoverflow.com/questions/7046956/populating-srcs-from-cmakelists-txt-in-subdirectories
macro (add_sources)
    file (RELATIVE_PATH _relPath "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    foreach (_src ${ARGN})
        if (_relPath)
            list (APPEND SRCS "${_relPath}/${_src}")
        else()
            list (APPEND SRCS "${_src}")
        endif()
    endforeach()
    if (_relPath)
        # propagate to parent directory
        set (SRCS ${SRCS} PARENT_SCOPE)
    endif()
endmacro()

set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_C_COMPILER   arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-as)
set(SIZE_COMMAND       arm-none-eabi-size)
set(OBJCOPY_COMMAND    arm-none-eabi-objcopy)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_LINK_FLAGS "")
endif()

set(MAIN_TARGET        ${PROJECT_NAME}.elf)
enable_language(ASM)

message(STATUS "C compiler  : ${CMAKE_C_COMPILER}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "ASM compiler: ${CMAKE_ASM_COMPILER}")
message(STATUS "Size command: ${SIZE_COMMAND}")
message(STATUS "Main target : ${MAIN_TARGET}")

############################################################################
# Build type should be clear from here so we
# can continue with selecting include directors, defines
# and other compiler/linker flags ...
############################################################################
# include directories
include_directories(
    ${TEST_SOURCE_DIR}
    ${MBED_SRC_PATH}/
    ${MBED_SRC_PATH}/api
    ${MBED_SRC_PATH}/common
    ${MBED_SRC_PATH}/hal
    ${MBED_SRC_PATH}/targets
    ${MBED_SRC_PATH}/targets/cmsis
    ${MBED_SRC_PATH}/targets/cmsis/TARGET_STM
    ${MBED_SRC_PATH}/targets/cmsis/TARGET_STM/TARGET_STM32L0
    ${MBED_SRC_PATH}/targets/cmsis/TARGET_STM/TARGET_STM32L0/TARGET_DISCO_L053C8
    ${MBED_SRC_PATH}/targets/cmsis/TARGET_STM/TARGET_STM32L0/TARGET_DISCO_L053C8/TOOLCHAIN_GCC_ARM
    ${MBED_SRC_PATH}/targets/hal
    ${MBED_SRC_PATH}/targets/hal/TARGET_STM
    ${MBED_SRC_PATH}/targets/hal/TARGET_STM/TARGET_DISCO_L053C8
)

# Generic compiler flags
add_definitions(
    -Os
    -DTARGET_STM
    -DTARGET_STM32L0
    -DTARGET_STM32L053C8
    -DTARGET_M0P
    -D__CORTEX_M0PLUS
    -DARM_MATH_CM0PLUS
    -D__MBED__=1
    -DMBED_BUILD_TIMESTAMP=1415959335.34
    -DMBED_USERNAME=rosterloh84
)

add_definitions(
    -mcpu=cortex-m0plus
    -mthumb
    -Wall
    -Wextra
    -Wno-unused-parameter
    -Wno-missing-field-initializers
    -Wno-error=switch
    -Wno-switch
    -Wa,-adhlns=$@.lst
    -fmessage-length=0
    -fno-builtin
    -ffunction-sections
    -fdata-sections
    -fno-delete-null-pointer-checks
    -fomit-frame-pointer
    -fno-common
    -funsigned-bitfields
    -DTOOLCHAIN_GCC_ARM
    -DTOOLCHAIN_GCC
)

# Language specifc compiler flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++98 -fno-rtti -fno-exceptions -fno-threadsafe-statics")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -std=gnu99 -Wno-pointer-sign -Wno-pointer-to-int-cast")
set(CMAKE_ASM_FLAGS "${COMMON_COMPILE_FLAGS} -x assembler-with-cpp")

SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS -T${MBED_SRC_PATH}/targets/cmsis/TARGET_STM/TARGET_STM32L0/TARGET_DISCO_L053C8/TOOLCHAIN_GCC_ARM/STM32L0xx.ld)
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS} -Wl,--gc-sections -Wl,--wrap,main -Wl,-Map=${PROJECT_NAME}.map -mcpu=cortex-m0plus -mthumb --specs=nano.specs -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys")

# A macro to collect local sources into ${SRCS}.
# This variable gets propagated to the parent scope and is ultimately used in
# the top-level CMakeLists.txt to define the dependencies for the build target.
#
# Please note that files within this list are relative to the current folder.
# Please also note that this macro must be used at all CMakeLists.txt files at
# intermediate levels even if the list is empty--this is due to the Cmake magic
# involved in propagating variables to only the parent scope.
add_sources(
    main.cpp
)

# Use file globbing to collect all sources from external repositories. File-
# globbing is discouraged by CMake, except when collecting sources from an
# external source which remains mostly frozen. The risk with globbing is that
# CMake doesn't automatically update the makefiles if new sources are added to
# the globbed location.
#
file(GLOB MBED_SRC_SOURCES
    ${MBED_SRC_PATH}/common/*.c
    ${MBED_SRC_PATH}/common/*.cpp
    ${MBED_SRC_PATH}/targets/cmsis/TARGET_STM/TARGET_STM32L0/*.c
    ${MBED_SRC_PATH}/targets/cmsis/TARGET_STM/TARGET_STM32L0/TARGET_DISCO_L053C8/*.c
    ${MBED_SRC_PATH}/targets/hal/TARGET_STM/TARGET_DISCO_L053C8/*.c
)
add_sources(${MBED_SRC_SOURCES})

add_sources(${MBED_SRC_PATH}/targets/cmsis/TARGET_STM/TARGET_STM32L0/TARGET_DISCO_L053C8/TOOLCHAIN_GCC_ARM/startup_stm32l053xx.s)

# print all include directories
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
message(STATUS "Include Directories")
foreach(dir ${dirs})
  message(STATUS "  ${dir}")
endforeach()

# print all include directories
message(STATUS "Source Directories")
foreach(src ${SRCS})
  message(STATUS "  ${src}")
endforeach()

############################################################################
# By now, we've traversed all subdirectories and have collected everything that
# needs to be built. We can define the build targets.
############################################################################
# add MbedTest as a build target depending on all the sources
add_executable(${MAIN_TARGET} ${SRCS})

# Add a post-build dependency like printing size of the
# resulting binary and copying to the target.
add_custom_command(
    TARGET ${MAIN_TARGET}
    COMMAND ${SIZE_COMMAND} ${MAIN_TARGET}
    COMMAND arm-none-eabi-objcopy -O ihex ${MAIN_TARGET} ${PROJECT_NAME}.hex # convert .elf to .hex (redundancy: only one of either .hex or .bin is needed)
    COMMAND arm-none-eabi-objcopy -O binary ${MAIN_TARGET} ${PROJECT_NAME}.bin # convert .elf to .hex
)
