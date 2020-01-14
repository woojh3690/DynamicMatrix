#include <iostream>
#include <string>
#include "Tensor.h"
using namespace std;

#define ICOL 2
#define IROW 2
int main()
{
	Tensor<int> scala;
	scala.Append(1);
	scala.Append(2);
	scala.Append(3);
	scala.Append(4);

	Tensor<int> matrix;
	matrix.Append(scala, 0);
	matrix.Append(scala, 0);

	Tensor<int> test = matrix + matrix;

	cout << test << endl;
	cout << "끝" << endl;
}