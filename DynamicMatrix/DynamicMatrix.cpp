#include <iostream>
#include <string>
#include "Tensor.h"
#include "MatrixData.h"
#include <time.h>
using namespace std;

#define ROW_SIZE 100000000
/*Tensor<int> matrix;
	matrix.append(scala);
	matrix.append(scala);

	Tensor<int> test = matrix + matrix;
	
	test = test.reshape({ 40, 100, 100 });
	cout << test << endl;

	matrix.erase(0);*/
int main()
{
	string temp;

	clock_t start = clock();
	Tensor<string> scala;
	for (int i = 0; i < ROW_SIZE; i++)
	{
		scala.append("a");
	}
	for (int i = 0; i < ROW_SIZE; i++)
	{
		temp = scala[i];
	}
	double tensorReuslt = (double)(clock() - start) / CLOCKS_PER_SEC;
	cout << tensorReuslt << endl;
	//-----------------------------------------------------------

	start = clock();
	MatrixData table;
	table.noOfRow = ROW_SIZE;
	table.noOfCol = 1;
	table.MallocData();
	for (int i = 0; i < ROW_SIZE; i++)
	{
		table.data[i][0] = "a";
	}

	for (int i = 0; i < ROW_SIZE; i++)
	{
		temp = table.data[i][0];
	}
	double MatrixResult = (double)(clock() - start) / CLOCKS_PER_SEC;
	cout << MatrixResult << endl;

	cout << "끝" << endl;
}