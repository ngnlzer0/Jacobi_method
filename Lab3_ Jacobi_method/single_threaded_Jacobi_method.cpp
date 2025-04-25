#include "single_threaded_Jacobi_method.h"

#include <iostream>
#include <cmath>

void single_threaded_Jacobi_method::decoupling()
{
	D.resize(SIZE);
	for (int i = 0; i < SIZE; ++i) {
		D[i].resize(SIZE, 0.0);
	}
	for (int i = 0; i < SIZE;i++)
	{
		D[i][i] = A[i][i];
	}

	R.resize(SIZE);
	for (int i = 0; i < SIZE; i++)
	{
		R[i].resize(SIZE);
		for (int j = 0; j < SIZE; j++)
		{
			if (i != j)
				R[i][j] = A[i][j];
			else
				R[i][j] = 0;
		}
	}
}

double single_threaded_Jacobi_method::SumR(int i,std::vector<double>& x)
{
	double SUM = 0;		
	for (int j = 0; j<SIZE; j++)
	{
		SUM += R[i][j] * x[j];
	}
	return SUM;
}

single_threaded_Jacobi_method::single_threaded_Jacobi_method(std::vector<std::vector<double>> a,std::vector<double> b, int size)
	:A(a),B(b),SIZE(size)
{
	X.resize(SIZE, -1);

}

void single_threaded_Jacobi_method::Main_method(double epsilo)
{

	if (!Check_working_method())
		return;
	std::vector<double> x_prev(SIZE,0.0);
	std::vector<double> x_cur(SIZE, (0.0));
	bool done = false;

	decoupling();
	int k = 0;

	do
	{
		//std::cout << "Number iteration = " << k << std::endl;
		done = true;
		for (int i = 0; i < SIZE;i++)
		{
			double x_temp = x_cur[i];
			if (D[i][i] == 0) {
				std::cerr << "Error: Division by zero at row " << i << std::endl;
				return;
			}
			x_cur[i] = (1.0 / D[i][i]) * (B[i] - SumR(i, x_prev));
			x_prev[i] = x_temp;
			if (fabs(x_cur[i] - x_prev[i]) > epsilo) done = false;
		}

		for (int i = 0; i < SIZE; ++i)
			X[i] = x_cur[i];

		//k++;
	} while (!done);
}

void single_threaded_Jacobi_method::ShowX()
{
	for (int i = 0; i < SIZE; i++)
		std::cout << "x" << i << " = " << X[i] << "; " << std::endl;
	std::cout << std::endl;
}

bool single_threaded_Jacobi_method::Check_working_method()
{
	for (int i=0;i< SIZE;i++)
	{
		double sum_ij = 0.0;

		for (int j = 0; j < SIZE; j++)
		{
			if (i != j)
				sum_ij += abs(A[i][j]);
		}
		if (abs(A[i][i]) < sum_ij)
		{
			//std::cout << " Matrix A is not diagonally dominant " << std::endl;
			return false;
		}
	}
	//std::cout << " Matrix A diagonally dominant " << std::endl;
	return true;
}
