#ifndef _TENSOR_OPERATORMACRO_H_
#define _TENSOR_OPERATORMACRO_H_

#define DEFINE_OPERATOR(METHOD) \
friend auto& operator##METHOD##(Tensor<double> lTsr, Tensor<double> rTsr); \
friend auto& operator##METHOD##(Tensor<double>& lTsr, double value); \
friend auto& operator##METHOD##(double value, Tensor<double>& lTsr); \

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

#define MAKE_TENSOR_OPERATOR(METHOD) \
auto& operator##METHOD##(Tensor<double> lTsr, Tensor<double> rTsr) \
{ \
	vector<int> lShape = lTsr.shape(); \
	vector<int> rShape = rTsr.shape(); \
	if (lShape != rShape) \
	{ \
		if (lShape == vector<int>({1})) \
		{ \
			return operator##METHOD##(lTsr[0].value(), rTsr); \
		} \
		else if (rShape == vector<int>({ 1 })) \
		{ \
			return operator##METHOD##(lTsr, rTsr[0].value()); \
		} \
		else if (lShape.size() > rShape.size()) \
		{ \
			rTsr = rTsr.broadcasting(lShape); \
		} \
		else if (lShape.size() < rShape.size())\
		{ \
			lTsr = lTsr.broadcasting(rShape); \
		} \
	} \
 \
	auto type = 0.1 ##METHOD## 0.1; \
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

#define MAKE_OPERATOR(METHOD) \
MAKE_RIGHT_OPERATOR_DOUBLE(METHOD) \
MAKE_LEFT_OPERATOR_DOUBLE(METHOD) \
MAKE_TENSOR_OPERATOR(METHOD)

#endif // !_TENSOR_OPERATORMACRO_H_


