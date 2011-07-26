#include "vector2d.h"

// Default constructor
template <class T>
vector2d<T>::vector2d()
{
	dimSize = 1;
}

// Class constructor
template <class T>
vector2d<T>::vector2d(int dims)
{
	dimSize = dims;
}

// Destructor
template <class T>
vector2d<T>::~vector2d()
{
	for(int i = allData.size() - 1; i >= 0; i--)
	{
		delete allData[i];
		allData.pop_back();
	}
}

// Other methods:
template <class T>
void vector2d<T>::push(vector<T> input)
{
	tmpVector = new T[dimSize];
	for (unsigned int i = 0; i < dimSize; i++)
	{
		if (i >= input.size())
			tmpVector[i] = 0;
		else
			tmpVector[i] = input[i];  
	}
	allData.push_back(tmpVector);
}

template <class T>
vector<T> vector2d<T>::pop()
{
	vector<T> res;
	int sz = allData.size() - 1;
	if (sz >= 0)
	{
		for (int j = 0; j < dimSize; j++)
		{
			res.push_back(allData[sz][j]);
		}
		delete allData[sz];
		allData.pop_back();
	}
	return res;
}

template <class T>
bool vector2d<T>::saveToFile(string fileName)
{
	if (allData.size() == 0)
	{
		cout << "ERROR! Attempt to write empty data." << endl;
		return false;
	}
	string fileNameTmp = fileName + ".bin";
	ofstream f;
	f.open(fileNameTmp.c_str(), ios::out | ios::binary);
	if (f.is_open())
	{
		int tmp = allData.size();
		f.write(reinterpret_cast<char *>(&tmp), sizeof(int));
		f.write(reinterpret_cast<char *>(&dimSize), sizeof(int));
		for (unsigned int i = 0; i < allData.size(); i++)
		{
			f.write(reinterpret_cast<char *>(allData[i]), sizeof(T)*dimSize);
		}
	}
	f.close();
	return true;
}

template <class T>
bool vector2d<T>::readFromFile(string fileName)
{
	for(int i = allData.size() - 1; i >= 0; i--)
	{
		delete allData[i];
		allData.pop_back();
	}
	ifstream f;
	f.open(fileName.c_str(), ios::in | ios::binary);
	if (f.is_open())
	{
		int size;
		f.read(reinterpret_cast<char *>(&size), sizeof(int));
		f.read(reinterpret_cast<char *>(&dimSize), sizeof(int));
		for (int i = 0; i < size; i++)
		{
			tmpVector = new T[dimSize];
			f.read(reinterpret_cast<char *>(tmpVector), sizeof(T)*dimSize);
			allData.push_back(tmpVector);
		}
		f.close();
	}
	else
	{
		cout << "ERROR! Unable to open the file: " << fileName << endl;
	}
	return true;
}

template <class T>
vector<T> vector2d<T>::operator [](int index)
{
	vector<T> res;
	if (index < allData.size())
	{
		for (int j = 0; j < dimSize; j++)
		{
			res.push_back(allData[index][j]);
		}
	}
	return res;
}

template <class T>
int vector2d<T>::size()
{
	return allData.size();
}

template class vector2d<char>;
template class vector2d<int>;
template class vector2d<float>;
template class vector2d<double>;