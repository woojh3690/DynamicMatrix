#pragma once
#include <cstdarg>
#include <vector>
#include <algorithm>
using namespace std;

template <typename T>
class Matrix
{
public:
	Matrix(vector<int> shape)
	{
		this->shape = shape;
		if (shape.size() != 0)
		{
			vector<int> child_shape(shape.begin() + 1, shape.end());
			for (int i = 0; i < shape.front(); i++)
			{
				child_link.push_back(new Matrix<T>(child_shape));
			}
		}
	}

	~Matrix()
	{
		for (auto mtx : this->child_link)
		{
			delete mtx;
		}
	}

private:
	vector<Matrix<T>*> child_link;
	vector<int> shape;
	T value;

public:
	Matrix<T>& operator[](const int n) {
		if (shape.size() > 0)
		{
			return *child_link[n];
		}
		else
		{
			throw out_of_range("Out of dimention!");
		}
	}

	Matrix<T>& operator=(const T n) {
		value = n;
		return *this;
	}

	Matrix<T>& operator=(const Matrix<T> _Right)
	{
		_Right;
	}

	operator T()
	{
		return value;
	}
};