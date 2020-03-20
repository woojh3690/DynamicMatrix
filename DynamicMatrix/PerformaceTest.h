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
	void compareTest();
	void selectTest();
	void expTest();
};