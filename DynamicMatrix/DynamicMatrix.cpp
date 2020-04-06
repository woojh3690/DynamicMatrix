#include <iostream>
#include "PerformaceTest.h"
#include "include/Tensor.h"
#include "include/CSVToTsrReader.h"
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
	csv::CSVFormat format;
	format.delimiter(',').column_names({ "a", "b", "c" });
	Matrix::CSVToTsrReader reader("test.csv");
	Tensor<double> a = reader.to_tensor<double>();

	checkMemorLeak();
	return 0;
}
