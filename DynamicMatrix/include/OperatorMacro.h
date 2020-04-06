#ifndef _TENSOR_OPERATORMACRO_H_
#define _TENSOR_OPERATORMACRO_H_

#define DEFINE_OPERATOR(METHOD) \
friend auto operator##METHOD##(const Tensor<double>& lTsr, const double value); \
friend auto operator##METHOD##(const double value, const Tensor<double>& rTsr); \
friend auto operator##METHOD##(const Tensor<double>& lTsr, const Tensor<double>& rTsr); \

#define MAKE_RIGHT_OPERATOR_DOUBLE(METHOD) \
inline auto operator##METHOD##(const Tensor<double>& lTsr, const double value) \
{ \
	auto type = 0.1 ##METHOD## 0.1; \
	Tensor<decltype(type)> result(lTsr.shape()); \
	vector<size_t> formatShape = lTsr.shape(); \
	vector<size_t> idx(formatShape.size()); \
	do \
	{ \
		double tsrValue = lTsr[idx].value(); \
		result[idx] = tsrValue ##METHOD## value; \
	} while (dPlus(formatShape, idx)); \
	return result; \
}

#define MAKE_LEFT_OPERATOR_DOUBLE(METHOD) \
inline auto operator##METHOD##(const double value, const Tensor<double>& rTsr) \
{ \
	auto type = 0.1 ##METHOD## 0.1; \
	Tensor<decltype(type)> result(rTsr.shape()); \
	vector<size_t> formatShape = rTsr.shape(); \
	vector<size_t> idx(formatShape.size()); \
	do \
	{ \
		double tsrValue = rTsr[idx].value(); \
		result[idx] = value ##METHOD## tsrValue; \
	} while (dPlus(formatShape, idx)); \
	return result; \
}

#define MAKE_TENSOR_OPERATOR(METHOD) \
inline auto operator##METHOD##(const Tensor<double>& lTsr, const Tensor<double>& rTsr) \
{ \
	vector<size_t> lShape = lTsr.shape(); \
	vector<size_t> rShape = rTsr.shape(); \
	Tensor<double> newlTsr; \
	Tensor<double> newrTsr; \
	if (lShape != rShape) \
	{ \
		if (lShape == vector<size_t>({ 1 })) \
		{ \
			return operator##METHOD##(lTsr[0].value(), rTsr); \
		} \
		else if (rShape == vector<size_t>({ 1 })) \
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
	Tensor<decltype(type)> result(newlTsr.shape()); \
	vector<size_t> formatShape = lTsr.shape(); \
	vector<size_t> idx(formatShape.size()); \
	do \
	{ \
		double tsrValue = newlTsr[idx].value(); \
		double value = newrTsr[idx].value(); \
		result[idx] = tsrValue ##METHOD## value; \
	} while (dPlus(formatShape, idx)); \
	return result; \
}

#define MAKE_OPERATOR(METHOD) \
MAKE_RIGHT_OPERATOR_DOUBLE(METHOD) \
MAKE_LEFT_OPERATOR_DOUBLE(METHOD) \
MAKE_TENSOR_OPERATOR(METHOD)

#endif // !_TENSOR_OPERATORMACRO_H_


