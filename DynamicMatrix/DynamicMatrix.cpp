#include <iostream>
#include <string>
#include "Tensor.h"
#include <time.h>
using namespace std;

#define ICOL 2
#define IROW 2
int main()
{
	clock_t start = clock();

	Tensor<int> scala;
	for (int i = 0; i < 100000; i++)
	{
		scala.append(i);
	}

	Tensor<int> matrix;
	matrix.append(scala);
	matrix.append(scala);

	Tensor<int> test = matrix + matrix;
	
	test = test.reshape({ 40, 100, 100 });
	cout << test << endl;

	matrix.erase(0);

	clock_t end = clock();
	double result = (double)(end - start) / CLOCKS_PER_SEC;
	cout << result << endl;
}