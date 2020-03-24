#ifndef _TENSOR_H_
#define _TENSOR_H_

#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <omp.h>
#include <math.h>
#include "OperatorMacro.h"
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
		item->m_value = value;
		this->m_childLink.push_back(item);
	}

	Tensor(vector<int> shape)
	{
		resize(shape);
	}

	Tensor(vector<int> shape, T initValue)
	{
		resize(shape);
		fill(initValue);
	}

	Tensor(const Tensor<T>& _Right)
	{
		this->operator=(_Right);
	}

	template <typename NODETYPE>
	friend ostream& operator<<(ostream& os, Tensor<T>& dt);

	DEFINE_OPERATOR(>)
	DEFINE_OPERATOR(<)
	DEFINE_OPERATOR(>=)
	DEFINE_OPERATOR(<=)

	DEFINE_OPERATOR(-)
	DEFINE_OPERATOR(+)
	DEFINE_OPERATOR(*)
	DEFINE_OPERATOR(/)

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

	vector<int> changeIdxOfDim(int index, vector<int> fomatShape) const
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

	template<typename check_T>
	void checkType() const
	{
		if (!std::is_same<T, check_T>::value)
			throw invalid_argument("The select() function only can use when \
				Tensor type is check_T.");
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

	void append(const Tensor<T>& tsr)
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
			T value = this->operator[](changeIdxOfDim(i, curShape)).value();
			newTsr->operator[](changeIdxOfDim(i, newShape)) = value;
		}
		return *newTsr;
	}

	Tensor<T>& resize(vector<int> shape)
	{
		this->~Tensor();
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

		return *this;
	}

	Tensor<T>& fill(T initValue)
	{
		vector<int> curShape = this->shape();
		#pragma omp parallel for
		for (int i = 0; i < this->volume(); i++)
		{
			this->operator[](changeIdxOfDim(i, curShape)) = initValue;
		}
		return *this;
	}

	Tensor<T>& slice(const int start) const
	{
		return slice(start, m_childLink.size());
	}

	Tensor<T>& slice(const int start, const int end) const
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

	vector<int> shape() const
	{
		if (m_childLink.empty())
		{
			return vector<int>();
		}

		vector<int> curShape = m_childLink[0]->shape();
		curShape.insert(curShape.begin(), (int)m_childLink.size());
		return curShape;
	}

	string toString() const
	{
		string result = "[";
		int shapeSize = (int)this->shape().size();

		if (shapeSize == 1)
		{
			for (auto child : this->m_childLink)
			{
				result += to_string((*child).value()) + ", ";
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

	string strShape() const
	{
		return strShape(this->shape());
	}

	string strShape(vector<int> shape) const
	{
		string header = "(";
		for (auto idx : shape)
		{
			header += to_string(idx) + ", ";
		}
		header.replace(header.size() - 2, header.size(), ")");
		return header;
	}

	Tensor<T>& matmul(Tensor<T>& tsr) const
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

		Tensor<T>* newTsr = new Tensor<T>({ thisShape.front(), tsrShape.back() });
		#pragma omp parallel for
		for (int i = 0; i < thisShape.front(); i++)
		{
			for (int tsrI = 0; tsrI < tsrShape.back(); tsrI++)
			{
				T newVal = 0;
				for (int j = 0; j < thisShape.back(); j++)
				{
					T thisVal = this->operator[](i).operator[](j).value();
					T tsrVal = tsr.operator[](j).operator[](tsrI).value();
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

	int volume() const
	{
		int indexLen = 1;
		for (auto size : this->shape())
			indexLen *= size;
		return indexLen;
	}

	size_t size() const
	{
		return m_childLink.size();
	}

	template<typename selectDerived>
	Tensor<selectDerived>& select(Tensor<selectDerived> thenTsr, Tensor<selectDerived> elseTsr)
	{
		vector<int> curShape = this->shape();
		checkType<bool>();

		// 이 인스턴스와 인자값들에 shape이 같은지 확인
		if ((curShape != thenTsr.shape()) || (curShape != elseTsr.shape()))
		{
			throw invalid_argument("The argument should be same shape of this.");
		}

		Tensor<selectDerived>* selectTsr = new Tensor<selectDerived>(curShape);
		#pragma omp parallel for
		for (int i = 0; i < this->volume(); i++)
		{
			vector<int> idx = changeIdxOfDim(i, curShape);
			T boolValue = this->operator[](idx).value();
			if (boolValue)
				selectTsr->operator[](idx) = thenTsr[idx];
			else
				selectTsr->operator[](idx) = elseTsr[idx];
		}

		return *selectTsr;
	}

	vector<int> changeIdxOfDim(int i) const
	{
		return changeIdxOfDim(i, this->shape());
	}

	T value() const
	{
		if (!m_childLink.empty())
		{
			throw invalid_argument("Tensor can't cast to Rvalue.");
		}

		return m_value;
	}

	Tensor<T>& broadcasting(vector<int> shape) const
	{
		vector<int> curShape = this->shape();
		vector<int> childShape(shape.begin() + 1, shape.end());
		if (curShape != childShape)
		{
			throw invalid_argument("Cannot broadcasting " + 
				strShape(curShape) + " to " + strShape(childShape));
		}

		Tensor<T>* result = new Tensor<T>();
		for (int i = 0; i < shape.front(); i++)
		{
			result->append(*this);
		}

		return *result;
	}

	/***************************************************/
	/*                    연산자                       */
	/***************************************************/
	Tensor<double>& exp() const
	{
		checkType<double>();
		vector<int> curShape = this->shape();
		Tensor<double>* expTsr = new Tensor<double>(curShape);
		#pragma omp parallel for
		for (int i = 0; i < this->volume(); i++)
		{
			vector<int> idx = changeIdxOfDim(i, curShape);
			double value = this->operator[](idx).value();
			double exp_value = std::exp(value);
			expTsr->operator[](idx) = exp_value;
		}
		return *expTsr;
	}

	Tensor<double>& sum() const
	{
		checkType<double>();

		vector<int> childShape = m_childLink[0]->shape();
		Tensor<double> *sumTsr = new Tensor<double>(childShape, 0);
		for (int i = 0; i < this->size(); i++)
		{
			sumTsr = &(*sumTsr + this->operator[](i));
		}
		return *sumTsr;
	}

	Tensor<double>& mean() const
	{
		checkType<double>();
		return sum() / this->volume();
	}

	Tensor<double>& pow() const
	{
		checkType<double>();

		vector<int> curShape = this->shape();
		Tensor<double>* expTsr = new Tensor<double>(curShape);
		#pragma omp parallel for
		for (int i = 0; i < this->volume(); i++)
		{
			vector<int> idx = changeIdxOfDim(i, curShape);
			double value = this->operator[](idx).value();
			double exp_value = std::pow(value, 2);
			expTsr->operator[](idx) = exp_value;
		}
		return *expTsr;
	}

	Tensor<double>& sqrt() const
	{
		vector<int> curShape = this->shape();
		Tensor<double>* expTsr = new Tensor<double>(curShape);
		#pragma omp parallel for
		for (int i = 0; i < this->volume(); i++)
		{
			vector<int> idx = changeIdxOfDim(i, curShape);
			double value = this->operator[](idx).value();
			double exp_value = std::sqrt(value);
			expTsr->operator[](idx) = exp_value;
		}
		return *expTsr;
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

	const Tensor<T>& operator[](const int n) const
	{
		return *m_childLink[n];
	}

	const Tensor<T>& operator[](const vector<int> idxs) const
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

	Tensor<T>& operator=(const Tensor<T>& tsr)
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
};

template <typename NODETYPE>
ostream & operator<<(ostream& os, Tensor<NODETYPE>& tsr)
{
	return os << tsr.toString();
}

//auto& operator*(const Tensor<double>& lTsr, const double value) \
//{ \
//	auto type = 0.1 * 0.1; \
//	Tensor<decltype(type)>* result = new Tensor<decltype(type)>(lTsr.shape()); \
//	for (int i = 0; i < lTsr.volume(); i++) \
//	{ \
//		vector<int> idx = lTsr.changeIdxOfDim(i); \
//		double tsrValue = lTsr.operator[](idx).value(); \
//		result->operator[](idx) = tsrValue * value; \
//	} \
//	return *result; \
//}
//
//auto& operator*(const double value, const Tensor<double>& lTsr) \
//{ \
//	auto type = 0.1 * 0.1; \
//	Tensor<decltype(type)>* result = new Tensor<decltype(type)>(lTsr.shape()); \
//	for (int i = 0; i < lTsr.volume(); i++) \
//	{ \
//		vector<int> idx = lTsr.changeIdxOfDim(i); \
//		double tsrValue = lTsr.operator[](idx).value(); \
//		result->operator[](idx) = value * tsrValue; \
//	} \
//	return *result; \
//}
//
//auto& operator*(const Tensor<double>& lTsr, const Tensor<double>& rTsr) \
//{ \
//	vector<int> lShape = lTsr.shape(); \
//	vector<int> rShape = rTsr.shape(); \
//	Tensor<double> newlTsr; \
//	Tensor<double> newrTsr; \
//	if (lShape != rShape) \
//	{ \
//		if (lShape == vector<int>({ 1 })) \
//		{ \
//			return operator*(lTsr[0].value(), rTsr); \
//		} \
//		else if (rShape == vector<int>({ 1 })) \
//		{ \
//			return operator*(lTsr, rTsr[0].value()); \
//		} \
//		else if (lShape.size() > rShape.size()) \
//		{ \
//			newrTsr = rTsr.broadcasting(lShape); \
//			newlTsr = lTsr; \
//		} \
//		else if (lShape.size() < rShape.size())\
//		{ \
//			newlTsr = lTsr.broadcasting(rShape); \
//			newrTsr = rTsr; \
//		} \
//	} \
//	else \
//	{ \
//		newlTsr = lTsr; \
//		newrTsr = rTsr; \
//	} \
// \
//	auto type = 0.1 * 0.1; \
//	Tensor<decltype(type)>* result = new Tensor<decltype(type)>(newlTsr.shape()); \
//	for (int i = 0; i < newlTsr.volume(); i++) \
//	{ \
//		vector<int> idx = newlTsr.changeIdxOfDim(i); \
//		double tsrValue = newlTsr.operator[](idx).value(); \
//		double value = newrTsr.operator[](idx).value(); \
//		result->operator[](idx) = tsrValue * value; \
//	} \
//	return *result; \
//}

MAKE_OPERATOR(>)
MAKE_OPERATOR(<)
MAKE_OPERATOR(>=)
MAKE_OPERATOR(<=)

MAKE_OPERATOR(-)
MAKE_OPERATOR(+)
MAKE_OPERATOR(*)
MAKE_OPERATOR(/)

#endif // !TENSOR_H_