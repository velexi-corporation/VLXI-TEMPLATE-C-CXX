# ----------------------------------------------------------------------
# CMake initialization
# ----------------------------------------------------------------------

cmake_minimum_required(VERSION 3.10)
project(XYZ
        LANGUAGES C CXX
        VERSION 0.1.0)

# ----------------------------------------------------------------------
# Configuration options
# ----------------------------------------------------------------------

# Build shared libraries
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# CMake build type
set(CMAKE_BUILD_TYPE Release CACHE STRING "CMake build type")

# ----------------------------------------------------------------------
# Build parameters
# ----------------------------------------------------------------------

# Compiler flags
set(CMAKE_C_FLAGS "${CFLAGS} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

# Set path variables
set(BINARY_INCLUDE_DIR ${CMAKE_BINARY_DIR}/include)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)

# Set search paths for header files
include_directories(${BINARY_INCLUDE_DIR})
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# ----------------------------------------------------------------------
# Install parameters
# ----------------------------------------------------------------------

# Set root directory (relative) for installed header files
execute_process(
    COMMAND "echo" ${CMAKE_PROJECT_NAME}
    COMMAND "tr" '[:upper:]' '[:lower:]'
    OUTPUT_VARIABLE get_project_name_lower_out
    RESULT_VARIABLE get_project_name_lower_result_var
    )

if (NOT "${get_project_name_lower_result_var}" STREQUAL 0)
    message(FATAL_ERROR "Unable to root directory for installed header files")
endif (NOT "${get_project_name_lower_result_var}" STREQUAL 0)
#string(STRIP include/${get_project_name_lower_out} INSTALL_INCLUDE_DIR)
set(INSTALL_INCLUDE_DIR include)

# Set root directory (relative) for installed library files
set(INSTALL_LIB_DIR lib)

# ----------------------------------------------------------------------
# Build requirements
# ----------------------------------------------------------------------

# --- Compiler requirements

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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

# ----------------------------------------------------------------------
# Build targets
# ----------------------------------------------------------------------

# library
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

# examples
add_subdirectory(examples EXCLUDE_FROM_ALL)

# ----------------------------------------------------------------------
# Testing
# ----------------------------------------------------------------------

# Enable CMake testing
enable_testing()

# Add googletest directory to build
add_subdirectory(${EXT_PROJECTS_DIR}/googletest)

# Add test directory to build
add_subdirectory(${PROJECT_SOURCE_DIR}/test)

# ----------------------------------------------------------------------
# Installation
# ----------------------------------------------------------------------

# Note: installation of header files, library files, and executables
#       should be handled by the CMakeLists.txt files included by the
#       add_subdirectory() command.

# --- Add `uninstall` target

# Generate cmake_uninstall.cmake file
configure_file(
    "${CMAKE_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY
)

# Define `uninstall` target
add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_uninstall.cmake
)

# ----------------------------------------------------------------------
# Source code distribution
# ----------------------------------------------------------------------

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

# ----------------------------------------------------------------------
# Build preparations
# ----------------------------------------------------------------------

# --- Create build directories

file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
file(MAKE_DIRECTORY ${LIBRARY_OUTPUT_PATH})

# --- Process template files

# Generate configuration header files
configure_file(config/xyz_config.h.in
               ${BINARY_INCLUDE_DIR}/xyz_config.h)
