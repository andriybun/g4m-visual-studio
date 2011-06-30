#include <cmath>

#include "g4m.h"
#include "g4mIntf.h"
#include "structToMapWriter.h"

#define min(x, y) ((x)<(y) ? (x):(y))
#define max(x, y) ((x)<(y) ? (y):(x))

template <class realT>
int computeCell(void * params, const inCellDataT<realT> &inCellData, const inCommonDataT<realT> &inCommonData, outCellDataT<realT> &outCellData)
{
	structToMapWriterT< outCellDataT<realT> > & simuDataWriter = (*((dataContainersHolder<realT> *)params)).simuWriter;
	//countrySummaryBaseT< outCellDataT<realT> > & outCountrySummaryData = (*((dataContainersHolder<realT> *)params)).outCountrySummaryData;
	countrySummaryT< outCellDataT<realT> > & outCountrySummaryData = (*((dataContainersHolder<realT> *)params)).outCountrySummaryData;

	// Stopgap:
	outCellData.forestArea = 20;
	outCellData.forestShare = 40;
	outCountrySummaryData.inc(inCellData.countryIdx, inCommonData.year, outCellData);
	outCellDataT<realT> tst = outCountrySummaryData.get(inCellData.countryIdx, inCommonData.year);
	// end stopgap

	simuDataWriter.writeData(inCellData.x, inCellData.y, outCellData);

	return 0;
}

template int computeCell<float>(void * params, const inCellDataT<float> &inCellData, const inCommonDataT<float> &inCommonData, outCellDataT<float> &outCellData);
template int computeCell<double>(void * params, const inCellDataT<double> &inCellData, const inCommonDataT<double> &inCommonData, outCellDataT<double> &outCellData);