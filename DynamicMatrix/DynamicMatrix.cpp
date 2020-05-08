#include <iostream>
#include "include/Tensor.h"
#include <time.h>
#include <vector>
#include <crtdbg.h>
#include "PerformaceTest.h"
using namespace std;
using namespace KDTLAB;


int main()
{
	int row = 3;
	int col = 4;
	Tensor<int> vec({ row, col });
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			vec[i][j] = i * col + j;
		}
	}

	for (auto item : vec)
	{
		std::cout << item << std::endl;
	}

	return 0;
}
