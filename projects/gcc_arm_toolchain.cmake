#Check http://www.cmake.org/Wiki/CMake_Cross_Compiling
INCLUDE(CMakeForceCompiler)

# Targeting an embedded system, no OS.
set(CMAKE_SYSTEM_NAME Generic)

# Tell we want to cross-compile
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_GENERATOR "Unix Makefiles")

# type of compiler we want to use
#set(COMPILER_PATH "C:/ProgramData/chocolatey/lib/gcc-arm-embedded.4.8.2014.3/tools")
#set(COMPILER_PATH "C:/Program Files (x86)/GNU Tools ARM Embedded/4.8 2014q1")
#set(COMPILER_TYPE arm-none-eabi)

# Specify the cross compiler, linker, etc.
#set(CMAKE_C_COMPILER	  ${COMPILER_PATH}/bin/${COMPILER_TYPE}-gcc.exe)
#set(CMAKE_CXX_COMPILER	${COMPILER_PATH}/bin/${COMPILER_TYPE}-g++.exe)
#set(CMAKE_ASM_COMPILER  ${COMPILER_PATH}/bin/${COMPILER_TYPE}-as.exe)
#set(CMAKE_ASM		        ${COMPILER_PATH}/bin/${COMPILER_TYPE}-as)
#set(CMAKE_LINKER	      ${COMPILER_PATH}/bin/${COMPILER_TYPE}-ld)
#set(CMAKE_OBJCOPY	      ${COMPILER_PATH}/bin/${COMPILER_TYPE}-objcopy)
#set(CMAKE_SIZE		      ${COMPILER_PATH}/bin/${COMPILER_TYPE}-size)
#set(CMAKE_NM		        ${COMPILER_PATH}/bin/${COMPILER_TYPE}-nm)

# There is a bug in CMAKE_OBJCOPY, it doesn't exist on execution for the first time
#set(CMAKE_OBJCOPY_OVERLOAD ${COMPILER_PATH}/bin/${COMPILER_TYPE}-objcopy)

# specify the cross compiler
CMAKE_FORCE_C_COMPILER(arm-none-eabi-gcc GNU)
CMAKE_FORCE_CXX_COMPILER(arm-none-eabi-g++ GNU)
set(CMAKE_ASM_COMPILER arm-none-eabi-as)

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

message("Compiling for TARGET: ${CMAKE_SYSTEM_PROCESSOR} ${CMAKE_SYSTEM_NAME} using ${CMAKE_GENERATOR} CC:${CMAKE_CROSSCOMPILING}")
message("on HOST: ${CMAKE_HOST_SYSTEM_NAME} ${CMAKE_HOST_SYSTEM_VERSION} ${CMAKE_HOST_SYSTEM_PROCESSOR} ${CMAKE_HOST_SYSTEM}")
#-------------------------------------------------------------------------------
# define presets
set(TOOLCHAIN TOOLCHAIN_GCC)

set(USE_RTOS false)
set(USE_NET  false)
set(USE_USB  false)
set(USE_DSP  false)

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
