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
	for (int idx = 0; idx < lTsr.volume(); idx++) \
	{ \
		*result.m_data[idx] = (*lTsr.m_data[idx]) ##METHOD## value; \
	} \
	return result; \
}

#define MAKE_LEFT_OPERATOR_DOUBLE(METHOD) \
inline auto operator##METHOD##(const double value, const Tensor<double>& rTsr) \
{ \
	auto type = 0.1 ##METHOD## 0.1; \
	Tensor<decltype(type)> result(rTsr.shape()); \
	for (int idx = 0; idx < rTsr.volume(); idx++) \
	{ \
		*result.m_data[idx] = value ##METHOD## (*rTsr.m_data[idx]); \
	} \
	return result; \
}

#define MAKE_TENSOR_OPERATOR(METHOD) \
inline auto operator##METHOD##(const Tensor<double>& lTsr, const Tensor<double>& rTsr) \
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
	auto type = 0.1 ##METHOD## 0.1; \
	Tensor<decltype(type)> result(newrTsr.shape()); \
	for (int idx = 0; idx < newrTsr.volume(); idx++) \
	{ \
		*result.m_data[idx] = (*newlTsr.m_data[idx]) ##METHOD## (*newrTsr.m_data[idx]); \
	} \
	return result; \
}

#define MAKE_OPERATOR(METHOD) \
MAKE_RIGHT_OPERATOR_DOUBLE(METHOD) \
MAKE_LEFT_OPERATOR_DOUBLE(METHOD) \
MAKE_TENSOR_OPERATOR(METHOD)

#endif // !_TENSOR_OPERATORMACRO_H_


