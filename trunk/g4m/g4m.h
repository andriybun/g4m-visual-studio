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

// data containers and writers
#include "countrySummary.h"
#include "structToMapWriter.h"
#include "structToTableWriter.h"

template <class realT>
struct dataContainersHolder
{
	structToMapWriterT< outCellDataT<realT> > & simuWriter;
	//countrySummaryBaseT< outCellDataT<realT> > &outCountrySummaryData;
	countrySummaryT< outCellDataT<realT> > &outCountrySummaryData;

	dataContainersHolder(structToMapWriterT< outCellDataT<realT> > & simuWriter /*= NULL*/
		//, countrySummaryBaseT< outCellDataT<realT> > &outCountrySummaryData /*= NULL*/
		, countrySummaryT< outCellDataT<realT> > &outCountrySummaryData /*= NULL*/
		)
		: simuWriter(simuWriter)
		, outCountrySummaryData(outCountrySummaryData)
	{}
};

//////////////////////////////////////////////////////////////////////////
// Function prototypes
//////////////////////////////////////////////////////////////////////////

template <class realT>
int readInputs(inputFileInfoT info,
			   simUnitsData simuData,
			   dynamicArrayRef(inCellDataT<realT>, inCellData), 
			   inCommonDataT<realT> &inCommonData,
			   dynamicArrayRef(outCellDataT<realT>, outCellData),
			   //countrySummaryBaseT< outCellDataT<realT> > & outCountrySummaryData);
			   countrySummaryT< outCellDataT<realT> > & outCountrySummaryData);

template <class realT>
int computeCell(void * params,
				const inCellDataT<realT> &inCellData,
				const inCommonDataT<realT> &inCommonData,
				outCellDataT<realT> &outCellData);


#endif