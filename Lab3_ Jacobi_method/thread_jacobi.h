#pragma once
#include <vector>

/**
 * @class thread_jacobi
 * @brief Multi-threaded implementation of the Jacobi method.
 */
class thread_jacobi {
private:
    std::vector<std::vector<double>> A;      ///< Coefficient matrix A
    std::vector<double> B;                   ///< Vector of free terms B
    std::vector<double> x_res;               ///< Solution vector
    int size;                                ///< Size of matrix and vectors

public:
    /**
     * @brief Constructor that initializes matrix A and vector B.
     * @param a Coefficient matrix A
     * @param b Vector of free terms B
     */
    thread_jacobi(const std::vector<std::vector<double>>& a, const std::vector<double>& b);

    /**
     * @brief Solves the system using the Jacobi iterative method.
     * @param tolerance Desired tolerance for convergence
     */
    void jacobi_method(double tolerance);

    /**
     * @brief Returns the computed solution vector.
     * @return Constant reference to the solution vector X
     */
    const std::vector<double>& Get_x() const;
};


