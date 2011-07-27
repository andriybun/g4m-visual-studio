#include <cmath>

#include "g4m.h"
#include "g4mIntf.h"
#include "structToMapWriter.h"

#define min(x, y) ((x)<(y) ? (x):(y))
#define max(x, y) ((x)<(y) ? (y):(x))

template <class realT>
int computeCell(const inCellDataT<realT> &inCellData,
				const inCommonDataT<realT> &inCommonData,
				outCellDataT<realT> &outCellData,
				void * params)
{
	structToMapWriterT< outCellDataT<realT> > * simuDataWriter = (*((dataContainersHolder<realT> *)params)).simuDataWriter;
	countrySummaryT< outCellDataT<realT> > * outCountrySummaryData = (*((dataContainersHolder<realT> *)params)).outCountrySummaryData;

	// Stopgap:
	outCellData.forestArea = 20;
	outCellData.forestShare = outCellData.forestArea * inCellData.area;
	// end stopgap

	if (simuDataWriter)
	{
		// Two ways are possible  /* not proved yet!!! */, however latter one is almost 2 times faster
		//simuDataWriter->writeData(inCellData.x, inCellData.y, outCellData);
		simuDataWriter->writeData(inCellData.simu, outCellData);
	}

	if (outCountrySummaryData)
	{
		outCountrySummaryData->inc(inCellData.countryIdx, inCommonData.year, outCellData);
	}

	return 0;
}

template int computeCell<float>(const inCellDataT<float> &inCellData,
								const inCommonDataT<float> &inCommonData, 
								outCellDataT<float> &outCellData,
								void * params);
template int computeCell<double>(const inCellDataT<double> &inCellData,
								 const inCommonDataT<double> &inCommonData,
								 outCellDataT<double> &outCellData,
								 void * params);