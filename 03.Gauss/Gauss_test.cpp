#include <gtest/gtest.h>

#include "Gauss_solve.h"
#include "util.h"

TEST(GaussSolve, TwoByTwoSystem)
{
    GaussMatrix ab(2, 3);
    ab << 2, 1, 4,
          1, 3, 7;

    GaussVector x = Gauss_solve(ab);

    EXPECT_NEAR(x(0), 1.0, 1e-6);
    EXPECT_NEAR(x(1), 2.0, 1e-6);
}

TEST(GaussSolve, ThreeByThreeSystem)
{
    GaussMatrix ab(3, 4);
    ab << 1, 1, 1, 6,
          2, -1, 1, 3,
          1, 2, 0, 5;

    GaussVector x = Gauss_solve(ab);

    EXPECT_NEAR(x(0), 1.0, 1e-6);
    EXPECT_NEAR(x(1), 2.0, 1e-6);
    EXPECT_NEAR(x(2), 3.0, 1e-6);
}

TEST(GaussSolve, IdentityMatrix)
{
    GaussMatrix ab(3, 4);
    ab << 1, 0, 0, 5,
          0, 1, 0, 6,
          0, 0, 1, 7;

    GaussVector x = Gauss_solve(ab);

    EXPECT_NEAR(x(0), 5.0, 1e-6);
    EXPECT_NEAR(x(1), 6.0, 1e-6);
    EXPECT_NEAR(x(2), 7.0, 1e-6);
}

TEST(GaussSolve, RequiresPivoting)
{
    GaussMatrix ab(2, 3);
    ab << 0, 2, 4,
          1, 1, 3;

    GaussVector x = Gauss_solve(ab);

    EXPECT_NEAR(x(0), 1.0, 1e-6);
    EXPECT_NEAR(x(1), 2.0, 1e-6);
}
