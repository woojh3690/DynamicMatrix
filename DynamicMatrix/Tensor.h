#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


template <typename T>
class Tensor
{
public:
	Tensor()
	{
	}

	Tensor(const T value)
	{
		Tensor<T>* item = new Tensor<T>;
		item->value = value;
		this->childLink.push_back(item);
	}

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

	template <typename NODETYPE>
	friend ostream& operator<<(ostream& os, const Tensor<NODETYPE>& dt);

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
	void Append(T value)
	{
		Tensor<T> item(value);
		Append(item);
	}

	void Append(Tensor<T> tsr)
	{
		Append(tsr, 0);
	}

	void Append(Tensor<T> tsr, const int axis)
	{
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

	Tensor<T>& Reshape(const vector<int> newShape)
	{
		Tensor<T>* newTsr = new Tensor(newShape);
		vector<int> curShape = this->Shape();

		// TODO 조건 검사
		int newShapeLen = 1;
		for (int item : newShape)
		{
			newShapeLen *= item;
		}

		int curShapeLen = 1;
		for (int item : curShape)
		{
			curShapeLen *= item;
		}
		
		if (newShapeLen != curShapeLen)
		{
			string msgShape = "(";
			for (int item : newShape)
			{
				msgShape += to_string(item) + ", ";
			}
			int msgSize = msgShape.size();
			msgShape.replace(msgSize - 2, msgSize, ")");

			throw invalid_argument("Cannot reshape array of size " + to_string(curShapeLen) + " into shape " + msgShape);
		}
		
		int size = 1;
		for (int shape : curShape)
			size *= shape;

		for (int i = 0; i < size; i++)
		{
			T value = this->operator[](ChangeDim(i, curShape));
			newTsr->operator[](ChangeDim(i, newShape)) = value;
		}

		return *newTsr;
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
		string result = "[";
		int shapeSize = this->Shape().size();

		if (shapeSize == 1)
		{
			for (auto child : this->childLink)
			{
				result += to_string(*child) + ", ";
			}
			result.replace(result.size() - 2, result.size(), "]");
		}
		else
		{
			string enter;
			for (int i = 0; i < shapeSize - 1; i++)
			{
				enter += "\n";
			}
			for (auto child : this->childLink)
			{
				result += child->ToString() + "," + enter;
			}
			result.replace(result.size() - shapeSize, result.size(), "]");
		}
		
		return result;
	}

	/***************************************************/
	/*                   operator                      */
	/***************************************************/
	Tensor<T>& operator[](const int n) 
	{
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

	Tensor<T>& operator[](const vector<int> idxs) 
	{
		if (idxs.size() != 0)
		{
			vector<int> child_idxs(idxs.begin() + 1, idxs.end());
			return childLink[idxs.front()]->operator[](child_idxs);
		}
		return *this;
	}

	Tensor<T>& operator=(const T n) 
	{
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
		if (childLink.size() != 0)
		{
			throw invalid_argument("Tensor can't cast to Rvalue.");
		}
		return value;
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

template <typename NODETYPE>
ostream & operator<<(ostream & os, Tensor<NODETYPE>& tsr)
{
	return os << tsr.ToString();
}