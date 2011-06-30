#ifndef COUNTRY_SUMMARY_BASE_H_
#define COUNTRY_SUMMARY_BASE_H_
// This class requires definition of basic arithmetic operators for structs used as template parameters

#include <vector>
#include <map>

#include "dynamicArray.h"
#include "../g4m/autogenerated_operators.h"

template <class structT>
class countrySummaryBaseT
{
protected:
	typedef vector<int> intVectorT;
	dynamicArray(structT, data);
	dynamicArray(int, count);
	int numYears;
	int beginYear;
	int endYear;
	int numCountries;
	map<int, int> countriesMap;			// first - external index, second - internal index
	bool isAllocated;

	size_t getElemendIdx(int countryIdx, int year)
	{
		return countriesMap[countryIdx] * numYears + year - beginYear;
	}
public:
	countrySummaryBaseT()
	{
		isAllocated = false;
	}

	countrySummaryBaseT(intVectorT &countriesVector, int beginYearIn, int endYearIn)
	{
		allocate(countriesVector, beginYearIn, endYearIn);
	}

	void allocate(intVectorT &countriesVector, int beginYearIn, int endYearIn)
	{
		numCountries = countriesVector.size();
		beginYear = beginYearIn;
		endYear = endYearIn;
		numYears = endYear - beginYear + 1;
		dynamicAllocateZeros(structT, data, numCountries * numYears);
		dynamicAllocateZeros(int, count, numCountries * numYears);
		intVectorT::iterator currElem = countriesVector.begin();
		intVectorT::iterator vectorEnd = countriesVector.end();
		int internalIdx = 0;
		while (currElem != vectorEnd)
		{
			countriesMap.insert(pair<int, int>(*currElem, internalIdx++));
			currElem++;
		}
		isAllocated = true;
	}

	~countrySummaryBaseT()
	{
		dynamicFree(data);
		dynamicFree(count);
	}

	//////////////////////////////////////////////////////////////////////////

	void reset()
	{
		dynamicSetZeros(structT, data, numCountries * numYears);
		dynamicSetZeros(int, count, numCountries * numYears);
	}

	void set(int countryIdx, int year, structT mStruct)
	{
		size_t idx = getElemendIdx(countryIdx, year);
		data[idx] = mStruct;
		count[idx] = 1;
	}

	void inc(int countryIdx, int year, structT mStruct)
	{
		size_t idx = getElemendIdx(countryIdx, year);
		data[idx] = data[idx] + mStruct;
		count[idx]++;
	}

	// TODO: if country not in list
	structT get(int countryIdx, int year)
	{
		size_t idx = getElemendIdx(countryIdx, year);
		return data[idx];
	}

	structT getAvg(int countryIdx, int year)
	{
		size_t idx = getElemendIdx(countryIdx, year);
		return data[idx] / count[idx];
	}
};

#endif