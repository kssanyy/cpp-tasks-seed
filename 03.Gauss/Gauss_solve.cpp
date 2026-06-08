#include "Gauss_solve.h"

#include <cmath>
#include <stdexcept>

GaussVector Gauss_solve(GaussMatrix &ab)
{
    const int n = ab.rows();

    if (ab.cols() != n + 1)
    {
        throw std::runtime_error("Bad augmented matrix size");
    }

    const double eps = 1e-12;

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

        for (int row = col + 1; row < n; ++row)
        {
            const double factor = ab(row, col) / ab(col, col);
            ab.row(row) -= factor * ab.row(col);
            ab(row, col) = 0.0;
        }
    }

    GaussVector result(n);
    for (int row = n - 1; row >= 0; --row)
    {
        double rhs = ab(row, n);
        for (int col = row + 1; col < n; ++col)
        {
            rhs -= ab(row, col) * result(col);
        }
        result(row) = rhs / ab(row, row);
    }

    return result;
}
