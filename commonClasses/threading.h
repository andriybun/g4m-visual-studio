#ifndef THREADING_H_
#define THREADING_H_

#include <cmath>

#include "dynamicArray.h"

// Uncomment for serial execution of program in one thread
//#define SERIAL_EXECUTION

#ifndef SERIAL_EXECUTION
	#if defined(_WIN32) || defined(_WIN64) || defined(OS_WINDOWS)
		#include <windows.h>
		#define PARALLEL_ON_WINDOWS
	#else
		// Write smth here for non windows OS'es

	#endif
#else
	// Write smth here for non parallel execution

#endif

#ifdef PARALLEL_ON_WINDOWS

template <class inCellDataT, class inCommonDataT, class outCellDataT>
struct exchangeT
{
	int count;
	const inCellDataT * inCellData;
	const inCommonDataT * inCommonData;
	outCellDataT * outCellData;
	int (*execute)(const inCellDataT &, const inCommonDataT &, outCellDataT &);
};

template <class inCellDataT, class inCommonDataT, class outCellDataT>
DWORD WINAPI ThreadFun(LPVOID lpParam)
{
	exchangeT<inCellDataT, inCommonDataT, outCellDataT> * localDataPtr = (exchangeT<inCellDataT, inCommonDataT, outCellDataT> *)lpParam;

	for (int i = 0; i < localDataPtr->count; i++)
	{
		localDataPtr->execute(localDataPtr->inCellData[i], *localDataPtr->inCommonData, localDataPtr->outCellData[i]);
	}

	return 0;
}

template <class realT, class inCellDataT, class inCommonDataT, class outCellDataT>
int parallelExecute(int (*execute)(const inCellDataT &, const inCommonDataT &, outCellDataT &), 
					const inCellDataT * arr, const inCommonDataT & paramsStruct, outCellDataT * outArr, int count)
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo( &sysinfo );
	int numCpus = sysinfo.dwNumberOfProcessors;
	printf("Detected %d CPU's\n", numCpus);

	int NUM_THREADS = numCpus;
	HANDLE * hThread = new HANDLE[NUM_THREADS];
	DWORD * threadID = new DWORD[NUM_THREADS];
	exchangeT<inCellDataT, inCommonDataT, outCellDataT> * msgArray = new exchangeT<inCellDataT, inCommonDataT, outCellDataT>[NUM_THREADS];

	// Distribute array between threads
	int itemsPerThread = (int)ceil((float)count / NUM_THREADS);
	for (int i = 0; i < NUM_THREADS; i++)
	{
		msgArray[i].count = min(itemsPerThread, count - i * itemsPerThread);
		msgArray[i].inCellData = &arr[i * itemsPerThread];
		msgArray[i].inCommonData = &paramsStruct;
		msgArray[i].outCellData = &outArr[i * itemsPerThread];
		msgArray[i].execute = execute;
	}

	DWORD threadsCreated = 0;
	for (int i = 0; i < NUM_THREADS; i++)
	{
		hThread[i] = CreateThread(NULL, // security attributes ( default if NULL )
			0, // stack SIZE default if 0
			ThreadFun<inCellDataT, inCommonDataT, outCellDataT>, // Start Address
			&msgArray[i], // input data
			0, // creational flag ( start if  0 )
			&threadID[i]); // thread ID
		if (!hThread[i])
		{
			printf("Thread #%d is not created!\n", i+1);
		}
		else
		{
			threadsCreated++;
		}
	}

	if (WaitForMultipleObjects(threadsCreated, hThread, true, INFINITE) == (DWORD)0xFFFFFFFF)
	{
		printf("Error finishing threads: %d\n", GetLastError());
	}

	for (int i = 0; i < NUM_THREADS; i++)
	{
		CloseHandle(hThread[i]);
	}

	// Cleanup:
	delete [] hThread;
	delete [] threadID;
	delete [] msgArray;

	return 0;
}

#else
// Serial execution

#ifndef DEBUG_ARRAYS

template <class realT, class inCellDataT, class inCommonDataT, class outCellDataT>
int parallelExecute(int (*execute)(const inCellDataT &, const inCommonDataT &, outCellDataT &), 
					const inCellDataT * arr, const inCommonDataT & paramsStruct, outCellDataT * outArr, int count)
{
	for (int i = 0; i < count; i++)	
	{
		execute(arr[i], paramsStruct, outArr[i]);
	}

	return 0;
}

#else

template <class realT, class inCellDataT, class inCommonDataT, class outCellDataT>
int parallelExecute(int (*execute)(const inCellDataT &, const inCommonDataT &, outCellDataT &),
					dynamicArrayInternal< inCellDataT > & arr, const inCommonDataT & paramsStruct, 
					dynamicArrayInternal< outCellDataT> & outArr, int count)
{
	for (int i = 0; i < count; i++)	
	{
		execute(arr[i], paramsStruct, outArr[i]);
	}

	return 0;
}

#endif // if defined DEBUG_ARRAYS

#endif // if defined PARALLEL_ON_WINDOWS

#endif