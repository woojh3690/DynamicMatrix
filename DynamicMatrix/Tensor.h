#pragma once
#include <vector>
#include <algorithm>
using namespace std;


template <typename T>
class Tensor
{
public:
	Tensor(const vector<int> shape)
	{
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
		this->operator=(_Right);
	}

	~Tensor()
	{
		for (auto mtx : this->childLink)
		{
			delete mtx;
		}
	}

private:
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

	Tensor<T>& Reshape(const vector<int> rshape)
	{
		Tensor<T>* newTsr = new Tensor(rshape);
		vector<int> curShape = this->Shape();

		// TODO 조건 검사
		
		int size = 1;
		for (int shape : curShape)
			size *= shape;

		for (int i = 0; i < size; i++)
		{
			T value = this->operator[](ChangeDim(i, curShape));
			newTsr->operator[](ChangeDim(i, rshape)) = value;
		}

		return *newTsr;
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

	Tensor<T>& operator[](const vector<int> idxs) {
		if (idxs.size() != 0)
		{
			vector<int> child_idxs(idxs.begin() + 1, idxs.end());
			return childLink[idxs.front()]->operator[](child_idxs);
		}
		return *this;
	}

	Tensor<T>& operator=(const T n) {
		if (childLink.size() != 0)
		{
			throw invalid_argument("Can't allocate Rvalue to Tesnor(Lvalue)!");
		}
		value = n;
		return *this;
	}

	Tensor<T>* operator=(Tensor<T>& _Right)
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

		return this;
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

	string ToString()
	{
		string result = "{";
		vector<int> shape = this->Shape();

		int len = 1;
		for (auto shape : shape)
		{
			len *= shape;
			result += to_string(shape) + ", ";
		}
		result.replace(result.size() - 2, result.size(), "}\n");

		for (int i = 0; i < len; i++)
		{
			vector<int> index = ChangeDim(i, shape);
			Tensor<T> tensor = this->operator[](index);
			string data = to_string(tensor);
			result += data + ", ";
		}

		return result;
	}

private:
	vector<int> ChangeDim(int index, vector<int> mshape)
	{
		std::reverse(mshape.begin(), mshape.end());

		vector<int> matrixIdx;
		int idx = 0;
		int Dn = 1;
		int multi = 1;
		int alpha = 0;
		for (int i = 0; i < mshape.size(); i++)
		{
			alpha += idx * multi;
			multi *= Dn;
			Dn = mshape[i];
			idx = ((index - alpha) / multi) % Dn;
			matrixIdx.push_back(idx);
		}

		std::reverse(matrixIdx.begin(), matrixIdx.end());
		return matrixIdx;
	}
};