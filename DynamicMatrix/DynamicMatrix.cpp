#include <iostream>
#include <string>
#include "Tensor.h"
using namespace std;

#define ICOL 2
#define IROW 2
int main()
{

	Tensor<int> a({ IROW, ICOL });
	
	int count = 0;
	for (int i = 0; i < IROW; i++)
	{
		for (int j = 0; j < ICOL; j++)
		{
			a[i][j] = count++;
		}
	}

	Tensor<int> b = a;

	a.Append(b, 1);

	a = a.Reshape({ 2, 2, 2 });

	cout << a << endl;
	cout << "끝" << endl;
}