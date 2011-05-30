#include "g4m.h"

template <class realT>
int computeModel();

int main(int argc, char * argv[])
{
	Timer stopwatch;
	stopwatch.start();

	computeModel<double>();

	stopwatch.stop();
	printf("Working time: %f\n", stopwatch.elapsedSeconds());

	return 0;
}

template <class realT>
int computeModel()
{
	dynamicArray(inCellDataT<realT>, inCellData);
	dynamicArray(outCellDataT<realT>, outCellData);
	inCommonDataT<realT> inCommonData;

	std::string guiFile = "data/gui_file.txt";

	inputFileInfoT info(guiFile);

	readInputs<realT>(info, inCellData, inCommonData, outCellData);

	for (inCommonData.year = inCommonData.beginYear; inCommonData.year <= inCommonData.endYear; inCommonData.year++)
	{
		parallelExecute< inCellDataT<realT>, inCommonDataT<realT>, outCellDataT<realT> > (
			&computeCell<realT>, inCellData, inCommonData, outCellData, inCommonData.numCells);

	}

	printf("%f\n", outCellData[0].STOPGAP);

	dynamicFree(inCellData);
	dynamicFree(outCellData);

	return 0;
};