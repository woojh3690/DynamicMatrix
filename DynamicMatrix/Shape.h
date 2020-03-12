#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <string>
#include <vector>

typedef std::vector<std::size_t> vector_t;

class Shape
{
public:
	vector_t m_shape;

public:
	Shape() {};
	~Shape() {};

public:
	bool operator==(Shape& rShape)
	{
		bool result = true;
		for (int i = 0; i < m_shape.size(); i++)
		{
			if (m_shape[i] != rShape.m_shape[i])
			{
				result = false;
				break;
			}
		}
		return result;
	}

	string strShape()
	{
		return strShape(m_shape);
	}

	string strShape(vector_t shape)
	{
		string header = "(";
		for (auto idx : shape)
		{
			header += to_string(idx) + ", ";
		}
		header.replace(header.size() - 2, header.size(), ")");
		return header;
	}
};
#endif // !_SHAPE_H_
