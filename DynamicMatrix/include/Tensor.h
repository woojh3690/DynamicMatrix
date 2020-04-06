#ifndef _TENSOR_H_
#define _TENSOR_H_

#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <math.h>
#include <assert.h>
#include <sstream>
#include <random>
#include "OperatorMacro.h"
using namespace std;

namespace Matrix
{
	static bool dPlus(const vector<size_t>& shape, vector<size_t>& idx);
	static bool dPlus(const vector<size_t>& shape, vector<size_t>& idx, int cur);

	template <typename T=double>
	class Tensor
	{
	public:
		Tensor()
		{
		}

		Tensor(const T& value)
		{
			Tensor<T>* item = new Tensor<T>;
			item->m_value = value;
			this->m_childLink.push_back(item);
		}

		Tensor(const vector<size_t>& shape)
		{
			if (!shape.empty())
			{
				vector<size_t> child_shape(shape.begin() + 1, shape.end());
				m_childLink.resize(shape.front());
				for (size_t i = 0; i < shape.front(); i++)
				{
					m_childLink[i] = new Tensor<T>(child_shape);
				}
			}
		}

		Tensor(const vector<size_t>& shape, T initValue) : Tensor(shape)
		{
			fill(initValue);
		}

		Tensor(const Tensor<T>& _Right)
		{
			this->operator=(_Right);
		}

		template <typename NODETYPE>
		friend ostream& operator<<(ostream& os, Tensor<T>& dt);

		DEFINE_OPERATOR(> )
		DEFINE_OPERATOR(< )
		DEFINE_OPERATOR(>=)
		DEFINE_OPERATOR(<=)

		DEFINE_OPERATOR(-)
		DEFINE_OPERATOR(+)
		DEFINE_OPERATOR(*)
		DEFINE_OPERATOR(/)

		~Tensor()
		{
			for (auto mtx : this->m_childLink)
				delete mtx;
		}

	private:
		vector<Tensor<T>*> m_childLink;
		T m_value;
	
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
			m_childLink.push_back(new Tensor<T>(tsr));
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
				size_t curFront = this->shape().front();
				size_t mtxFront = tsr.shape().front();

				if (curFront != mtxFront)
					throw invalid_argument("Argument axis is invalid value!");

				for (size_t i = 0; i < curFront; i++)
				{
					m_childLink[i]->concatenate(tsr[i], axis - 1);
				}
			}
		}

		Tensor<T> reshape(vector<size_t> newShape)
		{
			size_t emptyIndex = -1;
			size_t newVolume = 1;
			for (size_t i = 0; i < newShape.size(); i++)
			{
				if (newShape[i] != -1)
					newVolume *= newShape[i];
				else
					emptyIndex = i;
			}

			size_t curVolume = this->volume();
			if (emptyIndex != -1 && (curVolume % newVolume == 0))
			{
				size_t newDimShape = curVolume / newVolume;
				newShape[emptyIndex] = newDimShape;
				newVolume *= newDimShape;
			}

			if (newVolume != curVolume)
			{
				throw invalid_argument("Cannot reshape array of size " +
					to_string(curVolume) + " into shape " + this->strShape(newShape));
			}

			Tensor<T> newTsr(newShape);
			vector<size_t> newIdx(newShape.size());
			vector<size_t> curShape = this->shape();
			vector<size_t> curIdx(curShape.size());

			bool go = true;
			do
			{
				T value = this->operator[](curIdx).value();
				newTsr[newIdx] = value;

				go = dPlus(curShape, curIdx);
				bool temp = dPlus(newShape, newIdx);
				assert(go == temp);
			} while (go);

			return newTsr;
		}

		Tensor<T>& fill(T initValue)
		{
			vector<size_t> curShape = this->shape();
			vector<size_t> idx(curShape.size());
			do
			{
				this->operator[](idx) = initValue;
			} while (dPlus(curShape, idx));
			return *this;
		}

		Tensor<T> slice(const size_t start) const
		{
			return slice(start, m_childLink.size());
		}

		Tensor<T> slice(const size_t start, const size_t end) const
		{
			Tensor<T> newTsr;
			for (size_t i = start; i < end; i++)
			{
				Tensor<T>* node = this->m_childLink[i];
				newTsr.append(*node);
			}
			return newTsr;
		}

		Tensor<T> split(const size_t idx, unsigned int axis = 0)
		{

			if (axis == 0)
			{
				Tensor<T> result;
				result.append(this->slice(0, idx));
				result.append(this->slice(idx));
				return result;
			}
			else
			{
				Tensor<T> result({2, 0});
				Tensor<T> splited;
				for (auto childTsr : m_childLink)
				{
					splited = childTsr->split(idx, axis - 1);
					result[0].append(splited[0]);
					result[1].append(splited[1]);
				}
				return result;
			}
		}

		void erase(const size_t index)
		{
			delete m_childLink[index];
			m_childLink.erase(m_childLink.begin() + index);
		}

		vector<size_t> shape() const
		{
			if (m_childLink.empty())
			{
				return vector<size_t>();
			}

			vector<size_t> curShape = m_childLink[0]->shape();
			curShape.insert(curShape.begin(), m_childLink.size());
			return curShape;
		}

		string toString() const
		{
			string result = "[";
			size_t shapeSize = this->shape().size();
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
				for (size_t i = 0; i < shapeSize - 1; i++)
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

		bool loadFromString(const string& str)
		{
			this->~Tensor();
			char l_bracket = '[';
			char r_bracket = ']';
			if (str.front() != l_bracket || str.back() != r_bracket)
				throw invalid_argument("string format must be [] type");
			
			string strTsr = str.substr(1, str.size() - 2);
			string::size_type idx = strTsr.find(l_bracket);
			if (idx == string::npos)
			{
				size_t count = 0;
				size_t startIdx = 0;
				T ptr;
				for (size_t i = 0; i < strTsr.size(); i++)
				{
					char letter = strTsr[i];
					if (letter == ',')
					{
						string item = strTsr.substr(startIdx, i - startIdx);
						std::istringstream(item) >> ptr;
						m_childLink.push_back(new Tensor());
						m_childLink[size() - 1]->operator=(ptr);
						startIdx = i+2;
					}

				}
				string item = strTsr.substr(startIdx, strTsr.size());
				std::istringstream(item) >> ptr;
				m_childLink.push_back(new Tensor());
				m_childLink[size() - 1]->operator=(ptr);
			}
			else
			{
				size_t count = 0;
				size_t startIdx = 0;
				for (size_t i = 0; i < strTsr.size(); i++)
				{
					char letter = strTsr[i];
					if (letter == l_bracket)
					{
						count++;
						if (count == 1)
						{
							startIdx = i;
						}
					}

					if (letter == r_bracket)
					{
						count--;
						if (count == 0)
						{
							string child_strTsr = strTsr.substr(startIdx, i - startIdx + 1);
							Tensor* child_tsr = new Tensor();
							child_tsr->loadFromString(child_strTsr);
							m_childLink.push_back(child_tsr);
						}
					}
				}
			}
			return true;
		}

		string strShape() const
		{
			return strShape(this->shape());
		}

		string strShape(const vector<size_t>& shape) const
		{
			string header = "(";
			for (auto idx : shape)
			{
				header += to_string(idx) + ", ";
			}
			header.replace(header.size() - 2, header.size(), ")");
			return header;
		}

		Tensor<T> matmul(const Tensor<T>& tsr) const
		{
			vector<size_t> thisShape = this->shape();
			vector<size_t> tsrShape = tsr.shape();

			// 현재는 두 계산할려고하는 함수가 둘다 2차원 행렬일때만 지원
			if (thisShape.size() != 2 || tsrShape.size() != 2)
				throw invalid_argument("Currently the we support 2D Matrix multiply.");

			if (thisShape.back() != tsrShape.front())
			{
				throw invalid_argument("Argument axis is invalid value!");
			}

			Tensor<T> newTsr({ thisShape.front(), tsrShape.back() });
			for (size_t i = 0; i < thisShape.front(); i++)
			{
				for (size_t tsrI = 0; tsrI < tsrShape.back(); tsrI++)
				{
					T newVal = 0;
					for (size_t j = 0; j < thisShape.back(); j++)
					{
						T thisVal = this->operator[](i)[j].value();
						T tsrVal = tsr.operator[](j)[tsrI].value();
						newVal += thisVal * tsrVal;
					}
					newTsr[i][tsrI] = newVal;
				}
			}
			return newTsr;
		}

		Tensor<T> transpose() const
		{
			vector<size_t> curShpae = this->shape();

			// 현재는 2차원 텐서에 전치만 지원
			if (curShpae.size() != 2)
				throw invalid_argument("Currently we support only 2D Tensor.");

			Tensor<T> newTsr({ curShpae[1], curShpae[0] });
			for (size_t i = 0; i < curShpae[0]; i++)
			{
				for (size_t j = 0; j < curShpae[1]; j++)
				{
					newTsr[j][i] = this->operator[](i)[j];
				}
			}
			return newTsr;
		}

		size_t volume() const
		{
			size_t indexLen = 1;
			for (auto size : this->shape())
				indexLen *= size;
			return indexLen;
		}

		size_t size() const
		{
			return m_childLink.size();
		}

		template<typename derived>
		Tensor<derived> select(const Tensor<derived>& thenTsr, const Tensor<derived>& elseTsr) const
		{
			vector<size_t> curShape = this->shape();
			checkType<bool>();

			// 이 인스턴스와 인자값들에 shape이 같은지 확인
			if ((curShape != thenTsr.shape()) || (curShape != elseTsr.shape()))
			{
				throw invalid_argument("The argument should be same shape of this.");
			}

			Tensor<derived> selectTsr(curShape);
			vector<size_t> idx(curShape.size());
			do
			{
				T boolValue = this->operator[](idx).value();
				if (boolValue)
					selectTsr[idx] = thenTsr[idx];
				else
					selectTsr[idx] = elseTsr[idx];
			} while (dPlus(curShape, idx));
			return selectTsr;
		}

		T value() const
		{
			if (!m_childLink.empty())
			{
				throw invalid_argument("Tensor can't cast to Rvalue.");
			}

			return m_value;
		}

		Tensor<T> broadcasting(const vector<size_t>& shape) const
		{
			vector<size_t> curShape = this->shape();
			vector<size_t> childShape(shape.begin() + 1, shape.end());
			if (curShape != childShape)
			{
				throw invalid_argument("Cannot broadcasting " +
					strShape(curShape) + " to " + strShape(childShape));
			}

			Tensor<T> result;
			for (size_t i = 0; i < shape.front(); i++)
			{
				result.append(*this);
			}
			return result;
		}

		Tensor<double>& randomInit(T min, T max)
		{
			checkType<double>();

			random_device rn;
			mt19937_64 rnd(rn());
			uniform_real_distribution<T> range(min, max);

			vector<size_t> curShape = this->shape();
			vector<size_t> idx(curShape.size());
			do
			{
				this->operator[](idx) = range(rnd);
			} while (dPlus(curShape, idx));
			return *this;
		}
		/***************************************************/
		/*                    연산자                       */
		/***************************************************/
		Tensor<double> exp() const
		{
			checkType<double>();
			vector<size_t> curShape = this->shape();
			Tensor<double> expTsr(curShape);
			vector<size_t> idx(curShape.size());
			do
			{
				double value = this->operator[](idx).value();
				double exp_value = std::exp(value);
				expTsr[idx] = exp_value;
			} while (dPlus(curShape, idx));
			return expTsr;
		}

		Tensor<double> sum() const
		{
			checkType<double>();

			vector<size_t> childShape = m_childLink[0]->shape();
			Tensor<double> sumTsr(childShape, 0);
			for (size_t i = 0; i < this->size(); i++)
			{
				sumTsr = sumTsr + this->operator[](i);
			}
			return sumTsr;
		}

		Tensor<double> mean() const
		{
			checkType<double>();
			return sum() / this->volume();
		}

		Tensor<double> pow() const
		{
			checkType<double>();
			vector<size_t> curShape = this->shape();
			Tensor<double> expTsr(curShape);
			vector<size_t> idx(curShape.size());
			do
			{
				double value = this->operator[](idx).value();
				double exp_value = std::pow(value, 2);
				expTsr[idx] = exp_value;
			} while (dPlus(curShape, idx));
			return expTsr;
		}

		Tensor<double> sqrt() const
		{
			vector<size_t> curShape = this->shape();
			Tensor<double> expTsr(curShape);
			vector<size_t> idx(curShape.size());
			do
			{
				double value = this->operator[](idx).value();
				double exp_value = std::sqrt(value);
				expTsr[idx] = exp_value;
			} while (dPlus(curShape, idx));
			return expTsr;
		}

		/***************************************************/
		/*                   operator                      */
		/***************************************************/
		Tensor<T>& operator[](const size_t n)
		{
			return *m_childLink[n];
		}

		const Tensor<T>& operator[](const size_t n) const
		{
			return *m_childLink[n];
		}

		Tensor<T>& operator[](const vector<size_t>& idxs)
		{
			if (!idxs.empty())
			{
				vector<size_t> child_idxs(idxs.begin() + 1, idxs.end());
				return m_childLink[idxs.front()]->operator[](child_idxs);
			}
			return *this;
		}

		const Tensor<T>& operator[](const vector<size_t>& idxs) const
		{
			if (!idxs.empty())
			{
				vector<size_t> child_idxs(idxs.begin() + 1, idxs.end());
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
			vector<size_t> rShape = tsr.shape();
			if (rShape.empty())
			{
				this->m_value = tsr.m_value;
			}
			else
			{
				m_childLink.resize(rShape.front());
				for (size_t i = 0; i < rShape.front(); i++)
				{
					m_childLink[i] = new Tensor<T>(tsr[i]);
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

	static bool dPlus(const vector<size_t>& shape, vector<size_t>& idx, size_t cur)
	{
		idx[cur]++;
		if (shape[cur] <= idx[cur])
		{
			idx[cur] = 0;
			if (--cur == -1)
				return false;

			return dPlus(shape, idx, cur);
		}

		return true;
	}

	static bool dPlus(const vector<size_t>& shape, vector<size_t>& idx)
	{
		size_t cur = idx.size() - 1;
		idx[cur]++;
		if (shape[cur] <= idx[cur])
		{
			idx[cur] = 0;
			if (--cur == -1)
				return false;
			return dPlus(shape, idx, cur);
		}

		return true;
	}

	//static auto operator+(const Tensor<double>& lTsr, const double value) \
	//{ \
	//	auto type = 0.1 + 0.1; \
	//	Tensor<decltype(type)> result(lTsr.shape()); \
	//	vector<size_t> formatShape = lTsr.shape();
	//	vector<size_t> idx(formatShape.size());
	//	do
	//	{
	//		double tsrValue = lTsr[idx].value(); \
	//		result[idx] = tsrValue + value; \
	//	} while (dPlus(formatShape, idx));

	//	return result; \
	//}

	//static auto operator+(const double value, const Tensor<double>& rTsr) \
	//{ \
	//	auto type = 0.1 + 0.1; \
	//	Tensor<decltype(type)> result(rTsr.shape()); \
	//	vector<size_t> formatShape = rTsr.shape();
	//	vector<size_t> idx(formatShape.size());
	//	do
	//	{
	//		double tsrValue = rTsr[idx].value(); \
	//		result[idx] = value + tsrValue; \
	//	} while (dPlus(formatShape, idx));

	//	return result; \
	//}

	//static auto operator+(const Tensor<double>& lTsr, const Tensor<double>& rTsr) \
	//{ \
	//	vector<size_t> lShape = lTsr.shape(); \
	//	vector<size_t> rShape = rTsr.shape(); \
	//	Tensor<double> newlTsr; \
	//	Tensor<double> newrTsr; \
	//	if (lShape != rShape) \
	//	{ \
	//		if (lShape == vector<size_t>({ 1 })) \
	//		{ \
	//			return operator+(lTsr[0].value(), rTsr); \
	//		} \
	//		else if (rShape == vector<size_t>({ 1 })) \
	//		{ \
	//			return operator+(lTsr, rTsr[0].value()); \
	//		} \
	//		else if (lShape.size() > rShape.size()) \
	//		{ \
	//			newlTsr = lTsr; \
	//			newrTsr = rTsr.broadcasting(lShape); \
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
	//	auto type = 0.1 + 0.1; \
	//	Tensor<decltype(type)> result(newlTsr.shape()); \
	//	vector<size_t> formatShape = lTsr.shape();
	//	vector<size_t> idx(formatShape.size());
	//	do
	//	{
	//		double tsrValue = newlTsr[idx].value(); \
	//		double value = newrTsr[idx].value(); \
	//		result[idx] = tsrValue + value; \
	//	} while (dPlus(formatShape, idx));
	//	return result; \
	//}

	MAKE_OPERATOR(> )
	MAKE_OPERATOR(< )
	MAKE_OPERATOR(>=)
	MAKE_OPERATOR(<=)

	MAKE_OPERATOR(-)
	MAKE_OPERATOR(+)
	MAKE_OPERATOR(*)
	MAKE_OPERATOR(/)
}

#endif // !TENSOR_H_