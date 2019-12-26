#include <iostream>
#include <string>
#include "Tensor.h"
using namespace std;

#define ICOL 1
#define IROW 2
int main()
{
	while (true)
	{
		Tensor<string> a(vector<int> {100, 100, 3});
		Tensor<string> b(vector<int> {100, 100, 3});
		a[ICOL][IROW][2] = "hi";

		a.Append(b, 1);
		Tensor<string> copyTest(a);
		string strTemp = copyTest[ICOL][IROW][2];
		std::cout << strTemp.c_str() << std::endl;
	}
}