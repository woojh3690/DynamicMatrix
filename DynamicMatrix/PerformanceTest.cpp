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

void PerformanceTest::allocateTest()
{
	cout << "--------------- example 1 ---------------" << endl;
	clock_t start = clock();
	for (int i = 0; i < 1000; i++)
	{
		Tensor<int> tensor1({ 10, 10 });
		//double a[10][10];
	}
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
	int temp = 10;

	Tensor<int> tensor1({ temp, temp });
	tensor1[0][0] = 1;
	tensor1[0][1] = 2;

	tensor1[1][0] = 3;
	tensor1[1][1] = 4;

	tensor1[2][0] = 5;
	tensor1[2][1] = 6;

	Tensor<int> tensor2({ temp, temp });
	tensor2[0][0] = 1;
	tensor2[0][1] = 2;
	tensor2[0][2] = 3;

	tensor2[1][0] = 4;
	tensor2[1][1] = 5;
	tensor2[1][2] = 6;

	cout << "--------------- matmul test ---------------" << endl;
	clock_t start = clock();
	for (int i = 0; i < 1000; i++)
	{
		tensor1.matmul(tensor2);
	}
	cout << "run time = " << clock() - start << " milliseconds." << endl;
}

void PerformanceTest::transposeTest()
{
	Tensor<int> tensor1({ 128, 128 }, 100);

	cout << "--------------- transpose test ---------------" << endl;
	clock_t start = clock();
	for (int i = 0; i < 1000; i++)
	{
		tensor1.transpose();
	}
	cout << "run time = " << clock() - start << " milliseconds." << endl;
}

void PerformanceTest::copyTest()
{
	Tensor<int> tensor1({ 128, 128 }, 100);
	Tensor<int> tensor2;

	cout << "--------------- copy test ---------------" << endl;
	clock_t start = clock();
	for (int i = 0; i < 100; i++)
	{
		tensor2 = tensor1;
	}
	cout << "run time = " << clock() - start << " milliseconds." << endl;
}

void PerformanceTest::copyRun()
{
	Tensor<int> tensor1({ 2, 3, 4 }, 100);
	Tensor<int> tensor2;
	tensor2 = tensor1;
	cout << "tensor2 : " << tensor2 << endl;
}

void PerformanceTest::multiplyTest()
{
	Tensor<double> a({33, 33}, 2);
	Tensor<double> b({33}, 3);

	cout << "--------------- copy test ---------------" << endl;
	clock_t start = clock();
	for (int i = 0; i < 100; i++)
	{
		Tensor<double> result = (a * b);
	}
	cout << "run time = " << clock() - start << " milliseconds." << endl;
}

void PerformanceTest::selectTest()
{
	Tensor<double> a({ 3, 3 });
	a[0][0] = 0;
	a[0][1] = 1;
	a[0][2] = 2;
	a[1][0] = 3;
	a[1][1] = 4;
	a[1][2] = 5;
	a[2][0] = 6;
	a[2][1] = 7;
	a[2][2] = 8;
	Tensor<double> b({ 3, 3 } , 4);

	Tensor<bool> compare = (a <= b);

	Tensor<double> result = compare.select(a, b);

	cout << "a" << endl << a << endl;
	cout << "b" << endl << b << endl << endl;

	cout << compare << endl << endl;

	cout << result << endl;


}

void PerformanceTest::expTest()
{
	Tensor<double> a({ 3, 3 });
	a[0][0] = 0;
	a[0][1] = 1;
	a[0][2] = 2;
	a[1][0] = 3;
	a[1][1] = 4;
	a[1][2] = 5;
	a[2][0] = 6;
	a[2][1] = 7;
	a[2][2] = 8;

	cout << a << endl << endl;

	Tensor<double> temp = a.exp();
	cout << temp << endl;
}

void PerformanceTest::shapeTest()
{
	Tensor<double> a({ 33, 33 }, 2);

	cout << "--------------- copy test ---------------" << endl;
	clock_t start = clock();
	for (int i = 0; i < 1000; i++)
	{
		a.shape();
	}
	cout << "run time = " << clock() - start << " milliseconds." << endl;
}

void PerformanceTest::volumeTest()
{
	Tensor<double> a({ 33, 33 }, 2);

	cout << "--------------- copy test ---------------" << endl;
	clock_t start = clock();
	for (int i = 0; i < 1000; i++)
	{
		a.volume();
	}
	cout << "run time = " << clock() - start << " milliseconds." << endl;
}

void PerformanceTest::changeIdxTest()
{
	Tensor<double> a({ 33, 33 }, 2);

	cout << "--------------- changeIdxTest test ---------------" << endl;
	clock_t start = clock();
	for (int i = 0; i < 1000; i++)
	{
		a.changeIdxOfDim(10);
	}
	cout << "run time = " << clock() - start << " milliseconds." << endl;
}
