#ifndef MODEL_H_
#define MODEL_H_

#include <cmath>

#include "intf.h"

#define min(x, y) ((x)<(y) ? (x):(y))
#define max(x, y) ((x)<(y) ? (y):(x))

template <class inDataT, class outDataT>
int modelFunc(inDataT &inData, outDataT &outData)
{
	outData = inData; // stopgap
	return 0;
}

#endif