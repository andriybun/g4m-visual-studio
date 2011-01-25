//  Name:			2d vector class
//  Author:         Andriy Bun
//  Date:           01.04.10
//  Description:    

#ifndef VECTOR_2D_H_
#define VECTOR_2D_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class vector2d
{
private:
	int dimensionsNum;
	vector<double *> dat_all;
	double *tmpVector;
public:
	vector2d();
	vector2d(int dims);
	~vector2d();
	void push(vector<double> input);
	vector<double> pop();
	bool saveToFile(string fileName);
	bool readFromFile(string fileName);
	vector<double> operator [](int index);
	int size();
};

#endif