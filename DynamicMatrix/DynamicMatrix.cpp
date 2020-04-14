#include <iostream>
#include "include/Tensor.h"
#include <time.h>
#include <vector>
#include <crtdbg.h>
//#include "2dMatrix/Matrix.h"
using namespace std;
using namespace KDTLAB;


int main()
{ 
	Tensor<> a({ 3,3 }, 4);
	Tensor<> b({ 3,3 }, 2);


	/*Tensor<> result1 = a + b;
	Tensor<> result2 = a - b;
	Tensor<> result3 = a * b;
	Tensor<> result4 = a / b;*/

	Tensor<> result = a.concatenate(b, 1);

	return 0;
}
