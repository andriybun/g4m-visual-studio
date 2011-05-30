#ifndef G4M_INTF_H_
#define G4M_INTF_H_

#include "staticArray.h"



template <class realT>
struct inCellDataT
{
	// TODO: remove
	realT STOPGAP;
};

template <class realT>
struct inCommonDataT
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
struct outCellDataT
{
	// TODO: remove
	realT STOPGAP;
};

#endif