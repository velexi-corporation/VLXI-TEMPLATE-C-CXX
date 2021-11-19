//
// Unit tests for `xyz::math` module
//
// -----------------------------------------------------------------------------------------
// COPYRIGHT/LICENSE. This file is part of the {{ PKG_NAME }} package. It is subject to the
// license terms in the LICENSE file found in the root directory of this distribution. No
// part of the {{ PKG_NAME }} package, including this file, may be copied, modified,
// propagated, or distributed except according to the terms contained in the LICENSE file.
// -----------------------------------------------------------------------------------------

#include "gtest/gtest-message.h"    // for Message
#include "gtest/gtest-test-part.h"  // for TestPartResult
#include "gtest/gtest_pred_impl.h"  // for AssertionResult, SuiteApiResolver

#include "xyz_config.h"
#include "xyz_math.h"

/*
 * Test fixtures
 */
class XYZMathTest : public ::testing::Test {
  protected:
    // Data members
    XYZ_REAL x;
};

/*
 * Tests
 */
TEST_F(XYZMathTest, add_one) {
    // Preparations
    x = 3;

    // Exercise functionality and check results
    EXPECT_NEAR(add_one(x), 4, 1e-6);
}

TEST_F(XYZMathTest, is_approx_zero) {
    // Exercise functionality and check results
    x = 1e-15;
    EXPECT_TRUE(is_approx_zero(x));

    x = 1;
    EXPECT_FALSE(is_approx_zero(x));
}
