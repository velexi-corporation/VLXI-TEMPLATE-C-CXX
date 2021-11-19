# =============================================================================
# XYZ::xyz library
# =============================================================================

# -----------------------------------------------------------------------------
# Build
# -----------------------------------------------------------------------------

# --- Subdirectories

# math
add_subdirectory(math)

# --- Targets

add_library(xyz
            ${XYZ_MATH_SOURCE_FILES}
)
target_include_directories(xyz PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/math>
)
target_link_directories(xyz PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_LIBDIR}>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/lib>
)

# -----------------------------------------------------------------------------
# Install
# -----------------------------------------------------------------------------

# Library files
install(TARGETS xyz
    EXPORT ${PKG_NAME}Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Header files
install(FILES
        ${XYZ_MATH_HEADER_FILES}
        DESTINATION ${INSTALL_INCLUDEDIR}
)

# CMake files
install(EXPORT ${PKG_NAME}Targets
        FILE ${PKG_NAME}Targets.cmake
        NAMESPACE ${PKG_NAME}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PKG_NAME}
)

# -----------------------------------------------------------------------------
# Exports
# -----------------------------------------------------------------------------

# Create CMake script to support exporting from the build tree
export(EXPORT ${PKG_NAME}Targets
       FILE "${CMAKE_BINARY_DIR}/cmake/${PKG_NAME}Targets.cmake"
       NAMESPACE ${PKG_NAME}::
)
