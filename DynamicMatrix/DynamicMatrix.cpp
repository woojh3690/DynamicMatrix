#include <iostream>
#include <string>
#include "Tensor.h"
#include "PerformaceTest.h"
#include <time.h>
using namespace std;

int main()
{
	//PerformanceTest test;
	//test.copyRun();
	//test.transposeTest();

	Tensor<int> a({100, 33, 33}, 2);
	Tensor<int> b({100, 33, 33}, 3);
	Tensor<bool> result = (a <= b);

	cout << "a" << endl << a << endl;
	cout << "b" << endl << b << endl << endl;

	cout << result << endl;
	printf("end");
}