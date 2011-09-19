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
#include "structGrid.h"
#include "countrySummary.h"
#include "structToMapWriter.h"
#include "structToTableWriter.h"

template <class realT>
struct dataContainersHolder
{
	structGrid< outCellDataT<realT> > & structGridOut;
	simUnitsData * simuData;
	tableData * countrySummaryTable;
	structToMapWriterT< outCellDataT<realT> > * simuDataWriter;
	structToTableWriterT< outCellDataT<realT> > * countrySummaryWriter;
	countrySummaryT< outCellDataT<realT> > * outCountrySummaryData;

	dataContainersHolder(structGrid< outCellDataT<realT> > & structGridOut /*= NULL*/
		, simUnitsData * simuData = NULL
		, tableData * countrySummaryTable = NULL
		, structToMapWriterT< outCellDataT<realT> > * simuDataWriter = NULL
		, structToTableWriterT< outCellDataT<realT> > * countrySummaryWriter = NULL
		, countrySummaryT< outCellDataT<realT> > * outCountrySummaryData = NULL
		)
		: simuData(simuData)
		, countrySummaryTable(countrySummaryTable)
		, simuDataWriter(simuDataWriter)
		, countrySummaryWriter(countrySummaryWriter)
		, outCountrySummaryData(outCountrySummaryData)
		, structGridOut(structGridOut)
	{}
};

//////////////////////////////////////////////////////////////////////////
// Function prototypes
//////////////////////////////////////////////////////////////////////////

template <class realT>
int readInputs(inputFileInfoT info,
			   dataContainersHolder<realT> & pHolder,
			   vector<int> & countries,
			   dynamicArrayRef(inCellDataT<realT>, inCellData), 
			   inCommonDataT<realT> &inCommonData,
			   dynamicArrayRef(outCellDataT<realT>, outCellData));

template <class realT>
int computeCell(const inCellDataT<realT> &inCellData,
				const inCommonDataT<realT> &inCommonData,
				outCellDataT<realT> &outCellData,
				void * params);


//////////////////////////////////////////////////////////////////////////
// Model specific functions that should be modified
//////////////////////////////////////////////////////////////////////////

template <class realT>
int g4mCompute(const inCellDataT<realT> &inCellData,
			   const inCommonDataT<realT> &inCommonData,
			   outCellDataT<realT> &outCellData);


#endif