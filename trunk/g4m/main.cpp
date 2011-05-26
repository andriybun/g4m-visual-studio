#include "g4m.h"

int main(int argc, char * argv[])
{
	
	array(inCellDataT/*<double>*/, inCellData, 1);
	array(outCellDataT/*<double>*/, outCellData, 1);
	inCommonDataT/*<double>*/ inCommonData;

	int numCells = 1;

	parallelExecute<inCellDataT, inCommonDataT, outCellDataT>(&computeCell<inCellDataT, inCommonDataT, outCellDataT>, 
		inCellData, inCommonData, outCellData, numCells);

	return 0;
}
