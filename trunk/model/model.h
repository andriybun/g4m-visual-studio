#ifndef MODEL_H_
#define MODEL_H_

#include "intf.h"

template <class inDataT, class outDataT>
int modelFunc(inDataT &inData, outDataT &outData)
{
	outData = inData; // stopgap
	return 0;
}

#endif