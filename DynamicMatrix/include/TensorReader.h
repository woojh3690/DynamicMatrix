#ifndef _EINSNN_TENSORREADER_H_
#define _EINSNN_TENSORREADER_H_

#include <string>
#include <iostream>
#include "../../csv-parser/include/csv/reader.hpp"
#include "Tensor.h"
using namespace std;

namespace KDTLAB
{
	class TensorReader : public csv::Reader
	{
	public:
		TensorReader() : csv::Reader()
		{
		}

		template <typename FiledType>
		Tensor<FiledType> to_tensor()
		{
			Tensor<FiledType> tsr_csv;
			for (auto& row : this->rows()) {
				Tensor<FiledType> tsr_row;
				for (auto& col : this->cols())
				{
					FiledType ptr;
					std::istringstream(row[col]) >> ptr;
					tsr_row.append(ptr);
				}
				tsr_csv.append(tsr_row);
			}

			return tsr_csv;
		}
	};
}
#endif // !_EINSNN_CSVREADER_H_