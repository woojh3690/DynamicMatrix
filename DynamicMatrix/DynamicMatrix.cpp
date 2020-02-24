#include <iostream>
#include <string>
#include "Tensor.h"
#include "PerformaceTest.h"
#include <time.h>
using namespace std;

int main()
{
	PerformanceTest test;
	test.matmulTest();


	
	/*Tensor<int> tensor1({ 3, 3 });
	tensor1[0][0] = 1;
	tensor1[0][1] = 2;
	tensor1[0][2] = 1;
	tensor1[1][0] = 0;
	tensor1[1][1] = 1;
	tensor1[1][2] = 0;
	tensor1[2][0] = 2;
	tensor1[2][1] = 3;
	tensor1[2][2] = 4;
	cout << tensor1 << endl << endl;


	Tensor<int> tensor2({ 3, 2 });
	tensor2[0][0] = 2;
	tensor2[0][1] = 5;
	tensor2[1][0] = 6;
	tensor2[1][1] = 7;
	tensor2[2][0] = 1;
	tensor2[2][1] = 8;
	cout << tensor2 << endl << endl;

	Tensor<int> result = tensor1.matmul(tensor2);

	cout << result << endl;*/

	
	printf("end");
}