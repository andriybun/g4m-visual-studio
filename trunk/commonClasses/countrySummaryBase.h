#ifndef COUNTRY_SUMMARY_BASE_H_
#define COUNTRY_SUMMARY_BASE_H_
// This class requires definition of basic arithmetic operators for structs used as template parameters

#include <vector>
#include <map>
#include <assert.h>

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

	virtual int getElemendIdx(int countryIdx, int year)
	{
		assert((year >= beginYear) && (year <= endYear));
		map<int, int>::iterator found = countriesMap.find(countryIdx);
		if (found == countriesMap.end())
		{
			return -1;
		}
		else
		{
			return (year - beginYear) * numCountries + (found->second);
		}
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

	/*virtual */~countrySummaryBaseT()
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

	int set(int countryIdx, int year, structT mStruct)
	{
		int idx = getElemendIdx(countryIdx, year);
		if (idx < 0) return 1;
		data[idx] = mStruct;
		count[idx] = 1;
		return 0;
	}

	int inc(int countryIdx, int year, structT mStruct)
	{
		int idx = getElemendIdx(countryIdx, year);
		if (idx < 0) return 1;
		data[idx] = data[idx] + mStruct;
		count[idx]++;
		return 0;
	}

	structT & get(int countryIdx, int year)
	{
		int idx = getElemendIdx(countryIdx, year);
		return data[idx];
	}

	structT & getAvg(int countryIdx, int year)
	{
		int idx = getElemendIdx(countryIdx, year);
		return data[idx] / count[idx];
	}
};

#endif