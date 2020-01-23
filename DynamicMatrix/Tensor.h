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
		if (!shape.empty())
		{
			vector<int> child_shape(shape.begin() + 1, shape.end());
			for (int i = 0; i < shape.front(); i++)
			{
				childLink.push_back(new Tensor<T>(child_shape));
			}
			//throw invalid_argument("The Shape size is zero!");
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
		concatenate(item, 0);
	}

	void append(Tensor<T>& tsr)
	{
		Tensor<T>* newTsr = new Tensor<T>(tsr);
		childLink.push_back(newTsr);
	}

	void concatenate(Tensor<T>& tsr, const int axis)
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
				childLink[i]->concatenate(tsr[i], axis - 1);
			}
		}
	}

	Tensor<T>& reshape(vector<int> newShape)
	{
		int fillShapeIndex = -1;
		int newShapeLen = 1;
		for (int i = 0; i < newShape.size(); i++)
		{
			if (newShape[i] != -1)
			{
				newShapeLen *= newShape[i];
			}
			else
			{
				fillShapeIndex = i;
			}
		}

		vector<int> curShape = this->shape();
		int curShapeLen = 1;
		for (int item : curShape)
			curShapeLen *= item;

		if (fillShapeIndex != -1)
		{
			if (curShapeLen % newShapeLen == 0)
			{
				int newDimShape = curShapeLen / newShapeLen;
				newShape[fillShapeIndex] = newDimShape;
				newShapeLen = newShapeLen * newDimShape;
			}
			else
			{
				throw invalid_argument("Cannot reshape array of size.");
			}
		}
		else
		{
			if (newShapeLen != curShapeLen)
			{
				string msgShape = "(";
				for (int item : newShape)
					msgShape += to_string(item) + ", ";
				msgShape.replace(msgShape.size() - 2, msgShape.size(), ")");
				throw invalid_argument("Cannot reshape array of size " + to_string(curShapeLen) + " into shape " + msgShape);
			}
		}

		Tensor<T>* newTsr = new Tensor(newShape);
		for (int i = 0; i < curShapeLen; i++)
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
		if (childLink.empty())
		{
			return vector<int>();
		}

		vector<int> curShape = childLink[0]->shape();
		curShape.insert(curShape.begin(), childLink.size());
		return curShape;
	}

	string toString()
	{
		string result = "[";
		int shapeSize = this->shape().size();
		int resultSize = result.size();

		if (shapeSize == 1)
		{
			for (auto child : this->childLink)
			{
				result += to_string(*child) + ", ";
			}
			result.replace(resultSize - 2, resultSize, "]");
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
			result.replace(resultSize - shapeSize, resultSize, "]");
		}
		
		return result;
	}

	/***************************************************/
	/*                   operator                      */
	/***************************************************/
	Tensor<T>& operator[](const int n) 
	{
		return *childLink[n];
	}

	Tensor<T>& operator[](const vector<int> idxs) 
	{
		if (!idxs.empty())
		{
			vector<int> child_idxs(idxs.begin() + 1, idxs.end());
			return childLink[idxs.front()]->operator[](child_idxs);
		}
		return *this;
	}

	Tensor<T>& operator=(const T n) 
	{
		if (!childLink.empty())
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
		if (rShape.empty())
		{
			this->value = tsr.value;
		}
		else
		{
			for (int i = 0; i < rShape.front(); i++)
			{
				childLink.push_back(new Tensor<T>(tsr[i]));
			}
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
		if (!childLink.empty())
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