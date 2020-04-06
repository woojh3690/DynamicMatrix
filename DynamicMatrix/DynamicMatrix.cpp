#include <iostream>
#include "PerformaceTest.h"
#include "include/Tensor.h"
#include <time.h>
#include <vector>
#include <crtdbg.h>
using namespace std;
using namespace Matrix;

void checkMemorLeak()
{
	if (_CrtDumpMemoryLeaks())
	{
		printf("a");
	}
	else
	{
		printf("a");
	}
}

int main()
{                                                  
	PerformanceTest test;
	test.runTest();
	checkMemorLeak();
	return 0;
}
