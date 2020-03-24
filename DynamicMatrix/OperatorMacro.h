#ifndef _TENSOR_OPERATORMACRO_H_
#define _TENSOR_OPERATORMACRO_H_

#define DEFINE_OPERATOR(METHOD) \
friend auto& operator##METHOD##(const Tensor<double>& lTsr, const double value); \
friend auto& operator##METHOD##(const double value, const Tensor<double>& lTsr); \
friend auto& operator##METHOD##(const Tensor<double>& lTsr, const Tensor<double>& rTsr); \

#define MAKE_RIGHT_OPERATOR_DOUBLE(METHOD) \
auto& operator##METHOD##(const Tensor<double>& lTsr, const double value) \
{ \
	auto type = 0.1 ##METHOD## 0.1; \
	Tensor<decltype(type)>* result = new Tensor<decltype(type)>(lTsr.shape()); \
	for (int i = 0; i < lTsr.volume(); i++) \
	{ \
		vector<int> idx = lTsr.changeIdxOfDim(i); \
		double tsrValue = lTsr.operator[](idx).value(); \
		result->operator[](idx) = tsrValue ##METHOD## value; \
	} \
	return *result; \
}

#define MAKE_LEFT_OPERATOR_DOUBLE(METHOD) \
auto& operator##METHOD##(const double value, const Tensor<double>& lTsr) \
{ \
	auto type = 0.1 ##METHOD## 0.1; \
	Tensor<decltype(type)>* result = new Tensor<decltype(type)>(lTsr.shape()); \
	for (int i = 0; i < lTsr.volume(); i++) \
	{ \
		vector<int> idx = lTsr.changeIdxOfDim(i); \
		double tsrValue = lTsr.operator[](idx).value(); \
		result->operator[](idx) = value ##METHOD## tsrValue; \
	} \
	return *result; \
}

#define MAKE_TENSOR_OPERATOR(METHOD) \
auto& operator##METHOD##(const Tensor<double>& lTsr, const Tensor<double>& rTsr) \
{ \
	vector<int> lShape = lTsr.shape(); \
	vector<int> rShape = rTsr.shape(); \
	Tensor<double> newlTsr; \
	Tensor<double> newrTsr; \
	if (lShape != rShape) \
	{ \
		if (lShape == vector<int>({ 1 })) \
		{ \
			return operator##METHOD##(lTsr[0].value(), rTsr); \
		} \
		else if (rShape == vector<int>({ 1 })) \
		{ \
			return operator##METHOD##(lTsr, rTsr[0].value()); \
		} \
		else if (lShape.size() > rShape.size()) \
		{ \
			newrTsr = rTsr.broadcasting(lShape); \
			newlTsr = lTsr; \
		} \
		else if (lShape.size() < rShape.size())\
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
 \
	auto type = 0.1 ##METHOD## 0.1; \
	Tensor<decltype(type)>* result = new Tensor<decltype(type)>(newlTsr.shape()); \
	for (int i = 0; i < newlTsr.volume(); i++) \
	{ \
		vector<int> idx = newlTsr.changeIdxOfDim(i); \
		double tsrValue = newlTsr.operator[](idx).value(); \
		double value = newrTsr.operator[](idx).value(); \
		result->operator[](idx) = tsrValue ##METHOD## value; \
	} \
	return *result; \
}

#define MAKE_OPERATOR(METHOD) \
MAKE_RIGHT_OPERATOR_DOUBLE(METHOD) \
MAKE_LEFT_OPERATOR_DOUBLE(METHOD) \
MAKE_TENSOR_OPERATOR(METHOD)

#endif // !_TENSOR_OPERATORMACRO_H_


