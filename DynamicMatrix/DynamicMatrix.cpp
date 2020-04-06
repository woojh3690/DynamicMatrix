#include <iostream>
#include "PerformaceTest.h"
#include "include/Tensor.h"
#include "include/CSVToTsrReader.h"
#include <time.h>
#include <vector>
#include <crtdbg.h>
using namespace std;
using namespace Matrix;


void mainCode()
{
	PerformanceTest test;
	test.runTest();
}

int main()
{                     
	//_CrtSetBreakAlloc(172);
	mainCode();
	_CrtDumpMemoryLeaks();
	return 0;
}
