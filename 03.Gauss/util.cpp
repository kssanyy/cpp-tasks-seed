#include "util.h"

#include <iomanip>
#include <lazycsv.hpp>
#include <stdexcept>
#include <string>
#include <vector>

GaussMatrix load_csv_to_matrix(const char *filename)
{
    std::vector<std::vector<double>> values;
    lazycsv::parser parser{ filename };

    for (const auto row : parser)
    {
        std::vector<double> current_row;
        bool numeric_row = true;

        for (const auto cell : row)
        {
            try
            {
                current_row.push_back(std::stod(std::string(cell.raw())));
            }
            catch (const std::invalid_argument &)
            {
                numeric_row = false;
                break;
            }
        }

        if (numeric_row && !current_row.empty())
        {
            values.push_back(current_row);
        }
    }

    if (values.empty())
    {
        throw std::runtime_error("CSV file does not contain numeric rows");
    }

    const int rows = static_cast<int>(values.size());
    const int cols = static_cast<int>(values[0].size());
    GaussMatrix matrix(rows, cols);

    for (int i = 0; i < rows; ++i)
    {
        if (static_cast<int>(values[i].size()) != cols)
        {
            throw std::runtime_error("CSV rows have different sizes");
        }

        for (int j = 0; j < cols; ++j)
        {
            matrix(i, j) = values[i][j];
        }
    }

    return matrix;
}

void print_matrix_as_csv(std::ostream& out, const GaussMatrix &matrix, int prec)
{
    out << std::fixed << std::setprecision(prec);

    for (int i = 0; i < matrix.rows(); ++i)
    {
        for (int j = 0; j < matrix.cols(); ++j)
        {
            if (j != 0)
            {
                out << ',';
            }
            out << matrix(i, j);
        }
        out << '\n';
    }
}
