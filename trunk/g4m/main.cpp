#include "g4m.h"

int main(int argc, char * argv[])
{
	


	inCellDataT/*<double>*/ inCellData[1];
	inCommonDataT/*<double>*/ inCommonData;
	outCellDataT/*<double>*/ outCellData[1];

	int numCells = 1;

	parallelExecute<inCellDataT, inCommonDataT, outCellDataT>(&computeCell<inCellDataT, inCommonDataT, outCellDataT>, 
		inCellData, inCommonData, outCellData, numCells);

	return 0;
}
