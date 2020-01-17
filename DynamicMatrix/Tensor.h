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

	Tensor(vector<int> shape)
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
	friend ostream& operator<<(ostream& os, Tensor<NODETYPE>& dt);

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

	vector<int> changeDim(int index, vector<int> mshape)
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

public:
	/***************************************************/
	/*                     API                         */
	/***************************************************/
	void append(T value)
	{
		Tensor<T> item(value);
		append(item, 0);
	}

	void append(Tensor<T>& tsr)
	{
		Tensor<T>* newTsr = new Tensor<T>(tsr);
		childLink.push_back(newTsr);
	}

	void append(Tensor<T>& tsr, const int axis)
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
			int curFront = this->shape().front();
			int mtxFront = tsr.shape().front();

			if (curFront != mtxFront)
				throw invalid_argument("Argument axis is invalid value!");

			for (int i = 0; i < curFront; i++)
			{
				childLink[i]->append(tsr[i], axis - 1);
			}
		}
	}

	Tensor<T>& reshape(vector<int> newShape)
	{
		Tensor<T>* newTsr = new Tensor(newShape);
		vector<int> curShape = this->shape();

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
				msgShape += to_string(item) + ", ";
			msgShape.replace(msgShape.size() - 2, msgShape.size(), ")");
			throw invalid_argument("Cannot reshape array of size " + to_string(curShapeLen) + " into shape " + msgShape);
		}
		
		int size = 1;
		for (int shape : curShape)
			size *= shape;

		for (int i = 0; i < size; i++)
		{
			T value = this->operator[](changeDim(i, curShape));
			newTsr->operator[](changeDim(i, newShape)) = value;
		}

		return *newTsr;
	}

	Tensor<T>& slice(const int start)
	{
		return slice(start, childLink.size());
	}

	Tensor<T>& slice(const int start, const int end)
	{
		Tensor<T>* newTsr = new Tensor<T>;

		for (int i = start; i < end; i++)
		{
			Tensor<T> childTsr(*this->childLink[i]);
			newTsr->append(childTsr);
		}

		return *newTsr;
	}

	void erase(const int index)
	{
		delete childLink[index];
		childLink.erase(childLink.begin() + index);
	}

	vector<int> shape()
	{
		if (childLink.size() == 0)
		{
			return vector<int>();
		}

		vector<int> childShape = childLink[0]->shape();
		childShape.insert(childShape.begin(), childLink.size());
		return childShape;
	}

	string toString()
	{
		string result = "[";
		int shapeSize = this->shape().size();

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
				result += child->toString() + "," + enter;
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
		if (shape().size() > 0)
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

	Tensor<T>& operator=(Tensor<T>& tsr)
	{
		this->~Tensor();
		vector<int> rShape = tsr.shape();
		if (rShape.size() != 0)
		{
			for (int i = 0; i < rShape.front(); i++)
			{
				childLink.push_back(new Tensor<T>(tsr[i]));
			}
		}
		else
		{
			this->value = tsr.value;
		}

		return *this;
	}

	Tensor<T>& operator+(Tensor<T>& tsr)
	{
		Tensor<T>* empty = new Tensor<T>;
		empty->append(*this, 0);
		empty->append(tsr, 0);
		return *empty;
	}

	operator T()
	{
		if (childLink.size() != 0)
		{
			throw invalid_argument("Tensor can't cast to Rvalue.");
		}
		return value;
	}

};

template <typename NODETYPE>
ostream & operator<<(ostream & os, Tensor<NODETYPE>& tsr)
{
	return os << tsr.toString();
}