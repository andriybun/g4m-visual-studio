#ifndef COMPUTE_CELL_H_
#define COMPUTE_CELL_H_

#include <cmath>

#include "g4mIntf.h"

#define min(x, y) ((x)<(y) ? (x):(y))
#define max(x, y) ((x)<(y) ? (y):(x))

template <class realT>
int computeCell(const inCellDataT<realT> &inCellData, const inCommonDataT<realT> &inCommonData, outCellDataT<realT> &outCellData)
{
	outCellData.STOPGAP = inCellData.STOPGAP * inCommonData.STOPGAP;
	return 0;
}

#endif