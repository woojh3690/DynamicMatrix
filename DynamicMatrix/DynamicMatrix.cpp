#include <iostream>
#include <string>
#include "Tensor.h"
using namespace std;

#define ICOL 1
#define IROW 2
int main()
{
	Tensor<int> a({10, 10, 10});
	a[ICOL][IROW][2] = 1;
	Tensor<int> b({10, 10, 10});
	

	a.Append(b);
	Tensor<int> copytest = a;
	int strtemp = copytest[ICOL][IROW][2];
	std::cout << strtemp << std::endl;

	int temp = copytest[{ICOL, IROW, 2}];

	Tensor<int> test2 = a.Reshape({ 10, 20, 10 });

	//a.ChangeDim(10, { 10, 20 });
	cout << "끝" << endl;
}