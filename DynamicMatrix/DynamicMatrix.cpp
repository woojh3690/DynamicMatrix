#include <iostream>
#include "Matrix.h"

int main()
{
	for (int i = 0; i < 1000; i++)
	{
		Matrix<int> test(vector<int> {10, 10});
		test[0] = 1;
		test[0][1] = 2;

		Matrix<int> copyTest = test;
		std::cout << copyTest[0][1] << std::endl;
		std::cout << "Hello World! : " << i << std::endl;
	}
}