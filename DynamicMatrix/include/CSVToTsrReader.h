#ifndef _EINSNN_CSVREADER_H_
#define _EINSNN_CSVREADER_H_

#include <string>
#include "../../csv-parser/single_include/csv.hpp"
#include "Tensor.h"
using namespace std;

namespace Matrix
{
	class CSVToTsrReader : public csv::CSVReader
	{
	public:
		CSVToTsrReader(string path) : csv::CSVReader(path)
		{
		}

		CSVToTsrReader(string path, csv::CSVFormat format) : csv::CSVReader(path, format)
		{
		}

		template <typename FiledType>
		Tensor<FiledType> to_tensor()
		{
			Tensor<FiledType> tsr_csv;

			for (csv::CSVRow& row : *this) { // Input iterator
				Tensor<FiledType> tsr_row;
				for (csv::CSVField& field : row) {
					FiledType temp = field.get<FiledType>();
					tsr_row.append(temp);
				}
				tsr_csv.append(tsr_row);
			}

			return tsr_csv;
		}
	};
}
#endif // !_EINSNN_CSVREADER_H_



