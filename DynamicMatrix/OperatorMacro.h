#ifndef _TENSOR_OPERATORMACRO_H_
#define _TENSOR_OPERATORMACRO_H_

#define MAKE_RIGHT_OPERATOR(METHOD) \
auto& operator##METHOD##(Tensor<double>& lTsr, double value) \
{ \
	double a = 0.1; \
	double b = 0.1; \
	auto temp = a ##METHOD## b; \
	Tensor<decltype(temp)>* result = new Tensor<decltype(temp)>(lTsr.shape()); \
	for (int i = 0; i < lTsr.volume(); i++) \
	{ \
		vector<int> idx = lTsr.changeIdxOfDim(i); \
		double tsrValue = lTsr.operator[](idx); \
		result->operator[](idx) = tsrValue ##METHOD## value; \
	} \
	return *result; \
}

#define MAKE_LEFT_OPERATOR(METHOD) \
auto& operator##METHOD##(double value, Tensor<double>& lTsr) \
{ \
	double a = 0.1; \
	double b = 0.1; \
	auto temp = a ##METHOD## b; \
	Tensor<decltype(temp)>* result = new Tensor<decltype(temp)>(lTsr.shape()); \
	for (int i = 0; i < lTsr.volume(); i++) \
	{ \
		vector<int> idx = lTsr.changeIdxOfDim(i); \
		double tsrValue = lTsr.operator[](idx); \
		result->operator[](idx) = value ##METHOD## tsrValue; \
	} \
	return *result; \
}

#define MAKE_OPERATOR(METHOD) \
MAKE_RIGHT_OPERATOR(METHOD) \
MAKE_LEFT_OPERATOR(METHOD)

#define DEFINE_OPERATOR(METHOD) \
friend auto& operator##METHOD##(Tensor<double>& lTsr, double value); \
friend auto& operator##METHOD##(double value, Tensor<double>& lTsr); \

#endif // !_TENSOR_OPERATORMACRO_H_


