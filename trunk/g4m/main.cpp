#include "g4m.h"

// Autogenerated + struct utils
#include "autogenerated_metadata.h"
#include "autogenerated_operators.h"
//#include "structToMapWriter.h"

template <class realT>
int computeModel();

int main(int argc, char * argv[])
{
	Timer stopwatch;
	stopwatch.start();

	computeModel<float>();

	stopwatch.stop();
	printf("Working time: %f\n", stopwatch.elapsedSeconds());

	return 0;
}

template <class realT>
int computeModel()
{
	// Struct with paths to input files and configs
	std::string guiFile = "data/gui_file.txt";
	inputFileInfoT info(guiFile);

	// Declaring outputs and writers of outputs
	vector<int> countries;
	// data for entire Earth grid (like griddata)
	structGrid< outCellDataT<realT> > structGridOut;

	// Holder of data classes and structs
	dataContainersHolder<realT> pHolder(structGridOut);
	
	// Initialize data
	dynamicArray(inCellDataT<realT>, inCellData);
	dynamicArray(outCellDataT<realT>, outCellData);
	inCommonDataT<realT> inCommonData;

	readInputs<realT>(info, pHolder, countries, inCellData, inCommonData, outCellData);

	// main by-years loop
	for (inCommonData.year = inCommonData.beginYear; inCommonData.year <= inCommonData.endYear; inCommonData.year++)
	{
		pHolder.simuData->pointPush(inCommonData.year);
		pHolder.countrySummaryTable->pointPush(inCommonData.year);
		parallelExecute< inCellDataT<realT>, inCommonDataT<realT>, outCellDataT<realT> > (
			&computeCell<realT>, inCellData, inCommonData, outCellData, inCommonData.numCells, (void *)(& pHolder));

		if (pHolder.countrySummaryWriter)
		{
			pHolder.countrySummaryWriter->writeData(*pHolder.outCountrySummaryData, countries);
		}

		pHolder.simuData->pointPop();
		pHolder.countrySummaryTable->pointPop();
	}

	dynamicFree(inCellData);
	dynamicFree(outCellData);

	if (pHolder.simuDataWriter)
	{
		pHolder.simuData->SaveToFile(info.folders.outputDir, info.files.outMapFileName);
		delete pHolder.simuData;
		delete pHolder.simuDataWriter;
	}
	if (pHolder.countrySummaryWriter)
	{
		pHolder.countrySummaryTable->SaveToFile(info.folders.outputDir, info.files.outTableFileName);
		delete pHolder.countrySummaryTable;
		delete pHolder.countrySummaryWriter;
		delete pHolder.outCountrySummaryData;
	}

	return 0;
};