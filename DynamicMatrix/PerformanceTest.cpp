#include "PerformaceTest.h"
#include "Tensor.h"
#include <time.h>
#include <iostream>
using namespace std;

PerformanceTest::PerformanceTest()
{
}

PerformanceTest::~PerformanceTest()
{
}

void PerformanceTest::test()
{
	cout << "--------------- example 1 ---------------" << endl;
	clock_t start = clock();
	Tensor<int> tensor1({ 100, 100, 1000 });
	cout << "run time = " << clock() - start << " milliseconds." << endl;
}

void PerformanceTest::runTest()
{
	Tensor<int> tensor1({ 3, 3, 3 });

	int value = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				tensor1[i][j][k] = value++;
			}
		}
	}

	cout << tensor1 << endl;

	cout << "--------------- example 2 ---------------" << endl;
	Tensor<int> tensor2 = tensor1;
	Tensor<int> tensor3 = tensor2.slice(1, 2);
	cout << tensor3 << endl;

	cout << "--------------- example 3 ---------------" << endl;
	Tensor<int> tensor4 = tensor3.reshape({ 3, 3, 1 });
	cout << tensor4 << endl;

	cout << "--------------- example 4 ---------------" << endl;
	tensor1.concatenate(tensor4, 2);
	cout << tensor1 << endl;

	cout << "--------------- example 5 ---------------" << endl;
	tensor1.concatenate(tensor4, 2);
	cout << tensor1 << endl;
}

void PerformanceTest::matmulTest()
{
	Tensor<int> tensor1({ 64, 2 });
	tensor1[0][0] = 1;
	tensor1[0][1] = 2;
	tensor1[0][2] = 1;
	tensor1[1][0] = 0;
	tensor1[1][1] = 1;
	tensor1[1][2] = 0;
	tensor1[2][0] = 2;
	tensor1[2][1] = 3;
	tensor1[2][2] = 4;

	Tensor<int> tensor2({ 2, 64 });
	tensor2[0][0] = 2;
	tensor2[0][1] = 5;
	tensor2[1][0] = 6;
	tensor2[1][1] = 7;
	tensor2[2][0] = 1;
	tensor2[2][1] = 8;

	cout << "--------------- matmul test ---------------" << endl;
	clock_t start = clock();
	for (int i = 0; i < 1000; i++)
	{
		tensor1.matmul(tensor2);
	}
	cout << "run time = " << clock() - start << " milliseconds." << endl;
}
