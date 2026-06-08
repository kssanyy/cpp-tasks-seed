#include "Gauss_solve.h"
#include "util.h"

#include <fstream>
#include <iostream>

int main(int argc, const char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " input.csv [output.csv]\n";
        return 1;
    }

    const char *output_filename = argc > 2 ? argv[2] : "solution.csv";

    try
    {
        GaussMatrix ab = load_csv_to_matrix(argv[1]);
        GaussVector x = Gauss_solve(ab);
        GaussMatrix result(x.rows(), 1);
        result.col(0) = x;

        std::ofstream out(output_filename);
        if (!out)
        {
            std::cerr << "Cannot open output file\n";
            return 1;
        }

        print_matrix_as_csv(out, result, 6);
    }
    catch (const std::exception &error)
    {
        std::cerr << error.what() << '\n';
        return 1;
    }

    return 0;
}
