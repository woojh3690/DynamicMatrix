#include <iostream>
#include "PerformaceTest.h"
#include "Tensor.h"
#include <time.h>
#include <vector>
using namespace std;
using namespace Matrix;

struct Shape
{
	int* shape;
	int len;
};

int main()
{
	/*PerformanceTest test;
	test.copyTest();
	printf("end");*/                                                       
	Tensor<double> temp({ 2, 3 });
	temp.randomInit(-5, 5);

	return 0;
}



