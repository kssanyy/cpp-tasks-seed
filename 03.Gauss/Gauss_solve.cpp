#include "Gauss_solve.h"

#include <cmath>
#include <stdexcept>

GaussVector Gauss_solve(GaussMatrix &ab)
{
    const int n = static_cast<int>(ab.rows());
    const int m = static_cast<int>(ab.cols());
    const double eps = 1e-12;

    if (n == 0 || m != n + 1)
    {
        throw std::runtime_error("Invalid augmented matrix size");
    }

    for (int col = 0; col < n; ++col)
    {
        int pivot = col;
        for (int row = col + 1; row < n; ++row)
        {
            if (std::abs(ab(row, col)) > std::abs(ab(pivot, col)))
            {
                pivot = row;
            }
        }

        if (std::abs(ab(pivot, col)) < eps)
        {
            throw std::runtime_error("Singular matrix");
        }

        if (pivot != col)
        {
            ab.row(col).swap(ab.row(pivot));
        }

        const double pivot_value = ab(col, col);
        ab.row(col) /= pivot_value;

        for (int row = 0; row < n; ++row)
        {
            if (row != col)
            {
                const double factor = ab(row, col);
                ab.row(row) -= factor * ab.row(col);
            }
        }
    }

    return ab.col(n);
}
