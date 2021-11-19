//
// Header file for `xyz::math` module
//
// -----------------------------------------------------------------------------------------
// COPYRIGHT/LICENSE. This file is part of the {{ PKG_NAME }} package. It is subject to the
// license terms in the LICENSE file found in the root directory of this distribution. No
// part of the {{ PKG_NAME }} package, including this file, may be copied, modified,
// propagated, or distributed except according to the terms contained in the LICENSE file.
// -----------------------------------------------------------------------------------------

#include "xyz_config.h"

/*
 * add_one(x)
 *
 * Return `x + 1`.
 */
XYZ_REAL add_one(XYZ_REAL x);

/*
 * is_approx_zero(x)
 *
 * Return whether `x` is approximately zero.
 */
bool is_approx_zero(XYZ_REAL x);
