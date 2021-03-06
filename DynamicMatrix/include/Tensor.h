﻿#ifndef _TENSOR_H_
#define _TENSOR_H_ 2.0.2

#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <math.h>
#include <assert.h>
#include <sstream>
#include <random>
#include "operator_macro.h"
using namespace std;

namespace KDTLAB
{
	template <typename T=double>
	class Tensor
	{
		template<class derived>
		friend class Tensor;

	private:
		vector<int> m_shape;
		vector<T*> m_data;
		bool instanse = false;
		bool iter = false;

	public:
		class iterator : std::iterator<std::input_iterator_tag, int>
		{
		public:
			typedef int difference_type;

			typedef Tensor<T> value_type;
			typedef Tensor<T>& reference;
			typedef Tensor<T>* pointer;
			typedef std::random_access_iterator_tag iterator_category;

			int _idx;
			pointer _ptr = new value_type;
			pointer m_tsr = new value_type;

		public:
			explicit iterator(int ptr, Tensor<T>* tsr) : _idx(ptr), m_tsr(tsr) 
			{
				changePointer();
			}

			iterator(const iterator& _rhs)
			{
				this->operator=(_rhs);
			}

			iterator& operator++() 
			{ 
				++_idx;
				changePointer();
				return (*this);
			}
			iterator operator++(int)
			{
				iterator retval = *this;
				++_idx;
				changePointer();
				return retval;
			}

			iterator& operator--()
			{
				--_idx;
				changePointer();
				return (*this);
			}
			iterator operator--(int)
			{
				iterator retval = *this;
				--_idx;
				changePointer();
				return retval;
			}

			iterator& operator+=(int n)
			{
				_idx += n;
				changePointer();
				return *this;
			}
			iterator& operator-=(int n)
			{
				_idx -= n;
				changePointer();
				return *this;
			}

			iterator operator+(int n)
			{
				iterator temp = *this;
				return temp += n;
			}
			iterator operator-(int n)
			{
				iterator temp = *this;
				return temp -= n;
			}

			reference operator*() const
			{
				return *_ptr;
			}
			pointer operator->()
			{
				return _ptr;
			}

			bool operator==(iterator other) const
			{
				return _idx == other._idx;
			}
			bool operator!=(iterator other) const
			{
				return _idx != other._idx;
			}

			bool operator<(const iterator& rhs) const
			{
				return _idx < rhs._idx;
			}
			bool operator>(const iterator& rhs) const
			{
				return _idx > rhs._idx;
			}
			bool operator<=(const iterator& rhs) const
			{
				return _idx <= rhs._idx;
			}
			bool operator>=(const iterator& rhs) const
			{
				return _idx >= rhs._idx;
			}
			difference_type operator-(const iterator& rhs) const
			{
				return _idx - rhs._idx;
			}

			iterator& operator=(const iterator& rhs)
			{
				_idx = rhs._idx;
				_ptr = new value_type(*rhs._ptr);
				_ptr->iter = true;
				m_tsr = rhs.m_tsr;
				return *this;
			}

		private:
			void changePointer()
			{
				if (_idx < 0 || _idx >= m_tsr->shape().front())
					return;

				_ptr->m_data.clear();
				_ptr->instanse = true;
				_ptr->iter = true;

				// 새로운 텐서 생성
				vector<int> childShape(m_tsr->m_shape.begin() + 1, m_tsr->m_shape.end());
				if (childShape.size() == 0)
					_ptr->m_shape = { 1 };
				else
					_ptr->m_shape = childShape;

				int childVolume = m_tsr->splitVol(m_tsr->m_shape, 1);		
				_ptr->m_data.resize(childVolume);

				// 데이터 카피
				int startIdx = childVolume * _idx;
				std::copy(
					m_tsr->m_data.begin() + startIdx,
					m_tsr->m_data.begin() + startIdx + childVolume,
					_ptr->m_data.begin()
				);
			}
		};

	public:
		Tensor()
		{
			m_shape = { 0 };
		}

		Tensor(const T& value)
		{
			m_shape = { 1 };
			m_data.push_back(new T(value));
		}

		Tensor(const vector<int>& shape, T initValue) : m_shape(shape)
		{
			m_data.resize(splitVol(shape));
			fill(initValue);
		}

		Tensor(const vector<int>& shape) : Tensor(shape, 0)
		{
		}

		Tensor(const Tensor<T>& _rhs)
		{
			clear();
			m_shape = _rhs.m_shape;

			if (_rhs.instanse)
			{
				m_data = _rhs.m_data;
				instanse = true;
			}
			else
			{
				m_data.reserve(_rhs.volume());
				for (auto data : _rhs.m_data)
				{
					m_data.push_back(new T(*data));
				}
			}
		}

		Tensor(const Tensor<T>&& _rhs)
		{
			clear();
			m_shape = _rhs.m_shape;

			if (_rhs.instanse && !_rhs.iter)
			{
				m_data = _rhs.m_data;
				instanse = true;
			}
			else
			{
				m_data.reserve(_rhs.volume());
				for (auto data : _rhs.m_data)
				{
					m_data.push_back(new T(*data));
				}
			}
		}

		~Tensor()
		{
			if (!instanse)
				clear();
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

	private:
		template<typename check_T>
		inline void checkType() const
		{
			if (!std::is_same<T, check_T>::value)
				throw invalid_argument("The select() function only can use when \
					Tensor type is check_T.");
		}

		inline int splitVol(vector<int> shape, const unsigned int& axis = 0) const
		{
			int vol = 1;
			for (vector<int>::size_type i = axis; i < shape.size(); i++)
				vol *= shape[i];
			return vol;
		}

		inline int item_count(const string& str) const
		{
			int dimIdx = -1;
			int closeCount = 0;
			for (int idx = 0; idx < str.size(); idx++)
			{
				if (str[idx] == '[')
				{
					dimIdx++;
				}
				else if (str[idx] == ']')
				{
					dimIdx--;
					if (dimIdx == 0)
					{
						closeCount++;
					}
				}
			}

			return closeCount;
		}

	public:
		/***************************************************/
		/*                     API                         */
		/***************************************************/
		void append(const T& value)
		{
			if (m_shape.size() == 1)
			{
				m_data.push_back(new T(value));
				m_shape.front()++;
			}
			else
				throw invalid_argument("This is not 1d tensor");
		}

		void append(const Tensor<T>& _rhs)
		{
			vector<int> childShape(m_shape.begin() + 1, m_shape.end());
			if (childShape != _rhs.m_shape)
			{
				if (m_data.size() == 0)
				{
					m_shape.insert(m_shape.end(), _rhs.m_shape.begin(), _rhs.m_shape.end());
				}
				else
				{
					throw invalid_argument("Can't append _rhs Tensor");
				}
			}

			m_data.reserve(m_data.size() + splitVol(childShape));
			for (auto data : _rhs.m_data)
			{
				m_data.push_back(new T(*data));
			}
			m_shape[0]++;
		}

		//Tensor<T> concatenate(const Tensor<T>& _rhs, const unsigned int& axis = 0) const
		//{
		//	if (m_shape != _rhs.m_shape)
		//		throw invalid_argument("All the input tensor must have same number of dimensions");

		//	// 새로운 배열 할당
		//	Tensor<T> newTsr;
		//	newTsr.m_shape = m_shape;
		//	newTsr.m_shape[axis] *= 2;
		//	newTsr.m_data.reserve(this->volume() + _rhs.volume());

		//	// 요소 복사
		//	int copyVol = splitVol(m_shape, axis);
		//	for (int idx = 0; idx < this->volume(); idx += copyVol)
		//	{
		//		for (int childIdx = idx; childIdx < idx + copyVol; childIdx++)
		//		{
		//			T value = *this->m_data[childIdx];
		//			newTsr.m_data.push_back(new T(value));
		//		}

		//		for (int childIdx = idx; childIdx < idx + copyVol; childIdx++)
		//		{
		//			T value = *_rhs.m_data[childIdx];
		//			newTsr.m_data.push_back(new T(value));
		//		}
		//	}

		//	return newTsr;
		//}

		Tensor<T> concatenate(const Tensor<T>& _rhs, const unsigned int& axis = 0) const
		{
			if (m_shape.size() == _rhs.m_shape.size())
				for (int i = 0; i < m_shape.size(); ++i)
					if (i != axis && m_shape[i] != _rhs.m_shape[i])
						throw invalid_argument("Can't concatenate shape.");
			else
				throw invalid_argument("Shape of _rhs is not match.");

			Tensor<T> temp;
			temp.m_shape = m_shape;
			temp.m_shape[axis] += _rhs.m_shape[axis];
			temp.m_data.reserve(splitVol(temp.m_shape));
			
			int vol = splitVol(m_shape);
			int thisVol = splitVol(m_shape, axis);
			int rhsVol = splitVol(_rhs.m_shape, axis);

			int thisIdx = 0;
			int rhsIdx = 0;
			for (; thisIdx < vol; thisIdx += thisVol, rhsIdx += rhsVol)
			{
				for (int i = thisIdx; i < thisIdx + thisVol; ++i)
					temp.m_data.push_back(new T(*m_data[i]));

				for (int i = rhsIdx; i < rhsIdx + rhsVol; ++i)
					temp.m_data.push_back(new T(*_rhs.m_data[i]));
			}

			return temp;
		}

		inline void fill(const T& initValue)
		{
			for (std::size_t i = 0; i < m_data.size(); i++)
			{
				m_data[i] = new T(initValue);
			}
		}

		Tensor<T> slice(const int& start) const
		{
			return slice(start, m_shape.front());
		}

		Tensor<T> slice(const int& start, const int& end) const
		{
			int rowSize = end - start;
			if (rowSize < 0)
				throw invalid_argument("Index argument is invaild value.");

			vector<int> childShape = m_shape;
			childShape[0] = rowSize;
			Tensor<T> newTsr;
			newTsr.m_shape = childShape;
			newTsr.m_data.reserve(splitVol(childShape));

			int start_idx = start * splitVol(m_shape, 1);
			std::copy(
				this->m_data.begin() + start_idx,
				this->m_data.begin() + start_idx + newTsr.volume(),
				newTsr.m_data.begin()
			);
			return newTsr;
		}

		vector<Tensor<>> split(const int& idx, const unsigned int& axis = 0) const
		{
			if (axis > m_shape.size() - 1)
				throw invalid_argument("Argument axis is out of shape size");

			int window = m_shape[axis];
			int firstWindow = idx;
			int lastWindow = window - idx;

			Tensor<T> firstTsr;
			firstTsr.m_shape = m_shape;
			firstTsr.m_shape[axis] = firstWindow;
			firstTsr.m_data.reserve(splitVol(firstTsr.m_shape));

			Tensor<T> lastTsr;
			lastTsr.m_shape = m_shape;
			lastTsr.m_shape[axis] = lastWindow;
			lastTsr.m_data.reserve(splitVol(lastTsr.m_shape));

			for (int cur = 0; cur < m_data.size(); cur += window)
			{
				for (int firstCur = cur; firstCur < cur + firstWindow; firstCur++)
				{
					T value = *m_data[firstCur];
					firstTsr.m_data.push_back(new T(value));
				}

				for (int lastCur = cur + firstWindow; lastCur < cur + window; lastCur++)
				{
					T value = *m_data[lastCur];
					lastTsr.m_data.push_back(new T(value));
				}
			}

			return { firstTsr, lastTsr };
		}

		void erase(const int& index)
		{
			m_data.erase(
				m_data.begin() + index, 
				m_data.begin() + index + splitVol(m_shape, 1)
			);
		}

		inline vector<int> shape() const
		{
			return m_shape;
		}

		string toString() const
		{
			string left = "[";
			string right = "]";

			vector<string> before;
			before.reserve((m_data.size()));
			for (auto item : m_data)
				before.push_back(to_string(*item));

			vector<string> after;

			for (int idx = (int)m_shape.size() - 1; idx >= 0; idx--)
			{
				int window = m_shape[idx];
				string item = left;
				for (std::size_t i = 0, count = 1; i < before.size(); i++, count++)
				{
					item += before[i] + ", ";
					if (count == window)
					{
						item.replace(item.size() - 2, item.size(), right);
						after.push_back(item);

						item = left;
						count = 0;
					}
				}

				before = after;
				after.clear();
			}

			return before[0];
		}
					
		void loadFromString(string str)
		{
			clear();

			// 배열 전체 복사
			int split_l = -1;
			T ptr;
			for (int idx = 0; idx < str.size(); idx++)
			{
				char aChar = str[idx];
				if (aChar == ',' || aChar == ']')
				{
					if (split_l != -1)
					{
						string item = str.substr(split_l, idx - split_l);
						std::istringstream(item) >> ptr;
						m_data.push_back(new T(ptr));
						split_l = -1;
					}
				}
				else if (aChar != '[' && aChar != ' ')
				{
					if (split_l == -1)
					{
						split_l = idx;
					}
				}
			}

			m_shape.clear();
			m_shape.push_back(item_count(str));
			while (m_shape.back() != 0)
			{
				str = str.substr(1, str.size() - 2);
				auto split_l = str.find_first_of('[') + 1;
				auto split_r = str.find_first_of(']');
				str = str.substr(split_l, split_r - split_l);
				m_shape.push_back(item_count(str));
			}

			m_shape.back() = (int)std::count(str.begin(), str.end(), ',') + 1;
		}

		string strShape() const
		{
			return strShape(this->m_shape);
		}

		string strShape(const vector<int>& shape) const
		{
			string header = "(";
			for (auto idx : shape)
				header += to_string(idx) + ", ";
			header.replace(header.size() - 2, header.size(), ")");
			return header;
		}

		Tensor<T> matmul(const Tensor<T>& _rhs) const
		{
			vector<int> curShape = this->shape();
			int curShapeFront = curShape.front();
			int curShapeBack = curShape.back();
			vector<int> rShape = _rhs.shape();
			int rShapeFront = rShape.front();
			int rShapeBack = rShape.back();

			// 현재는 두 계산할려고하는 함수가 둘다 2차원 행렬일때만 지원
			if (curShape.size() != 2 || rShape.size() != 2)
				throw invalid_argument("Currently the we support 2D Matrix multiply.");

			if (curShapeBack != rShapeFront)
			{
				throw invalid_argument("Argument axis is invalid value!");
			}

			Tensor<T> newTsr({ curShapeFront, rShapeBack });
#pragma omp parallel for
			for (int i = 0; i < curShapeFront; i++)
			{
				for (int rIdx = 0; rIdx < rShapeBack; rIdx++)
				{
					T newVal = 0;
					for (int j = 0; j < curShapeBack; j++)
					{
						T thisVal = *this->m_data[i * curShapeBack + j];
						T tsrVal = *_rhs.m_data[j * rShapeBack + rIdx];
						newVal += thisVal * tsrVal;
					}
					*newTsr.m_data[i * rShapeBack + rIdx] = newVal;
				}
			}
			return newTsr;
		}

		Tensor<T> transpose() const
		{
			// 현재는 2차원 텐서에 전치만 지원
			if (m_shape.size() != 2)
				throw invalid_argument("Currently we support only 2D Tensor.");

			int row = m_shape[0];
			int col = m_shape[1];
			Tensor<T> newTsr({ col, row });

			for (int i = 0; i < row; i++)
			{
				for (int j = 0; j < col; j++)
				{
					*newTsr.m_data[j * row + i] = *m_data[i * col + j];
				}
			}
			return newTsr;
		}

		inline int volume() const
		{
			return (int)m_data.size();
		}

		inline int size() const
		{
			return m_shape[0];
		}

		template<typename derived>
		Tensor<derived> select(const Tensor<derived>& thenTsr, const Tensor<derived>& elseTsr) const
		{
			checkType<bool>();

			// 이 인스턴스와 인자값들에 shape이 같은지 확인
			vector<int> curShape = this->shape();
			if ((curShape != thenTsr.shape()) || (curShape != elseTsr.shape()))
			{
				throw invalid_argument("The argument should be same shape of this.");
			}

			Tensor<derived> selectTsr(curShape);
			for (int idx = 0; idx < volume(); idx++)
			{
				bool boolValue = *m_data[idx];
				if (boolValue)
					*selectTsr.m_data[idx] = *thenTsr.m_data[idx];
				else
					*selectTsr.m_data[idx] = *elseTsr.m_data[idx];
			}
			return selectTsr;
		}

		T value() const
		{
			if (m_data.size() == 1)
				return *(m_data[0]);
			else
				throw runtime_error("This tensor is not scala");
		}

		void clear()
		{
			for (auto item : m_data)
			{
				delete item;
			}
			m_shape = { 0 };
			m_data.clear();
		}

		Tensor<T> broadcasting(const vector<int>& shape) const
		{
			vector<int> curShape = this->shape();
			vector<int> childShape(shape.begin() + 1, shape.end());
			if (curShape != childShape)
			{
				throw invalid_argument("Cannot broadcasting " +
					strShape(curShape) + " to " + strShape(childShape));
			}

			Tensor<T> result;
			result.m_shape = shape;
			result.m_data.reserve(splitVol(shape));
			for (int i = 0; i < shape.front(); i++)
			{
				for (auto data : m_data)
				{
					result.m_data.push_back(new T(*data));
				}
			}
			return result;
		}  

		// 표준 정규분포를 따르는 랜던값들로 초기화
		Tensor<double>& randomInit(double min, double max)
		{
			checkType<double>();

			random_device rn;
			mt19937_64 rnd(rn());
			uniform_real_distribution<double> range(min, max);

			for (auto data : m_data)
			{
				*data = range(rnd);
			}
			return *this;
		}

		Tensor<int>& randomInit(int min, int max)
		{
			checkType<int>();

			random_device rn;
			mt19937_64 rnd(rn());
			uniform_int_distribution<int> range(min, max);

			for (auto data : m_data)
			{
				*data = range(rnd);
			}
			return *this;
		}


		iterator begin()
		{
			return iterator(0, this);
		}

		iterator end()
		{
			return iterator(size(), this);
		}

	//	/***************************************************/
	//	/*                    연산자                       */
	//	/***************************************************/
		Tensor<double> exp() const
		{
			checkType<double>();

			Tensor<double> result;
			result.m_shape = m_shape;
			result.m_data.reserve(volume());
			for (auto data : m_data)
			{
				double* value = new double(std::exp(*data));
				result.m_data.push_back(value);
			}
			return result;
		}

		Tensor<double> sum() const
		{
			checkType<double>();
			
			vector<int> childShape(m_shape.begin() + 1, m_shape.end());
			Tensor<double> sumTsr(childShape, 0);
			for (int i = 0; i < this->size(); i++)
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
			
			Tensor<double> result;
			result.m_shape = m_shape;
			result.m_data.reserve(volume());
			for (auto data : m_data)
			{
				double* value = new double(std::pow(*data, 2));
				result.m_data.push_back(value);
			}
			return result;
		}

		Tensor<double> sqrt() const
		{
			checkType<double>();

			Tensor<double> result;
			result.m_shape = m_shape;
			result.m_data.reserve(volume());
			for (auto data : m_data)
			{
				double* value = new double(std::sqrt(*data));
				result.m_data.push_back(value);
			}
			return result;
		}

	//	/***************************************************/
	//	/*                   operator                      */
	//	/***************************************************/
		Tensor<T> operator[](const unsigned int& n) const
		{
			if (m_shape.size() <= 0)
			{
				throw invalid_argument("Out of index.");
			}

			Tensor<T> newTsr;
			newTsr.instanse = true;

			// 새로운 텐서 생성
			vector<int> childShape(m_shape.begin() + 1, m_shape.end());
			if (childShape.size() == 0)
				newTsr.m_shape = {1};
			else
				newTsr.m_shape = childShape;

			int childVolume = splitVol(m_shape, 1);
			newTsr.m_data.resize(childVolume);

			// 데이터 카피
			int startIdx = childVolume * n;
			std::copy(
				m_data.begin() + startIdx,
				m_data.begin() + startIdx + childVolume,
				newTsr.m_data.begin()
			);
			return newTsr;
		}

		Tensor<T>& operator=(const T& n)
		{
			if (m_data.size() > 1)
			{
				throw invalid_argument("Can't allocate scalar to tensor");
			}

			*m_data[0] = n;
			return *this;
		}

		Tensor<T>& operator=(const Tensor<T>& _rhs)
		{
			if (instanse)
			{
				if (m_shape != _rhs.m_shape)
					throw invalid_argument("Cannot copy tensor. Tensor shape does not match.");

				for (int i = 0; i < _rhs.volume(); i++)
				{
					*m_data[i] = *_rhs.m_data[i];
				}
			}
			else
			{
				clear();
				m_shape = _rhs.m_shape;
				m_data.reserve(_rhs.volume());
				for (int i = 0; i < _rhs.volume(); i++)
				{
					m_data.push_back(new T(*_rhs.m_data[i]));
				}
			}

			return *this;
		}

		bool operator==(const Tensor<T>& _rhs)
		{
			bool same = true;
			if (m_shape == _rhs.m_shape)
			{
				int len = m_data.size();
				for (int idx = 0; idx < len; idx++)
				{
					if (*m_data[idx] != *_rhs.m_data[idx])
					{
						same = false;
						break;
					}
				}
			}
			else
			{
				same = false;
			}

			return same;
		}
	};

	template <typename NODETYPE>
	ostream & operator<<(ostream& os, Tensor<NODETYPE>& _rhs)
	{
		return os << _rhs.toString();
	}

	/*inline auto operator+(const Tensor<double>& lTsr, const double value) \
	{ \
		auto type = 0.1 + 0.1; \
		Tensor<decltype(type)> result(lTsr.shape()); \
		for (int idx = 0; idx < lTsr.volume(); idx++) \
		{ \
			*result.m_data[idx] = (*lTsr.m_data[idx]) + value; \
		} \
		return result; \
	}

	inline auto operator+(const double value, const Tensor<double>& rTsr) \
	{ \
		auto type = 0.1 + 0.1; \
		Tensor<decltype(type)> result(rTsr.shape()); \
		for (int idx = 0; idx < rTsr.volume(); idx++) \
		{ \
			*result.m_data[idx] = value + (*rTsr.m_data[idx]); \
		} \
		return result; \
	}
	
	inline auto operator+(const Tensor<double>& lTsr, const Tensor<double>& rTsr) \
	{ \
		vector<int> lShape = lTsr.shape(); \
		vector<int> rShape = rTsr.shape(); \
		Tensor<double> newlTsr; \
		Tensor<double> newrTsr; \
		if (lShape != rShape) \
		{ \
			if (lShape == vector<int>({ 1 })) \
			{ \
				return operator+(lTsr[0].value(), rTsr); \
			} \
			else if (rShape == vector<int>({ 1 })) \
			{ \
				return operator+(lTsr, rTsr[0].value()); \
			} \
			else if (lTsr.volume() > rTsr.volume()) \
			{ \
				newrTsr = rTsr.broadcasting(lShape); \
				newlTsr = lTsr; \
			} \
			else if (lTsr.volume() < rTsr.volume())\
			{ \
				newlTsr = lTsr.broadcasting(rShape); \
				newrTsr = rTsr; \
			} \
		} \
		else \
		{ \
			newlTsr = lTsr; \
			newrTsr = rTsr; \
		} \
		auto type = 0.1 + 0.1; \
		Tensor<decltype(type)> result(newrTsr.shape()); \
		for (int idx = 0; idx < newrTsr.volume(); idx++) \
		{ \
			*result.m_data[idx] = (*newlTsr.m_data[idx]) + (*newrTsr.m_data[idx]); \
		} \
		return result; \
	}*/

	MAKE_OPERATOR(> )
	MAKE_OPERATOR(< )
	MAKE_OPERATOR(>= )
	MAKE_OPERATOR(<= )

	MAKE_OPERATOR(-)
	MAKE_OPERATOR(+)
	MAKE_OPERATOR(*)
	MAKE_OPERATOR(/ )

}

#endif // !TENSOR_H_

/*
 * Copyright (c) by Woo,Jun-Hyeok(woojh3690@gmail.com). All rights reserved.
 * Consult your license regarding permissions and restrictions.
V2.0.1 */