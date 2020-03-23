#ifndef _TENSOR_OPERATORMACRO_H_
#define _TENSOR_OPERATORMACRO_H_

#define MAKE_TENSOR_OPERATOR(METHOD) \
auto& operator##METHOD##(Tensor<double>& lTsr, Tensor<double>& rTsr) \
{ \
	auto type = 0.1 ##METHOD## 0.1; \
 \
	if (lTsr.shape() != rTsr.shape()) \
	{ \
		rTsr = rTsr.broadcasting(lTsr.shape()); \
	} \
 \
	Tensor<decltype(type)>* result = new Tensor<decltype(type)>(lTsr.shape()); \
	for (int i = 0; i < lTsr.volume(); i++) \
	{ \
		vector<int> idx = lTsr.changeIdxOfDim(i); \
		double tsrValue = lTsr.operator[](idx).value(); \
		double value = rTsr.operator[](idx).value(); \
		result->operator[](idx) = tsrValue ##METHOD## value; \
	} \
	return *result; \
}

#define MAKE_RIGHT_OPERATOR_DOUBLE(METHOD) \
auto& operator##METHOD##(Tensor<double>& lTsr, double value) \
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
auto& operator##METHOD##(double value, Tensor<double>& lTsr) \
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

#define MAKE_OPERATOR(METHOD) \
MAKE_TENSOR_OPERATOR(METHOD) \
MAKE_RIGHT_OPERATOR_DOUBLE(METHOD) \
MAKE_LEFT_OPERATOR_DOUBLE(METHOD)

#define DEFINE_OPERATOR(METHOD) \
friend auto& operator##METHOD##(Tensor<double>& lTsr, Tensor<double>& rTsr); \
friend auto& operator##METHOD##(Tensor<double>& lTsr, double value); \
friend auto& operator##METHOD##(double value, Tensor<double>& lTsr); \

#endif // !_TENSOR_OPERATORMACRO_H_


