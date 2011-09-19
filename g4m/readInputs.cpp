#include <cmath>

#include "g4m.h"

template <class realT>
int readInputs(inputFileInfoT info,
			   dataContainersHolder<realT> & pHolder,
			   vector<int> & countries,
			   dynamicArrayRef(inCellDataT<realT>, inCellData), 
			   inCommonDataT<realT> &inCommonData,
			   dynamicArrayRef(outCellDataT<realT>, outCellData))
{
	// stopgap:
	int xMin = - 179;
	int xMax = 180;
	int yMin = -55;
	int yMax = 83;
	//int numCells = (2 * (xMax - xMin) + 1) * (2 * (yMax - yMin) + 1);
	//int numCountries = 10;

	inCommonData.beginYear = 2000;
	inCommonData.endYear = 2010;

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
	resultsDistribution.push_back(DISTRIBUTE_EQUAL);
	countries.push_back(1);
	countries.push_back(2);
	countries.push_back(3);
	countries.push_back(4);
	countries.push_back(5);
	countries.push_back(6);
	countries.push_back(7);
	countries.push_back(8);
	countries.push_back(9);
	countries.push_back(0);
	// end stopgap

	// Initialize data containers and writers to maps/tables
	if (info.produceMaps)
	{
		pHolder.simuData = new simUnitsData(info.folders.workingDir + info.files.simuBinFileName);
		pHolder.simuDataWriter = new structToMapWriterT< outCellDataT<realT> >(*pHolder.simuData);
		// Initialize dimensions of SIMU data class
		pHolder.simuData->addDim("year", years);
		pHolder.simuData->addDim("value", results);
		// Initialize writers of outputs
		vector<string>::iterator it = results.begin();
		vector<distribute_value_t>::iterator itDistr = resultsDistribution.begin();
		while (it != results.end())
		{
			pHolder.simuDataWriter->addOutputParam(*it, *itDistr);
			it++;
			itDistr++;
		}
	}
	if (info.produceTables)
	{
		pHolder.countrySummaryTable = new tableData;
		pHolder.countrySummaryWriter = new structToTableWriterT< outCellDataT<realT> >(*pHolder.countrySummaryTable);
		pHolder.outCountrySummaryData = new countrySummaryT< outCellDataT<realT> >;
		pHolder.outCountrySummaryData->allocate(countries, inCommonData.beginYear, inCommonData.endYear);
		// Initialize dimensions of table data class
		pHolder.countrySummaryTable->addDim("year", years);
		pHolder.countrySummaryTable->addDim("country", countries);
		pHolder.countrySummaryTable->addDim("value", results);
		// Initialize writers of outputs
		vector<string>::iterator it = results.begin();
		vector<distribute_value_t>::iterator itDistr = resultsDistribution.begin();
		while (it != results.end())
		{
			pHolder.countrySummaryWriter->addOutputParam(*it, *itDistr);
			it++;
			itDistr++;
		}
	}
	
	// Helper vectors and variables needed for filtering cells for active countries
	vector< inCellDataT<realT> > tmpData;
	vector<bool> useCell;
	int numActiveCountries = countries.size();
	int numActiveCells = 0;

	// Creating a helper set with indices of active countries to simplify search of a country idx
	set<int> countriesSet;
	for (unsigned int idx = 0; idx < countries.size(); idx++)
	{
		countriesSet.insert(countries[idx]);
	}

	// stopgap:
	int numCells = 0;

	// First we fill in temporary data for all countries
	for (int x = xMin * (realT)2; x <= xMax * (realT)2; x++)
	{
		for (int y = yMin * (realT)2; y <= yMax * (realT)2; y++)
		{
			double xx = x / (realT)2;
			double yy = y / (realT)2;
			inCellDataT<realT> cell;
			cell.x = xx;
			cell.y = yy;
			cell.countryIdx = fabs((realT)(((x + y) / 10) % 10));
			//cell.simu = pHolder.simuData->getSimUnitsMap().getSimu(cell.x, cell.y);
			vector<simUnitsMap::simu_info_struct_t> cellSimuInfo;
			pHolder.simuData->getSimUnitsMap().getSimuInfoByXY(cell.x, cell.y, cellSimuInfo);
			for (unsigned int simuIdx = 0; simuIdx < cellSimuInfo.size(); simuIdx++)
			{
				cell.simu = cellSimuInfo[simuIdx].simu;
				cell.area = cellSimuInfo[simuIdx].simuFraction;
				tmpData.push_back(cell);
				useCell.push_back(false);
				numCells++;
				// get number of cells belonging to active countries
				if (countriesSet.find(cell.countryIdx) != countriesSet.end())
				{
					numActiveCells++;
					useCell[numCells-1] = true;
				}
			}
		}
	}

	// end stopgap

	// Filling data containers with input data
	inCommonData.numCells = numActiveCells;
	inCommonData.numCountries = numActiveCells;

	// Allocate memory only for active cells
	dynamicAllocate(inCellDataT<realT>, inCellData, numActiveCells);
	dynamicAllocate(outCellDataT<realT>, outCellData, numActiveCells);

	// Fill in cell data only for active cells
	unsigned int activeCellIdx = 0;
	for (unsigned int cellIdx = 0; cellIdx < numCells; cellIdx++)
	{
		if (useCell[cellIdx])
		{
			inCellData[activeCellIdx] = tmpData[cellIdx];
			activeCellIdx++;
		}
	}

	return 0;
}

template int readInputs<float>(inputFileInfoT info,
							   dataContainersHolder<float> & pHolder,
							   vector<int> & countries,
							   dynamicArrayRef(inCellDataT<float>, inCellData), 
							   inCommonDataT<float> &inCommonData,
							   dynamicArrayRef(outCellDataT<float>, outCellData));

template int readInputs<double>(inputFileInfoT info,
								dataContainersHolder<double> & pHolder,
								vector<int> & countries,
								dynamicArrayRef(inCellDataT<double>, inCellData),
								inCommonDataT<double> &inCommonData, 
								dynamicArrayRef(outCellDataT<double>, outCellData));
