#ifndef COUNTRY_BASE_H_
#define COUNTRY_BASE_H_
// This class requires definition of basic arithmetic operators for structs used as template parameters

#include <vector>
#include <map>
#include <assert.h>

#include "countrySummaryBase.h"
#include "autogenerated_operators.h"

template <class structT>
class countrySummaryT : public countrySummaryBaseT<structT>
{
private:
	dynamicArray(int, countriesMap);

	int getElemendIdx(int countryIdx, int year)
	{
		assert((year >= beginYear) && (year <= endYear));
		if (countriesMap[countryIdx] < 0)
		{
			return -1;
		}
		else
		{
			return (year - beginYear) * numCountries + countriesMap[countryIdx];
		}
	}
public:

	/*virtual */~countrySummaryT()
	{
		//dynamicFree(data);
		//dynamicFree(count);
		dynamicFree(countriesMap);
	}

	void allocate(intVectorT &countriesVector, int beginYearIn, int endYearIn)
	{
		numCountries = countriesVector.size();
		beginYear = beginYearIn;
		endYear = endYearIn;
		numYears = endYear - beginYear + 1;
		dynamicAllocateZeros(structT, data, numCountries * numYears);
		dynamicAllocateZeros(int, count, numCountries * numYears);
		dynamicAllocate(int, countriesMap, MAX_NUM_COUNTRIES);
		for (int i = 0; i < MAX_NUM_COUNTRIES; i++)
		{
			countriesMap[i] = -1;
		}
		intVectorT::iterator currElem = countriesVector.begin();
		intVectorT::iterator vectorEnd = countriesVector.end();
		int internalIdx = 0;
		while (currElem != vectorEnd)
		{
			countriesMap[*currElem] = internalIdx++;
			currElem++;
		}
		isAllocated = true;
	}
};

#endif