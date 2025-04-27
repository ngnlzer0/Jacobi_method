#pragma once
#include <vector>

/**
 * @class single_threaded_Jacobi_method
 * @brief Single-threaded implementation of the Jacobi iterative method.
 *        Approximates solutions for systems of linear equations.
 */
class single_threaded_Jacobi_method {
protected:
    std::vector<std::vector<double>> A;      ///< Coefficient matrix A
    std::vector<double> X;                   ///< Solution vector X
    std::vector<double> B;                   ///< Vector of free terms B
    std::vector<std::vector<double>> R;      ///< Matrix R with diagonal elements from A
    std::vector<std::vector<double>> D;      ///< Matrix D with off-diagonal elements from A
    int SIZE;                                ///< Number of unknowns

    /**
     * @brief Splits matrix A into matrices R (diagonal) and D (off-diagonal).
     */
    void decoupling();

    /**
     * @brief Calculates the sum of R[i][j] * x[j] for a given row i.
     * @param i Index of the row
     * @param x Current approximation of the solution
     * @return Sum of products for the ith row
     */
    double SumR(int i, std::vector<double>& x);

public:
    /**
     * @brief Default constructor.
     */
    single_threaded_Jacobi_method();

    /**
     * @brief Parameterized constructor.
     * @param a Coefficient matrix A
     * @param b Vector of free terms B
     * @param size Size of the system
     */
    single_threaded_Jacobi_method(std::vector<std::vector<double>> a, std::vector<double> b, int size);

    /**
     * @brief Performs the Jacobi iterative algorithm to solve the system.
     * @param epsilon Desired accuracy
     */
    virtual void Main_method(double epsilon);

    /**
     * @brief Displays the current solution vector X.
     */
    void ShowX();

    /**
     * @brief Checks if matrix A is diagonally dominant.
     * @return True if the matrix is diagonally dominant, false otherwise.
     */
    bool Check_working_method();

    /**
     * @brief Gets the value of X[i].
     * @param i Index in the solution vector
     * @return Value at index i
     */
    double Get_Xi(int i) { return X[i]; };
};
