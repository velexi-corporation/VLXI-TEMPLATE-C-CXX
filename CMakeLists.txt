# --------------------------------------------------------------
# CMake initialization
# --------------------------------------------------------------

cmake_minimum_required(VERSION 3.5)
project(PKG
        LANGUAGES C CXX
        VERSION 0.1.0)

# --------------------------------------------------------------
# Configuration options
# --------------------------------------------------------------

# Build shared libraries
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# CMake build type
set(CMAKE_BUILD_TYPE Release CACHE STRING "CMake build type")

# --------------------------------------------------------------
# Build parameters
# --------------------------------------------------------------

# Path variables
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)
set(BINARY_INCLUDE_DIR ${CMAKE_BINARY_DIR}/include)

# Compiler flags
set(CMAKE_C_FLAGS "${CFLAGS} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)

# Search path for header files
include_directories(${BINARY_INCLUDE_DIR})

# CMake options
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# --------------------------------------------------------------
# Build requirements
# --------------------------------------------------------------

# --- Software with pre-defined CMake package modules

find_package(Threads REQUIRED)
find_package(Git)

# --- include-what-you-use

find_program(IWYU NAMES include-what-you-use iwyu)

if (IWYU)
    message("-- Found include-what-you-use: ${IWYU}")
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${IWYU} ${CMAKE_CXX_FLAGS})
else (IWYU)
    message("-- Could not find include-what-you-use.  "
            "Analysis of #includes will be skipped.")
endif (IWYU)

# --------------------------------------------------------------
# Process template files
# --------------------------------------------------------------

# Generate configuration header files
configure_file(config/PKG_config.h.in
               ${BINARY_INCLUDE_DIR}/PKG_config.h)

# --------------------------------------------------------------
# Build targets
# --------------------------------------------------------------

# library
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

# examples
add_subdirectory(examples EXCLUDE_FROM_ALL)

# --------------------------------------------------------------
# Testing
# --------------------------------------------------------------

# Enable CMake testing
enable_testing()

# Emulate 'make check'
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

# Add googletest directory to build
add_subdirectory(${EXT_PROJECTS_DIR}/googletest)

# Add test directory to build
add_subdirectory(${PROJECT_SOURCE_DIR}/test)

# --------------------------------------------------------------
# Installation
# --------------------------------------------------------------

# Header files
#file(GLOB HEADER_FILES ${SRCDIR}/*.h)
#install(FILES ${HEADER_FILES} DESTINATION include)

# Library files
#install(TARGETS lsm_parallel DESTINATION lib)

# Executable files
# TODO

# --- Add `uninstall` target

# Generate cmake_uninstall.cmake file
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY
)

# Define `uninstall` target
add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND}
            -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

# --------------------------------------------------------------
# Source code distribution
# --------------------------------------------------------------

if (GIT_FOUND)
    # Add target to build source code distribution
    set(ARCHIVE_NAME ${CMAKE_PROJECT_NAME}-${PROJECT_VERSION})
    set(ARCHIVE_TARBALL ${ARCHIVE_NAME}.tar.gz)
    add_custom_target(dist
        COMMAND git archive --prefix=${ARCHIVE_NAME}/ HEAD |
                gzip > ${CMAKE_BINARY_DIR}/${ARCHIVE_TARBALL}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )

    # Add target to verify source code distribution
    if (UNIX)
        file(GENERATE OUTPUT check-uninstall.sh
             CONTENT "
remaining=\"`find \${PWD}/${ARCHIVE_NAME}/build/_inst -type f | wc -l`\"
if test \"\${remaining}\" -ne 0; then
    echo \"** Stage directory not empty: \${remaining} file (s) remaining!\"
    exit 1
fi"
        )

        add_custom_target(distcheck
            COMMAND rm -rf ${ARCHIVE_NAME}
            COMMAND gzip -cd ${ARCHIVE_TARBALL} | tar xfz -
            COMMAND cd ${ARCHIVE_NAME} && cmake -H. -Bbuild
            COMMAND cd ${ARCHIVE_NAME}/build && make
            COMMAND cd ${ARCHIVE_NAME}/build && make test
            COMMAND cd ${ARCHIVE_NAME}/build && make DESTDIR=$${PWD}/_inst install
            COMMAND cd ${ARCHIVE_NAME}/build && make DESTDIR=$${PWD}/_inst uninstall
            COMMAND sh check-uninstall.sh
            COMMAND cd ${ARCHIVE_NAME}/build && make clean
            COMMAND rm -rf ${ARCHIVE_NAME}
            COMMAND echo "*** Package ${ARCHIVE_TARBALL} ready for distribution."
        )
        add_dependencies(distcheck dist)

    endif (UNIX)
endif (GIT_FOUND)

# --------------------------------------------------------------
# Build preparations
# --------------------------------------------------------------

# Create build output directories
file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
file(MAKE_DIRECTORY ${LIBRARY_OUTPUT_PATH})
