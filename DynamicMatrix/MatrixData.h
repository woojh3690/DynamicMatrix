#pragma once
#include <string>
#include <cassert>
using namespace std;

class MatrixData
{
public:
	int noOfRow = 0; // 레코드수
	int noOfCol = 0; // 필드수
	string** data = nullptr; // 2차원 데이터

	/**
	* @brief 2차원 string 동적 배열 메모리 할당
	*/
	bool MallocData()
	{
		// 조건 검사
		if (noOfRow <= 0 || noOfCol <= 0)
		{
			assert(false);
			return false;
		}

		data = new string*[noOfRow];
		for (int i = 0; i < noOfRow; i++)
			data[i] = new string[noOfCol];

		return true;
	}

	/**
	* @brief 2차원 string 메모리 해제
	*/
	bool DeleteData()
	{
		for (int i = 0; i < noOfRow; i++)
		{
 			delete[] data[i];
		}
		delete[] data;

		noOfRow = 0;
		noOfCol = 0;

		return true;
	}

	/**
	* @brief 데이터를 CSV 포맷으로 변환
	*/
	string toCSV()
	{
		return toCSV(0, noOfRow);
	}

	/**
	* @brief 지정한 부분만 CSV포맷으로 변경
	* @param int startIndex row의 시작 인덱스
	* @param int endIndex row의 끝 인덱스
	*/
	string toCSV(int startIndex, int endIndex)
	{
		if (data == nullptr)
			return "";

		string base = "";
		for (int i = startIndex; i < endIndex; i++)
		{
			for (int j = 0; j < noOfCol; j++)
			{
				base += data[i][j] + ", ";
			}
			base += "\n";
		}

		return base;
	}

	/**
	* @brief 할당 연산자를 오버라이드해서 깊은 복사 구현
	*/
	/*void operator=(const MatrixData& rhs)
	{
		if (this != &rhs) 
		{
			noOfRow = rhs.noOfRow;
			noOfCol = rhs.noOfCol;
			MallocData();
			for (int row = 0; row < rhs.noOfRow; row++) {
				for (int col = 0; col < rhs.noOfCol; col++) {
					data[row][col] = rhs.data[row][col];
				}
			}
		}
	}*/

}; // 2차원 string 테이블