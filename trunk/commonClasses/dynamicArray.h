#ifndef DYNAMIC_ARRAYS_H_
#define DYNAMIC_ARRAYS_H_

#if !defined DEBUG_ARRAYS

#define dynamicArray(TYPE, NAME)					TYPE * NAME
#define dynamicArrayRef(TYPE, NAME)					TYPE * & NAME
#define dynamicAllocate(TYPE, ARR_NAME, SIZE)		ARR_NAME = new TYPE[SIZE]
#define dynamicAllocateZeros(TYPE, ARR_NAME, SIZE)	ARR_NAME = new TYPE[SIZE]; memset(ARR_NAME, 0, SIZE * sizeof(TYPE))
#define dynamicFree(ARR_NAME)						delete [] ARR_NAME

#else

#define SERIAL_EXECUTION

#include <assert.h>

#define dynamicArray(TYPE, NAME)					dynamicArrayInternal< TYPE > NAME
#define dynamicArrayRef(TYPE, NAME)					dynamicArrayInternal< TYPE > & NAME
#define dynamicAllocate(TYPE, ARR_NAME, SIZE)		ARR_NAME.allocate(SIZE)
#define dynamicAllocateZeros(TYPE, ARR_NAME, SIZE)	ARR_NAME.allocateZeros(SIZE)
#define dynamicFree(ARR_NAME)						ARR_NAME.setMemoryReleasedFlag()

// Dynamic array 1d
template <class T>
class dynamicArrayInternal
{
private:
	bool isAllocated;
	bool isMemoryReleased;
	int dim1size;
	T * data;
public:
	dynamicArrayInternal();
	~dynamicArrayInternal();
	dynamicArrayInternal(const dynamicArrayInternal & orig);
	dynamicArrayInternal & operator = (const dynamicArrayInternal & orig);
	void allocate(const int size);
	void allocateZeros(const int size);
	void setMemoryReleasedFlag();
	T & operator [] (int idx);
};

template <class T>
dynamicArrayInternal<T>::dynamicArrayInternal()
{
	isAllocated = false;
	dim1size = 0;
	data = NULL;
}

template <class T>
dynamicArrayInternal<T>::~dynamicArrayInternal(void)
{
	if (isAllocated)
	{
		delete [] data;
		assert(isMemoryReleased);
	}
}

template <class T>
dynamicArrayInternal<T>::dynamicArrayInternal(const dynamicArrayInternal & orig)
{
	assert(orig.isAllocated);
	dim1size = orig.dim1size;
	if (isAllocated) delete [] data;
	data = new T[dim1size];
	memcpy(orig.data, data, dim1size * sizeof(T));
	isAllocated = true;
}

template <class T>
dynamicArrayInternal<T> & dynamicArrayInternal<T>::operator = (const dynamicArrayInternal & orig)
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
T & dynamicArrayInternal<T>::operator [] (int idx)
{
	assert(isAllocated);
	assert(!isMemoryReleased);
	assert(idx >= 0);
	assert(idx < dim1size);
	return data[idx];
}

template <class T>
void dynamicArrayInternal<T>::allocate(const int size)
{
	assert(!isAllocated);
	dim1size = size;
	data = new T[size];
	isAllocated = true;
	isMemoryReleased = false;
}

template <class T>
void dynamicArrayInternal<T>::allocateZeros(const int size)
{
	assert(!isAllocated);
	dim1size = size;
	data = new T[size];
	isAllocated = true;
	isMemoryReleased = false;
	for (int i = 0; i < size; i++)
	{
		memset((void *)(& data[i]), 0, sizeof(T));
	}
}

template <class T>
void dynamicArrayInternal<T>::setMemoryReleasedFlag()
{
	isMemoryReleased = true;
}

#endif

#endif