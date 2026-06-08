#include <gtest/gtest.h>

#include <random>
#include <stdexcept>

#include <Eigen/Dense>

#include "Gauss_solve.h"
#include "util.h"

namespace
{
GaussMatrix make_augmented(const GaussMatrix &a, const GaussVector &b)
{
    GaussMatrix ab(a.rows(), a.cols() + 1);
    ab << a, b;
    return ab;
}
}

TEST(GaussSolve, TwoByTwoSystem)
{
    GaussMatrix ab(2, 3);
    ab << 2, 1, 4,
    1, 3, 7;

    const GaussVector x = Gauss_solve(ab);

    EXPECT_NEAR(x(0), 1.0, 1e-9);
    EXPECT_NEAR(x(1), 2.0, 1e-9);
}

TEST(GaussSolve, ThreeByThreeSystem)
{
    GaussMatrix ab(3, 4);
    ab << 1, 1, 1, 6,
    2, -1, 1, 3,
    1, 2, 0, 5;

    const GaussVector x = Gauss_solve(ab);

    EXPECT_NEAR(x(0), 1.0, 1e-9);
    EXPECT_NEAR(x(1), 2.0, 1e-9);
    EXPECT_NEAR(x(2), 3.0, 1e-9);
}

TEST(GaussSolve, IdentityMatrix)
{
    GaussMatrix ab(3, 4);
    ab << 1, 0, 0, 5,
    0, 1, 0, 6,
    0, 0, 1, 7;

    const GaussVector x = Gauss_solve(ab);

    EXPECT_NEAR(x(0), 5.0, 1e-9);
    EXPECT_NEAR(x(1), 6.0, 1e-9);
    EXPECT_NEAR(x(2), 7.0, 1e-9);
}

TEST(GaussSolve, ReproducibleLargeSystem)
{
    const int n = 80;
    std::mt19937 generator(25);
    std::uniform_real_distribution<double> distribution(-1.0, 1.0);

    GaussMatrix a(n, n);
    GaussVector expected(n);

    for (int i = 0; i < n; ++i)
    {
        double row_sum = 0.0;
        for (int j = 0; j < n; ++j)
        {
            a(i, j) = distribution(generator);
            row_sum += std::abs(a(i, j));
        }
        a(i, i) += row_sum + 1.0;
        expected(i) = distribution(generator);
    }

    const GaussVector b = a * expected;
    GaussMatrix ab = make_augmented(a, b);
    const GaussVector actual = Gauss_solve(ab);

    EXPECT_LT((actual - expected).norm(), 1e-7);
}

TEST(GaussSolve, RejectsSingularMatrix)
{
    GaussMatrix ab(2, 3);
    ab << 1, 2, 3,
    2, 4, 6;

    EXPECT_THROW(Gauss_solve(ab), std::runtime_error);
}
