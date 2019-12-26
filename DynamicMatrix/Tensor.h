#pragma once
#include <vector>
using namespace std;


template <typename T>
class Tensor
{
public:
	Tensor(const vector<int> shape)
	{
		//this->shape = shape;
		if (shape.size() != 0)
		{
			vector<int> child_shape(shape.begin() + 1, shape.end());
			for (int i = 0; i < shape.front(); i++)
			{
				childLink.push_back(new Tensor<T>(child_shape));
			}
		}
	}

	Tensor(Tensor<T>& _Right)
	{
		this->~Tensor();
		vector<int> rShape = _Right.Shape();
		if (rShape.size() != 0)
		{
			for (int i = 0; i < rShape.front(); i++)
			{
				childLink.push_back(new Tensor<T>(_Right[i]));
			}
		}
		else
		{
			this->value = _Right.value;
		}
	}

	~Tensor()
	{
		for (auto mtx : this->childLink)
		{
			delete mtx;
		}
	}

private:
	//vector<int> shape;
	vector<Tensor<T>*> childLink;
	T value;

public:
	/***************************************************/
	/*                     API                         */
	/***************************************************/
	void Append(Tensor<T> tsr)
	{
		Append(tsr, 0);
	}

	void Append(Tensor<T> tsr, const int axis)
	{
		int curShape = this->Shape().size();
		int mtxShape = tsr.Shape().size();

		if (curShape > mtxShape || curShape < mtxShape)
		{
			throw length_error("Argument tensor is invalid shape!");
		}

		if (axis == 0)
		{
			for (auto clink : tsr.childLink)
			{
				this->childLink.push_back(new Tensor<T>(*clink));
			}
		}
		else
		{
			int curFront = this->Shape().front();
			int mtxFront = tsr.Shape().front();

			if (curFront != mtxFront)
				throw invalid_argument("Argument axis is invalid value!");

			for (int i = 0; i < curFront; i++)
			{
				childLink[i]->Append(tsr[i], axis - 1);
			}
		}
	}

	/***************************************************/
	/*                   operator                      */
	/***************************************************/
	Tensor<T>& operator[](const int n) {
		if (Shape().size() > 0)
		{
			if (childLink.size() < (n + 1))
				throw out_of_range("Out of range!");

			return *childLink[n];
		}
		else
		{
			throw out_of_range("Out of dimention!");
		}
	}

	Tensor<T>& operator=(const T n) {
		if (childLink.size() != 0)
		{
			throw invalid_argument("Can't allocate Rvalue to Tesnor(Lvalue)!");
		}
		value = n;
		return *this;
	}

	Tensor<T> operator=(const Tensor<T>& _Right)
	{
		return this(_Right);
	}

	operator T()
	{
		return value;
	}

	vector<int> Shape()
	{
		if (childLink.size() == 0)
		{
			return vector<int>();
		}

		vector<int> childShape = childLink[0]->Shape();
		childShape.insert(childShape.begin(), childLink.size());
		return childShape;
	}
};