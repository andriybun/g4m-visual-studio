#ifndef G4M_INTF_H_
#define G4M_INTF_H_

#include "staticArray.h"

#define STRUCTDEF(NAME)				struct NAME
#define CONSTDEF(NAME, VALUE)		const int NAME = VALUE;

CONSTDEF(MAX_NUM_COUNTRIES, 300)

template <class realT>
STRUCTDEF(inCellDataT)
{
	realT x;
	realT y;

	int simu;
	int countryIdx;

	realT area;
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