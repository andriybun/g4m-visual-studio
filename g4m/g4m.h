#ifndef G4M_H_
#define G4M_H_

//#define DEBUG_ARRAYS
//#define SERIAL_EXECUTION

#include "jDataViewClasses.h"
#include "commonClasses.h"

#include "staticArray.h"
#include "dynamicArray.h"

#include "threading.h"
#include "timer.h"

// g4m specific
#include "g4mIntf.h"
#include "inputFileInfo.h"
#include "computeCell.h"

//////////////////////////////////////////////////////////////////////////
// Function prototypes
//////////////////////////////////////////////////////////////////////////

template <class realT>
int readInputs(inputFileInfoT info, dynamicArrayRef(inCellDataT<realT>, inCellData), 
			   inCommonDataT<realT> &inCommonData, dynamicArrayRef(outCellDataT<realT>, outCellData));


#endif