#pragma once
#include <vector>
#include <chrono>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>

typedef unsigned long ulong;

/**
 * @brief Calculates next approximation of X[i] using Jacobi formula.
 * @param row Row from matrix A (coefficients)
 * @param solutions Previous solution vector X^(k)
 * @param term Value of B[i]
 * @param index Index of the variable being solved
 * @return Updated value of X[i]^(k+1)
 */
inline double solution_find(const std::vector<double>& row, const std::vector<double>& solutions, double term, const ulong index)
{
#pragma simd
    for (int j = 0; j < row.size(); ++j) {
        term -= (solutions[j] * row[j]);
    }
    return (term + (solutions[index] * row[index])) / row[index];
}
/**
 * @brief Prints the solution vector to console.
 * @param solution The solution vector
 */
inline void print_solution(const std::vector<double>& solution) {
    for (int i = 0; i < solution.size(); ++i) {
        std::cout << "X[" << i << "] = " << solution[i] << std::endl;
    }
}
/**
 * @brief Generates a random value in a given range.
 * @tparam T Type of value
 * @param min Minimum value
 * @param max Maximum value
 * @return Random value of type T
 */
template<typename T>
T rand_t(T min, T max) {
    T range = max - min;
    T r = static_cast<T>(rand()) / static_cast<T>(RAND_MAX);
    return r * range + min;
}

/**
 * @brief Fills a vector with random values.
 * @tparam T Type of vector elements
 * @param size Size of the vector
 * @param v Vector to be filled
 * @param min Minimum value
 * @param max Maximum value
 */
template<typename T>
void generate_vector(const ulong size, std::vector<T>& v, const T min, const T max) {
    for (ulong i = 0; i < size; ++i) {
        v.emplace_back(rand_t(min, max));
    }
}
	
/**
 * @brief Generates a diagonally dominant matrix.
 * @tparam T Type of matrix elements
 * @param size Dimension of the matrix
 * @param matrix Matrix to fill
 * @param min Minimum value
 * @param max Maximum value
 */
template<typename T>
void generate_diagonal_dominant_matrix(const ulong size, std::vector<std::vector<T>>& matrix,
    const T min, const T max) {
    for (ulong i = 0; i < size; ++i) {
        std::vector<T> row;
        generate_vector(size, row, min, max);
        matrix.emplace_back(row);
    }

    for (ulong i = 0; i < size; ++i) {
        T sum = T(0);
        for (const auto& val : matrix[i]) {
            sum += std::abs(val);
        }
        sum -= std::abs(matrix[i][i]);
        matrix[i][i] = std::abs(matrix[i][i]) + sum; // ensure diagonal dominance
    }
}

/**
 * @brief Calculates total absolute error of Ax - b.
 * @tparam T Type of numeric values
 * @param matrix Matrix A
 * @param terms Vector B
 * @param solution Solution vector X
 * @return Total absolute error
 */
template<typename T>
T check_error(const std::vector<std::vector<T>>& matrix, const std::vector<T>& terms, const std::vector<T>& solution) {
    T error = T(0);
    for (int i = 0; i < matrix.size(); ++i) {
        T diff = terms[i];
        for (int j = 0; j < matrix[i].size(); ++j) {
            diff -= matrix[i][j] * solution[j]; // compute Ax - b
        }
        error += std::abs(diff);
    }
    return error;
}