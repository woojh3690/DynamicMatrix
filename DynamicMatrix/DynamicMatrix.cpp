#include <iostream>
//#include <string>
#include "Tensor.h"
//#include "PerformaceTest.h"
//#include <time.h>
using namespace std;

int main()
{
	//PerformanceTest test;
	//test.matmulTest();

	Tensor<double> temp1({ 2, 2 }, 2);
	Tensor<double> temp2({ 2, 2 }, 3);
	Tensor<double> temp3({ 1 }, 4);

	Tensor<double> result1 = 0.01 + temp1;
	Tensor<bool> result2 = temp1 > temp2;
	Tensor<double> result3 = temp1 * temp2;
	Tensor<double> result4 = temp1.matmul(temp2);

	Tensor<double> result5 = temp3 * temp1;

	printf("end");
} 