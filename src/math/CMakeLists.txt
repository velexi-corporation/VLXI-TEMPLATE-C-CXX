# =============================================================================
# `math` module
# =============================================================================

# -----------------------------------------------------------------------------
# Parameters
# -----------------------------------------------------------------------------

# --- Build parameters

# Source files
set(XYZ_MATH_SOURCE_FILES)
foreach(FILE IN ITEMS
        xyz_math.cc
       )
    list(APPEND XYZ_MATH_SOURCE_FILES "math/${FILE}")
endforeach()
set(XYZ_MATH_SOURCE_FILES ${XYZ_MATH_SOURCE_FILES} PARENT_SCOPE)

# --- Install parameters

# Header files
set(XYZ_MATH_HEADER_FILES)
foreach(FILE IN ITEMS
        xyz_math.h
       )
    list(APPEND XYZ_MATH_HEADER_FILES "math/${FILE}")
endforeach()
set(XYZ_MATH_HEADER_FILES ${XYZ_MATH_HEADER_FILES} PARENT_SCOPE)
