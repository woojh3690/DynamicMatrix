#include <iostream>
#include "PerformaceTest.h"
#include "Tensor.h"
#include <time.h>
#include <vector>
#include <crtdbg.h>
using namespace std;
using namespace Matrix;

int main()
{                                                  
	/*Tensor<double> temp1({ 2, 3 });
	temp1.randomInit(-5, 5);

	Tensor<double> temp2({ 3, 2 });
	temp2.randomInit(-10, 10);

	Tensor<double> a = temp1.matmul(temp2);*/

	PerformanceTest test;
	test.runTest();

	if (_CrtDumpMemoryLeaks())
	{
		printf("a");
	}
	else
	{
		printf("a");
	}
	return 0;
}
