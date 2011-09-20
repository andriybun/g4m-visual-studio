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
	// Read parameters of the current run from *.xml file
	xmlParser paramsParser(info.files.outputParametersFileName);
	vector<int> years;
	vector<string> results;
	vector<distribute_value_t> resultsDistribution;
	vector<xmlData::paramsMapT> dummy;
	vector<xmlData::paramsMapT> resultsDistributionRaw;
	paramsParser.getSubtags("dim", years, dummy, pair<string, string>("name", "years"), "value");
	paramsParser.getSubtags("dim", countries, dummy, pair<string, string>("name", "countries"), "value");
	paramsParser.getSubtags("dim", results, resultsDistributionRaw, pair<string, string>("name", "params"), "value");
	for (size_t idx = 0; idx < resultsDistributionRaw.size(); idx++)
	{
		if (resultsDistributionRaw[idx]["distribute"] == "proportionally")
		{
			resultsDistribution.push_back(DISTRIBUTE_PROPORTIONALLY);
		}
		else if (resultsDistributionRaw[idx]["distribute"] == "equal")
		{
			resultsDistribution.push_back(DISTRIBUTE_EQUAL);
		}
		else
		{
			assert(false);
		}
	}

	// Initialize data containers and writers to maps/tables
	if (info.produceMaps)
	{
		pHolder.simuData = new simUnitsData(info.files.simuBinFileName);
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
	
	// Helper vectors and variables needed for filtering cells for active countries
	vector< inCellDataT<realT> > tmpData;
	vector<bool> useCell;
	int numActiveCells = 0;
	int numCells = 0;

	// Creating a helper set with indices of active countries to simplify search of a country idx
	set<int> countriesSet;
	for (unsigned int idx = 0; idx < countries.size(); idx++)
	{
		countriesSet.insert(countries[idx]);
	}

	g4mLoad<realT>(countriesSet, pHolder, inCommonData, tmpData, useCell, numActiveCells, numCells);

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
