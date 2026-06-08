#pragma once

#include <Eigen/Dense>
#include <ostream>

using GaussVector = Eigen::VectorXd;
using GaussMatrix = Eigen::Matrix<double, -1, -1, Eigen::RowMajor>;

GaussMatrix load_csv_to_matrix(const char *filename);
void print_matrix_as_csv(std::ostream& out, const GaussMatrix &matrix, int prec = 6);
