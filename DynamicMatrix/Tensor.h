#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <omp.h>
#include <Windows.h>
using namespace std;

#define MAKE_COMPARE_OPERATOR(sign) \
Tensor<bool>& operator##sign##(Tensor<T>& compare) \
{ \
	if (compare.size() == 1) \
	{ \
		Tensor<T> a(this->shape(), compare.operator[](0)); \
		compare = a; \
	} \
	else if (compare.size() != this->size()) \
	{ \
		throw invalid_argument("RValue size must be (1) or " + \
			this->strShape() + "shape."); \
	} \
 \
	Tensor<bool>* boolTsr = new Tensor<bool>(compare.shape()); \
 \
	vector<int> curShape = this->shape(); \
	vector<int> idx; \
	for (int i = 0; i < this->volume(); i++) \
	{ \
		idx = changeDim(i, curShape); \
		T curValue = this->operator[](idx); \
		T compareValue = compare[idx]; \
		bool boolValue = curValue ##sign## compareValue; \
		boolTsr->operator[](idx) = boolValue; \
	} \
 \
	return *boolTsr; \
}

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
		item->m_value = value;
		this->m_childLink.push_back(item);
	}

	Tensor(vector<int> shape)
	{
		if (!shape.empty())
		{
			vector<int> child_shape(shape.begin() + 1, shape.end());
			m_childLink.reserve(shape.front());
			for (int i = 0; i < shape.front(); i++)
			{
				Tensor<T>* child_Tensor = new Tensor<T>(child_shape);
				m_childLink.push_back(child_Tensor);
			}
		}
	}

	Tensor(vector<int> shape, T initValue) : Tensor(shape)
	{
		vector<int> curShape = this->shape();
		for (int i = 0; i < this->volume(); i++)
		{
			this->operator[](changeDim(i, curShape)) = initValue;
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
		for (auto mtx : this->m_childLink)
		{
			delete mtx;
		}
	}

private:
	vector<Tensor<T>*> m_childLink;
	T m_value;

	vector<int> changeDim(int index, vector<int> fomatShape)
	{
		std::reverse(fomatShape.begin(), fomatShape.end());

		vector<int> matrixIdx;
		int idx = 0;
		int Dn = 1;
		int multi = 1;
		int alpha = 0;
		for (int i = 0; i < fomatShape.size(); i++)
		{
			alpha += idx * multi;
			multi *= Dn;
			Dn = fomatShape[i];
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
		m_childLink.push_back(newTsr);
	}

	void concatenate(Tensor<T>& tsr, const int axis)
	{
		if (axis == 0)
		{
			for (auto clink : tsr.m_childLink)
			{
				this->m_childLink.push_back(new Tensor<T>(*clink));
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
				m_childLink[i]->concatenate(tsr[i], axis - 1);
			}
		}
	}

	Tensor<T>& reshape(vector<int> newShape)
	{
		int emptyIndex = -1;
		int newVolume = 1;
		for (int i = 0; i < newShape.size(); i++)
		{
			if (newShape[i] != -1)
				newVolume *= newShape[i];
			else
				emptyIndex = i;
		}

		int curVolume = this->volume();
		if (emptyIndex != -1 && (curVolume % newVolume == 0))
		{
			int newDimShape = curVolume / newVolume;
			newShape[emptyIndex] = newDimShape;
			newVolume *= newDimShape;
		}

		if (newVolume != curVolume)
		{
			throw invalid_argument("Cannot reshape array of size " + 
				to_string(curVolume) + " into shape " + this->strShape(newShape));
		}

		Tensor<T>* newTsr = new Tensor(newShape);
		vector<int> curShape = this->shape();
		for (int i = 0; i < curVolume; i++)
		{
			T value = this->operator[](changeDim(i, curShape));
			newTsr->operator[](changeDim(i, newShape)) = value;
		}
		return *newTsr;
	}

	Tensor<T>& slice(const int start)
	{
		return slice(start, m_childLink.size());
	}

	Tensor<T>& slice(const int start, const int end)
	{
		Tensor<T>* newTsr = new Tensor<T>;

		for (int i = start; i < end; i++)
		{
			Tensor<T> childTsr(*this->m_childLink[i]);
			newTsr->append(childTsr);
		}

		return *newTsr;
	}

	void erase(const int index)
	{
		delete m_childLink[index];
		m_childLink.erase(m_childLink.begin() + index);
	}

	vector<int> shape()
	{
		if (m_childLink.empty())
		{
			return vector<int>();
		}

		vector<int> curShape = m_childLink[0]->shape();
		curShape.insert(curShape.begin(), (int)m_childLink.size());
		return curShape;
	}

	string toString()
	{
		string result = "[";
		int shapeSize = (int)this->shape().size();

		if (shapeSize == 1)
		{
			for (auto child : this->m_childLink)
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
			for (auto child : this->m_childLink)
			{
				result += child->toString() + "," + enter;
			}
			result.replace(result.size() - shapeSize, result.size(), "]");
		}
		
		return result;
	}

	string strShape()
	{
		string header = "(";
		for (auto idx : this->shape())
		{
			header += to_string(idx) + ", ";
		}
		header.replace(header.size() - 2, header.size(), ")");
		return header;
	}

	string strShape(vector<int> shape)
	{
		string header = "(";
		for (auto idx : shape)
		{
			header += to_string(idx) + ", ";
		}
		header.replace(header.size() - 2, header.size(), ")");
		return header;
	}

	Tensor<T>& matmul(Tensor<T>& tsr)
	{
		vector<int> thisShape = this->shape();
		vector<int> tsrShape = tsr.shape();

		// 현재는 두 계산할려고하는 함수가 둘다 2차원 행렬일때만 지원
		if (thisShape.size() != 2 || tsrShape.size() != 2)
			throw invalid_argument("Currently the we support 2D Matrix multiply.");

		if (thisShape.back() != tsrShape.front())
		{
			throw invalid_argument("Argument axis is invalid value!");
		}
		
		vector<int> newShape = { thisShape.front(), tsrShape.back() };
		Tensor<T>* newTsr = new Tensor<T>(newShape);

		#pragma omp parallel for
		for (int i = 0; i < thisShape.front(); i++)
		{
			for (int tsrI = 0; tsrI < tsrShape.back(); tsrI++)
			{
				T newVal = 0;
				for (int j = 0; j < thisShape.back(); j++)
				{
					T thisVal = this->operator[](i).operator[](j);
					T tsrVal = tsr[j][tsrI];
					newVal += thisVal * tsrVal;
				}
				newTsr->operator[](i).operator[](tsrI) = newVal;
			}
		}
		return *newTsr;
	}

	Tensor<T>& transpose()
	{
		vector<int> curShpae = this->shape();

		// 현재는 2차원 텐서에 전치만 지원
		if (curShpae.size() != 2)
			throw invalid_argument("Currently we support only 2D Tensor.");

		Tensor<T>* newTsr = new Tensor<T>({ curShpae[1], curShpae[0] });
		for (int i = 0; i < curShpae[0]; i++)
		{
			for (int j = 0; j < curShpae[1]; j++)
			{
				newTsr->operator[](j).operator[](i) 
					= this->operator[](i).operator[](j);
			}
		}
		this->~Tensor();
		this->m_childLink = newTsr->m_childLink;
		return *this;
	}

	int volume()
	{
		int indexLen = 1;
		for (auto size : this->shape())
			indexLen *= size;
		return indexLen;
	}

	int size()
	{
		return m_childLink.size();
	}

	/***************************************************/
	/*                   operator                      */
	/***************************************************/
	Tensor<T>& operator[](const int n) 
	{
		return *m_childLink[n];
	}

	Tensor<T>& operator[](const vector<int> idxs) 
	{
		if (!idxs.empty())
		{
			vector<int> child_idxs(idxs.begin() + 1, idxs.end());
			return m_childLink[idxs.front()]->operator[](child_idxs);
		}
		return *this;
	}

	Tensor<T>& operator=(const T n) 
	{
		if (!m_childLink.empty())
		{
			throw invalid_argument("Can't allocate Rvalue to Tesnor(Lvalue)!");
		}
		m_value = n;
		return *this;
	}

	Tensor<T>& operator=(Tensor<T>& tsr)
	{
		this->~Tensor();
		vector<int> rShape = tsr.shape();
		if (rShape.empty())
		{
			this->m_value = tsr.m_value;
		}
		else
		{
			for (int i = 0; i < rShape.front(); i++)
			{
				m_childLink.push_back(new Tensor<T>(tsr[i]));
			}
		}

		return *this;
	}

	MAKE_COMPARE_OPERATOR(>);
	MAKE_COMPARE_OPERATOR(<);
	MAKE_COMPARE_OPERATOR(>=);
	MAKE_COMPARE_OPERATOR(<=);

	operator T()
	{
		if (!m_childLink.empty())
		{
			throw invalid_argument("Tensor can't cast to Rvalue.");
		}

		return m_value;
	}

};

template <typename NODETYPE>
ostream & operator<<(ostream & os, Tensor<NODETYPE>& tsr)
{
	return os << tsr.toString();
}