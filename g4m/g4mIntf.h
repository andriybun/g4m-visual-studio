#ifndef G4M_INTF_H_
#define G4M_INTF_H_

#include "staticArray.h"

#define STRUCTDEF(NAME) struct NAME

template <class realT>
STRUCTDEF(inCellDataT)
{
	// TODO: remove
	realT STOPGAP;
};

template <class realT>
STRUCTDEF(inCommonDataT)
{
	// Parameters
	int numCells;
	int beginYear;
	int endYear;

	// Runtime data
	int year;

	// TODO: remove
	realT STOPGAP;
};

template <class realT>
STRUCTDEF(outCellDataT)
{
	realT forestArea;
	realT forestShare;
	// TODO: remove
	realT STOPGAP;
};

#endif