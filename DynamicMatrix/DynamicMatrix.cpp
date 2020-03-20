#include <iostream>
#include <string>
#include "Tensor.h"
#include "PerformaceTest.h"
#include <time.h>
using namespace std;

int main()
{
	PerformanceTest test;
	test.matmulTest();

	printf("end");
} 