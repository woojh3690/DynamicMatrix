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
	Tensor<int> vec({ 3, 4 });
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			vec[i][j] = i * 4 + j;
		}
	}

	for (auto item : vec)
	{
		std::cout << item << std::endl;
	}
	return 0;
}
