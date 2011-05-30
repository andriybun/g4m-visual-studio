#include "g4m.h"

template <class realT>
int readInputs(inputFileInfoT info, dynamicArrayRef(inCellDataT<realT>, inCellData), 
			   inCommonDataT<realT> &inCommonData, dynamicArrayRef(outCellDataT<realT>, outCellData))
{
	int numCells = 1;

	dynamicAllocate(inCellDataT<realT>, inCellData, numCells);
	dynamicAllocate(outCellDataT<realT>, outCellData, numCells);

	inCellData[0].STOPGAP = 8;
	inCommonData.STOPGAP = 9;
	inCommonData.numCells = numCells;

	return 0;
}

template int readInputs<float>(inputFileInfoT info, dynamicArrayRef(inCellDataT<float>, inCellData), 
							   inCommonDataT<float> &inCommonData, dynamicArrayRef(outCellDataT<float>, outCellData));
template int readInputs<double>(inputFileInfoT info, dynamicArrayRef(inCellDataT<double>, inCellData),
								inCommonDataT<double> &inCommonData, dynamicArrayRef(outCellDataT<double>, outCellData));

