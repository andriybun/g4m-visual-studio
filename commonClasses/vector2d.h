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

template <class T>
class vector2d
{
private:
	int dimSize;
	vector<T *> allData;
	T *tmpVector;
public:
	vector2d();
	vector2d(int dims);
	~vector2d();
	void push(vector<T> input);
	vector<T> pop();
	bool saveToFile(string fileName);
	bool readFromFile(string fileName);
	vector<T> operator [](int index);
	int size();
};

#endif