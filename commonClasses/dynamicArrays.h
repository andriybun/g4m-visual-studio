#ifndef DYNAMIC_ARRAYS_H_
#define DYNAMIC_ARRAYS_H_

//#define DEBUG_ARRAYS		// uncomment to test exceeding array bounds

#include <assert.h>

// Dynamic array 1d
template <class T>
class dynamicArray
{
private:
	bool isAllocated;
	int dim1size;
	T * data;
public:
	dynamicArray();
	~dynamicArray();
	dynamicArray(const dynamicArray & orig);
	dynamicArray & operator = (const dynamicArray & orig);
	void allocate(const int size);
	T & operator [] (int idx);
};

template <class T>
dynamicArray<T>::dynamicArray()
{
	isAllocated = false;
	dim1size = 0;
	data = NULL;
}

template <class T>
dynamicArray<T>::~dynamicArray(void)
{
	if (isAllocated)
	{
		delete [] data;
	}
}

template <class T>
dynamicArray<T>::dynamicArray(const dynamicArray & orig)
{
	assert(orig.isAllocated);
	dim1size = orig.dim1size;
	if (isAllocated) delete [] data;
	data = new T[dim1size];
	memcpy(orig.data, data, dim1size * sizeof(T));
	isAllocated = true;
}

template <class T>
dynamicArray<T> & dynamicArray<T>::operator = (const dynamicArray & orig)
{
	if (this != &orig)
	{
		assert(orig.isAllocated);
		dim1size = orig.dim1size;
		if (isAllocated) delete [] data;
		data = new T[dim1size];
		memcpy(orig.data, data, dim1size * sizeof(T));
		isAllocated = true;
	}
	return *this;
}

template <class T>
T & dynamicArray<T>::operator [] (int idx)
{
	assert(isAllocated);
	assert(idx >= 0);
	assert(idx < dim1size);
	return data[idx];
}

template <class T>
void dynamicArray<T>::allocate(const int size)
{
	assert(!isAllocated);
	dim1size = size;
	data = new T[size];
	isAllocated = true;
}

#endif