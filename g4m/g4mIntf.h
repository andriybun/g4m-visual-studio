#ifndef G4M_INTF_H_
#define G4M_INTF_H_

#include "staticArray.h"

#define STRUCTDEF(NAME)				struct NAME
#define CONSTDEF(NAME, VALUE)		const int NAME = VALUE;

template <class realT>
STRUCTDEF(inCellDataT)
{
	realT x;
	realT y;

	int countryIdx;
};

template <class realT>
STRUCTDEF(inCommonDataT)
{
	// Parameters
	int numCells;
	int numCountries;
	int beginYear;
	int endYear;

	// Runtime data
	int year;

};

template <class realT>
STRUCTDEF(outCellDataT)
{
	realT forestArea;
	realT forestShare;
};

#endif