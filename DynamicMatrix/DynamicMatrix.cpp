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

	string temp = { 1, 2, 3 };

	vector<int> mother = { 2,3,4,10 };

	Tensor<double> a(mother, 10);
	string strValue = a.toString();

	Tensor<double> b;
	b.loadFromString(strValue);

	return 0;
}



