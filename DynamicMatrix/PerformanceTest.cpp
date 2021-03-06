#include "PerformaceTest.h"
#include "include/Tensor.h"
#include <time.h>
#include <iostream>
using namespace std;
using namespace KDTLAB;

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
		Tensor<int> tensor1({ 10, 10 }, 0);
		//double a[10][10];
	}
	cout << "run time = " << clock() - start << " milliseconds." << endl;
}

void PerformanceTest::runTest()
{
	allocateTest();
	matmulTest();
	transposeTest();
	copyTest();
	copyRun();
	multiplyTest();
	selectTest();
	expTest();
	shapeTest();
	volumeTest();
	SplitTest();
}

void PerformanceTest::matmulTest()
{
	int len = 10;

	Tensor<int> tensor1({ len, len }, 0);
	tensor1[0][0] = 1;
	tensor1[0][1] = 2;

	tensor1[1][0] = 3;
	tensor1[1][1] = 4;

	tensor1[2][0] = 5;
	tensor1[2][1] = 6;

	Tensor<int> tensor2({ len, len }, 0);
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
	for (int i = 0; i < 10; i++)
	{
		tensor1.transpose();
	}
	cout << "run time = " << clock() - start << " milliseconds." << endl;
}

void PerformanceTest::copyTest()
{
	Tensor<int> tensor1({ 13824, 6 }, 100);
	Tensor<int> tensor2;
	cout << "--------------- copy test ---------------" << endl;
	clock_t start = clock();
	for (int i = 0; i < 10; i++)
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
		Tensor<double> result = (a + b);
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

void PerformanceTest::SplitTest()
{
	Tensor<double> a({ 33, 33}, 2);

	cout << "--------------- Split test ---------------" << endl;
	clock_t start = clock();

	for (int i = 0; i < 1000; i++)
	{
		vector<Tensor<double>> result = a.split(4, 1);
	}
	cout << "run time = " << clock() - start << " milliseconds." << endl;
}
