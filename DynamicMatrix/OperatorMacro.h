#ifndef _TENSOR_OPERATORMACRO_H_
#define _TENSOR_OPERATORMACRO_H_

#define MAKE_TENSOR_OPERATOR(METHOD) \
auto& operator##METHOD##(Tensor<double>& lTsr, Tensor<double>& rTsr) \
{ \
	double a = 0.1; \
	double b = 0.1; \
	auto temp = a ##METHOD## b; \
	Tensor<decltype(temp)>* result = new Tensor<decltype(temp)>(lTsr.shape()); \
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
	double a = 0.1; \
	double b = 0.1; \
	auto temp = a ##METHOD## b; \
	Tensor<decltype(temp)>* result = new Tensor<decltype(temp)>(lTsr.shape()); \
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
	double a = 0.1; \
	double b = 0.1; \
	auto temp = a ##METHOD## b; \
	Tensor<decltype(temp)>* result = new Tensor<decltype(temp)>(lTsr.shape()); \
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


