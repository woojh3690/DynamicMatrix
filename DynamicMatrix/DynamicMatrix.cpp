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
	int row = 2;
	Tensor<int> vec1({ row, 4 });
	vec1.randomInit(0, 1000);

	Tensor<int> vec2({ row, 1 });
	vec2.randomInit(0, 1000);

	auto temp = vec1.concatenate(vec2);


	return 0;
}
