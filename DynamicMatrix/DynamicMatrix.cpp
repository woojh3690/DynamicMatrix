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
	scala.append(1);
	scala.append(2);
	scala.append(3);
	scala.append(4);

	Tensor<int> matrix;
	matrix.append(scala);
	matrix.append(scala);

	Tensor<int> test = matrix + matrix;
	
	test = test.reshape({ 2, 4, 2 });
	cout << test << endl;

	matrix.erase(0);

	clock_t end = clock();
	double result = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "끝" << endl;
}