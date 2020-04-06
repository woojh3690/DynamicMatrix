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
	csv::CSVFormat format;
	format.delimiter(',').column_names({ "a", "b", "c" });
	Matrix::CSVToTsrReader reader("test.csv", format);
	Tensor<double> a = reader.to_tensor<double>();
}

int main()
{                     
	//_CrtSetBreakAlloc(172);
	mainCode();
	_CrtDumpMemoryLeaks();
	return 0;
}
