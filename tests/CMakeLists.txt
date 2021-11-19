# =============================================================================
# XYZ Tests
# =============================================================================

# -----------------------------------------------------------------------------
# Imports
# -----------------------------------------------------------------------------

# XYZ
add_library(XYZ::xyz STATIC IMPORTED)
set_property(TARGET XYZ::xyz PROPERTY
             IMPORTED_LOCATION "${LIBRARY_OUTPUT_PATH}/libxyz.a"
)

# GoogleTest
include(googletest)

# -----------------------------------------------------------------------------
# Functions
# -----------------------------------------------------------------------------

function(add_test_target TEST_PROGRAM SOURCE_FILES)
    add_executable(${TEST_PROGRAM} ${SOURCE_FILES})
    add_dependencies(${TEST_PROGRAM} XYZ::xyz googletest)
    target_include_directories(${TEST_PROGRAM} PUBLIC ${GTEST_INCLUDE_DIRS})
    target_link_directories(${TEST_PROGRAM} PUBLIC ${GTEST_LIB_DIRS})
    target_link_libraries(
        ${TEST_PROGRAM}
        PRIVATE xyz
        gtest
        gtest_main
    )
endfunction()

# -----------------------------------------------------------------------------
# Tests
# -----------------------------------------------------------------------------

# Component tests
add_subdirectory(math)

# Custom `tests` target to build test programs
add_custom_target(tests DEPENDS
                  math-tests)
