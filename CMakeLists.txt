# =============================================================================
# Main CMakeLists.txt
# =============================================================================

# -----------------------------------------------------------------------------
# CMake Configuration
# -----------------------------------------------------------------------------

# --- Initialization

cmake_minimum_required(VERSION 3.10)
project(XYZ
        LANGUAGES C CXX
        VERSION 0.1.0)

# --- Configuration parameters

# Build type
set(CMAKE_BUILD_TYPE Release CACHE STRING "CMake build type")

# Compiler requirements
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# -----------------------------------------------------------------------------
# Build Options
# -----------------------------------------------------------------------------

# Build shared libraries
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# -----------------------------------------------------------------------------
# Imported Modules
# -----------------------------------------------------------------------------

# --- CMake modules

# Packaging
include(CMakePackageConfigHelpers)

# Make cache variables for install destinations
include(GNUInstallDirs)

# --- External CMake-compatible packages

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

# -----------------------------------------------------------------------------
# Parameters
# -----------------------------------------------------------------------------

# --- Build parameters

# Package name
set(PKG_NAME ${CMAKE_PROJECT_NAME})

# Construct name to use for package-specific directories (e.g., header files)
execute_process(
    COMMAND "echo" ${PKG_NAME}
    COMMAND "tr" '[:upper:]' '[:lower:]'
    OUTPUT_VARIABLE get_project_name_lower_out
    RESULT_VARIABLE get_project_name_lower_result_var
    )
if (NOT "${get_project_name_lower_result_var}" STREQUAL 0)
    message(FATAL_ERROR "Error constructing directory name for package")
endif (NOT "${get_project_name_lower_result_var}" STREQUAL 0)
string(STRIP ${get_project_name_lower_out} PKG_DIR_NAME)

# Set path variables
set(BINARY_INCLUDEDIR ${CMAKE_BINARY_DIR}/include)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(EXT_PROJECTS_DIR ext)

# Compiler flags

# --- Install parameters

# Set directory (relative) for installed header files
set(INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR}/${PKG_DIR_NAME})

# -----------------------------------------------------------------------------
# Build
# -----------------------------------------------------------------------------

# --- Process template files

# Generate configuration header files
configure_file(include/xyz_config.h.in
               ${BINARY_INCLUDEDIR}/xyz_config.h)

# Copy CMake configuration files
file(COPY ${CMAKE_SOURCE_DIR}/cmake/${PKG_NAME}ConfigExtras.cmake
     DESTINATION ${CMAKE_BINARY_DIR}/cmake
)

# --- Targets

# Libraries
add_subdirectory(src)

# CMake configuration files
add_custom_target(cmake-config-files ALL
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/cmake/${PKG_NAME}ConfigExtras.cmake
    ${CMAKE_BINARY_DIR}/cmake/${PKG_NAME}ConfigExtras.cmake
)

# -----------------------------------------------------------------------------
# Test
# -----------------------------------------------------------------------------

# Enable CMake testing
enable_testing()

# GoogleTest
add_subdirectory(${EXT_PROJECTS_DIR}/googletest EXCLUDE_FROM_ALL)

# Tests
add_subdirectory(tests EXCLUDE_FROM_ALL)

# -----------------------------------------------------------------------------
# Package
# -----------------------------------------------------------------------------

# Create package configuration file
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/${PKG_NAME}Config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PKG_NAME}
)

# Create package version file
set_property(TARGET xyz PROPERTY VERSION ${XYZ_VERSION})
set_property(TARGET xyz PROPERTY
             INTERFACE_XYZ_MAJOR_VERSION ${XYZ_VERSION_MAJOR})
set_property(TARGET xyz APPEND PROPERTY
    COMPATIBLE_INTERFACE_STRING XYZ_MAJOR_VERSION
)
write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/cmake/${PKG_NAME}ConfigVersion.cmake
    VERSION "${version}"
    COMPATIBILITY AnyNewerVersion
)

# Create source code distribution
if (GIT_FOUND)
    # Add target to build source code distribution
    set(ARCHIVE_NAME ${PKG_NAME}-${PROJECT_VERSION})
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

# -----------------------------------------------------------------------------
# Install
# -----------------------------------------------------------------------------

# --- Files

# Configuration header file
install(FILES ${BINARY_INCLUDEDIR}/xyz_config.h
        DESTINATION ${INSTALL_HEADER_PATH})

# CMake packaging files
install(FILES
        ${CMAKE_BINARY_DIR}/cmake/${PKG_NAME}Config.cmake
        ${CMAKE_BINARY_DIR}/cmake/${PKG_NAME}ConfigVersion.cmake
        ${CMAKE_BINARY_DIR}/cmake/${PKG_NAME}ConfigExtras.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PKG_NAME})

# --- `uninstall`

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
