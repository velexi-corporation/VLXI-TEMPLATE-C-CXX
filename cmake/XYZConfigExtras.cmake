# =============================================================================
# XYZConfigExtras.cmake
# =============================================================================

# Set CMake-available package variables
get_property(XYZ_INCLUDE_DIRS
             TARGET XYZ::xyz
             PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
get_property(XYZ_LIB_DIRS
             TARGET XYZ::xyz
             PROPERTY INTERFACE_LINK_DIRECTORIES)
