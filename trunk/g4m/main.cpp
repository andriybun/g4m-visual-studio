#include "g4m.h"

template <class realT>
int computeModel();

int main(int argc, char * argv[])
{
	computeModel<double>();

	return 0;
}

template <class realT>
int computeModel()
{
	dynamicArray(inCellDataT<realT>, inCellData);
	dynamicArray(outCellDataT<realT>, outCellData);
	inCommonDataT<realT> inCommonData;

	int numCells = 1;

	dynamicAllocate(inCellData, inCellDataT<realT>, numCells);
	dynamicAllocate(outCellData, outCellDataT<realT>, numCells);

	parallelExecute< realT, inCellDataT<realT>, inCommonDataT<realT>, outCellDataT<realT> > (
		&computeCell<realT>, inCellData, inCommonData, outCellData, numCells);

	dynamicFree(inCellData);
	dynamicFree(outCellData);

	return 0;
};