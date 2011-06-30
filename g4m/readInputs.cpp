#include <cmath>

#include "g4m.h"

template <class realT>
int readInputs(inputFileInfoT info,
			   simUnitsData simuData,
			   dynamicArrayRef(inCellDataT<realT>, inCellData), 
			   inCommonDataT<realT> &inCommonData,
			   dynamicArrayRef(outCellDataT<realT>, outCellData),
			   countrySummaryBaseT< outCellDataT<realT> > & outCountrySummaryData)
{
	// stopgap
	int xMin = - 180;
	int xMax = 180;
	int yMin = -69;
	int yMax = 83;
	int numCells = (xMax - xMin + 2) * (yMax - yMin + 2) * 4;
	int numCountries = 10;

	inCommonData.beginYear = 2000;
	inCommonData.endYear = 2010;

	dynamicAllocate(inCellDataT<realT>, inCellData, numCells);
	dynamicAllocate(outCellDataT<realT>, outCellData, numCells);

	int cellIdx = 0;

	for (int x = xMin * (realT)2; x <= xMax * (realT)2; x++)
	{
		for (int y = yMin * (realT)2; y <= yMax * (realT)2; y++)
		{
			double xx = x / (realT)2;
			double yy = y / (realT)2;
			inCellData[cellIdx].x = xx;
			inCellData[cellIdx].y = yy;
			inCellData[cellIdx].countryIdx = fabs((realT)(((x + y) / 10) % 10));
			cellIdx++;
		}
	}

	inCommonData.numCells = numCells;
	inCommonData.numCountries = numCountries;
	// end stopgap

	return 0;
}

template int readInputs<float>(inputFileInfoT info,
							   simUnitsData simuData,
							   dynamicArrayRef(inCellDataT<float>, inCellData), 
							   inCommonDataT<float> &inCommonData,
							   dynamicArrayRef(outCellDataT<float>, outCellData),
							   countrySummaryBaseT< outCellDataT<float> > & outCountrySummaryData);
template int readInputs<double>(inputFileInfoT info,
								simUnitsData simuData,
								dynamicArrayRef(inCellDataT<double>, inCellData),
								inCommonDataT<double> &inCommonData, 
								dynamicArrayRef(outCellDataT<double>, outCellData),
								countrySummaryBaseT< outCellDataT<double> > & outCountrySummaryData);
