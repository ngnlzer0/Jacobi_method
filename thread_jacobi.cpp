#include "thread_jacobi.h"
#include "utils.hpp"
#include <omp.h>
#include <cmath>
#include <algorithm>

thread_jacobi::thread_jacobi(const std::vector<std::vector<double>>& a, const std::vector<double>& b)
    : A(a), B(b), size(a.size()) {
}

void thread_jacobi::jacobi_method(double tolerance) {
    std::vector<double> x(size, 0.0);
    std::vector<double> x_new(size, 0.0);
    int iter = 0;

    omp_set_num_threads(8); // you can change the number of threads

    bool converged = false;

    while (!converged) {
        double error = 0.0;
        converged = true;  

        //std::cout << "Number iteration = " << iter << std::endl;

#pragma omp parallel for reduction(+:error)
        for (int i = 0; i < size; ++i) {
            x_new[i] = solution_find(A[i], x, B[i], i);
            error += std::abs(x_new[i] - x[i]);
        }

        // After all the values of x_new are calculated, check for convergence
        for (int i = 0; i < size; ++i) {
            if (std::fabs(x_new[i] - x[i]) > tolerance) {
                converged = false;
                break;  
            }
        }

        std::swap(x, x_new);
        //++iter;
    }

    x_res = x;
}


const std::vector<double>& thread_jacobi::Get_x() const {
    return x_res; // x — це твій вектор у класі, переконайся що він існує
}








