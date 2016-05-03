#include "mpi.h"
#include <stdio.h>
#include <iostream>
#include <ctime>
double** dataGenA( int const len) {
	double** data= new double*[len];
	//srand(time(NULL));
	for (int i = 0; i < len; i++)
	{
		data[i] = new double[len];
		for (int j = 0; j < len; j++)
		{
			data[i][j] = (rand()%1000-500)+0.001*(rand() % 1001);
			//std::cout << data[i][j] << " ";
		}
		//std::cout << "\n";
	}
	return data;
}
double* dataGenB(int len,double** dataA) {
	double* b = new double[len];
	for(int i=0;i<len;i++)
	{
		b[i] = 0;
		for (int j = 0; j < len; j++)
		{
			b[i] += (j+1) * dataA[i][j];
		}
	}
	return b;
}
int main(int argc, char *argv[])
{
	int const n = 3000;
	double** a = dataGenA(n);
	double* x =new double[n] { 0 };
	double* b = dataGenB(n, a);

	auto start = clock();
	#pragma omp for
	for (int k = 0; k < n; k++)
	{
		for (int j = k+1; j < n; j++)
		{
			double left = a[j][k] / a[k][k];
			b[j] -= left * b[k];
			for (int i = 0; i < n; i++)
			{
				a[j][i] -= left * a[k][i];
			}
		}
	}
	#pragma omp for 
	for (int i = n - 1; i > -1; i--)
	{
		//parallel j
		//#pragma omp for
		for (int j = n-1; j >i ; j--)
		{
			b[i] -= a[i][j] * b[j];
		}
		b[i] /= a[i][i];
		a[i][i] = 1;
		x[i] = b[i];
	}
	auto fin = clock() - start;
	for (int i=0;i<n;i++)
	{
		std::cout << x[i]<<"\n";
	}
	std::cout << "time is:" << static_cast<double>( fin)/CLK_TCK << "\n";
	getchar();
}

	


