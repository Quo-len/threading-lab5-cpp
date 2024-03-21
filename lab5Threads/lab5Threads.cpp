#include<iostream>
#include"omp.h"

const int rows = 1000;
const int cols = 100000;

int arr[rows][cols];

void init_arr() {
	srand(time(0));
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			arr[i][j] = rand() % 100;
		}
	}
}

long long get_sum(int num_threads) {
	long long sum = 0;
	double t1 = omp_get_wtime();

#pragma omp parallel num_threads(num_threads)
	{
#pragma omp for reduction(+:sum)
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				sum += arr[i][j];
			}
		}
	}

	double t2 = omp_get_wtime();
	std::cout << "threads used: " << num_threads << " in sum - " << t2 - t1 << " seconds" << std::endl;

	return sum;
}

int get_min_row(int num_threads) {
	int min_sum = INT_MAX;
	int min_row = -1;
	double t1 = omp_get_wtime();

#pragma omp parallel for num_threads(num_threads)
	for (int i = 0; i < rows; i++) {
		long long row_sum = 0;
		for (int j = 0; j < cols; j++) {
			row_sum += arr[i][j];
		}
		if (row_sum < min_sum) {
#pragma omp critical
			{
				if (row_sum < min_sum) {
					min_sum = row_sum;
					min_row = i;
				}
			}
		}
	}
	double t2 = omp_get_wtime();
	std::cout << "min sum - " << min_sum << " threads used: " << num_threads << " in min - " << t2 - t1 << " seconds" << std::endl;

	return min_row;
}

int main() {
	int max_thread = 32;
	int step = 2;
	init_arr();

	omp_set_nested(1);
	double t1 = omp_get_wtime();
#pragma omp parallel sections
	{
#pragma omp section
		{
			for (int i = 1; i <= max_thread; i *= step)
			{
				std::cout << "sum " << i << " = " << get_sum(i) << std::endl;
			}
		}
#pragma omp section
		{
			for (int i = 1; i <= max_thread; i *= step)
			{
				std::cout << "min row " << i << " = " << get_min_row(i) << std::endl;
			}
		}
	}
	double t2 = omp_get_wtime();
	std::cout << "Total time - " << t2 - t1 << " seconds" << std::endl;

	return 0;
}