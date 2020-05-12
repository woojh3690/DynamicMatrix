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
	// 텐서 생성
	int row = 100;
	int col = 4;
	Tensor<double> vec({ row, col });
	vec.randomInit(0.0, 1000.0);

	for (auto item : vec)
	{
		std::cout << item << std::endl;
	}

	std::sort(vec.begin(), vec.end(), [](Tensor<double> a, Tensor<double> b) { 
		return a[0].value() < b[0].value();
	});

	std::cout << "-----------------------------------" << std::endl;
	for (auto item : vec)
	{
		std::cout << item << std::endl;
	}

	return 0;
}
