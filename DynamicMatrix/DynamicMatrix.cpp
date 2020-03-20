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

	Tensor<double> result = 0.01 + temp1;
	printf("end");
} 