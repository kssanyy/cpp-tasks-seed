#include "util.h"

#include <cctype>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
bool parse_csv_row(const std::string &line, std::vector<double> &row)
{
    std::stringstream ss(line);
    std::string cell;
    row.clear();

    while (std::getline(ss, cell, ','))
    {
        const std::size_t begin = cell.find_first_not_of(" \t\r\n");
        if (begin == std::string::npos)
        {
            return false;
        }

        const std::size_t end = cell.find_last_not_of(" \t\r\n");
        const std::string trimmed = cell.substr(begin, end - begin + 1);

        std::size_t parsed = 0;
        try
        {
            const double value = std::stod(trimmed, &parsed);
            if (parsed != trimmed.size())
            {
                return false;
            }
            row.push_back(value);
        }
        catch (const std::exception &)
        {
            return false;
        }
    }

    return !row.empty();
}
}

GaussMatrix load_csv_to_matrix(const char *filename)
{
    std::ifstream in(filename);
    if (!in)
    {
        throw std::runtime_error("Cannot open input CSV file");
    }

    std::vector<std::vector<double>> rows;
    std::string line;
    std::size_t columns = 0;

    while (std::getline(in, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::vector<double> row;
        if (!parse_csv_row(line, row))
        {
            if (rows.empty())
            {
                continue;
            }
            throw std::runtime_error("Invalid numeric CSV row");
        }

        if (columns == 0)
        {
            columns = row.size();
        }
        else if (row.size() != columns)
        {
            throw std::runtime_error("CSV rows have different sizes");
        }

        rows.push_back(row);
    }

    if (rows.empty())
    {
        throw std::runtime_error("Input CSV does not contain numeric data");
    }

    GaussMatrix matrix(static_cast<int>(rows.size()), static_cast<int>(columns));
    for (int i = 0; i < matrix.rows(); ++i)
    {
        for (int j = 0; j < matrix.cols(); ++j)
        {
            matrix(i, j) = rows[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)];
        }
    }

    return matrix;
}

void print_vector_as_csv(std::ostream &out, const GaussVector &vector, int prec)
{
    out << "x\n";
    out << std::fixed << std::setprecision(prec);

    for (int i = 0; i < vector.size(); ++i)
    {
        out << vector(i) << '\n';
    }
}
