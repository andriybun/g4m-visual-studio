#include "g4m.h"

template <class realT>
int g4mCompute(const inCellDataT<realT> &inCellData,
				const inCommonDataT<realT> &inCommonData,
				outCellDataT<realT> &outCellData)
{
	// Stopgap:
	outCellData.forestArea = 70;
	outCellData.forestShare = outCellData.forestArea * inCellData.area;
	// end stopgap

	return 0;
}

template int g4mCompute<float>(const inCellDataT<float> &inCellData,
								const inCommonDataT<float> &inCommonData, 
								outCellDataT<float> &outCellData);
template int g4mCompute<double>(const inCellDataT<double> &inCellData,
								 const inCommonDataT<double> &inCommonData,
								 outCellDataT<double> &outCellData);