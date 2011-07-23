#include <cmath>

#include "g4m.h"

template <class realT>
int readInputs(inputFileInfoT info,
			   dataContainersHolder<realT> & pHolder,
			   vector<int> countries,
			   dynamicArrayRef(inCellDataT<realT>, inCellData), 
			   inCommonDataT<realT> &inCommonData,
			   dynamicArrayRef(outCellDataT<realT>, outCellData))
{
	// stopgap:
	int xMin = - 180;
	int xMax = 180;
	int yMin = -69;
	int yMax = 83;
	int numCells = (2 * (xMax - xMin) + 1) * (2 * (yMax - yMin) + 1);
	int numCountries = 10;

	inCommonData.beginYear = 2000;
	inCommonData.endYear = 2010;
	// end stopgap

	// Data by simulation units
	// Helper sets
	vector<int> years;
	years.push_back(2000);
	years.push_back(2005);
	years.push_back(2010);
	vector<string> results;
	vector<distribute_value_t> resultsDistribution;
	results.push_back("forestArea");
	resultsDistribution.push_back(DISTRIBUTE_PROPORTIONALLY);
	results.push_back("forestShare");
	resultsDistribution.push_back(IS_CONSTANT);
	countries.push_back(1);
	countries.push_back(2);
	// Initialize dimensions of SIMU data class
	pHolder.simuData.addDim("year", years);
	pHolder.simuData.addDim("value", results);
	// Initialize dimensions of table data class
	pHolder.countrySummaryTable.addDim("year", years);
	pHolder.countrySummaryTable.addDim("country", countries);
	pHolder.countrySummaryTable.addDim("value", results);
	// Initialize writers of outputs
	vector<string>::iterator it = results.begin();
	vector<distribute_value_t>::iterator itDistr = resultsDistribution.begin();
	while (it != results.end())
	{
		pHolder.simuDataWriter.addOutputParam(*it, *itDistr);
		pHolder.countrySummaryWriter.addOutputParam(*it);
		it++;
		itDistr++;
	}
	//pHolder.simuDataWriter.addOutputParam("forestArea", DISTRIBUTE_PROPORTIONALLY);
	//pHolder.simuDataWriter.addOutputParam("forestShare", IS_CONSTANT);
	//pHolder.countrySummaryWriter.addOutputParam("forestArea");
	//pHolder.countrySummaryWriter.addOutputParam("forestShare");

	pHolder.outCountrySummaryData.allocate(countries, inCommonData.beginYear, inCommonData.endYear);

	dynamicAllocate(inCellDataT<realT>, inCellData, numCells);
	dynamicAllocate(outCellDataT<realT>, outCellData, numCells);

	// stopgap:
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
							   dataContainersHolder<float> & pHolder,
							   vector<int> countries,
							   dynamicArrayRef(inCellDataT<float>, inCellData), 
							   inCommonDataT<float> &inCommonData,
							   dynamicArrayRef(outCellDataT<float>, outCellData));

template int readInputs<double>(inputFileInfoT info,
								dataContainersHolder<double> & pHolder,
								vector<int> countries,
								dynamicArrayRef(inCellDataT<double>, inCellData),
								inCommonDataT<double> &inCommonData, 
								dynamicArrayRef(outCellDataT<double>, outCellData));
