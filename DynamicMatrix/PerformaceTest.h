#pragma once

class PerformanceTest
{
public:
	PerformanceTest();
	~PerformanceTest();
public:
	void allocateTest();
	void runTest();
	void matmulTest();
	void transposeTest();
	void copyTest();
	void copyRun();
	void multiplyTest();
	void selectTest();
	void expTest();
	void shapeTest();
	void volumeTest();
	void CSVReadTest();
	void SplitTest();
};