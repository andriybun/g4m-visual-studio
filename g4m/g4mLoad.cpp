#include "g4m.h"

template <class realT>
int g4mLoad(const set<int> & countriesSet,
			const dataContainersHolder<realT> & pHolder,
			inCommonDataT<realT> & inCommonData,
			vector< inCellDataT<realT> > & tmpData,
			vector<bool> & useCell,
			int & numActiveCells,
			int & numCells)
{
	inCommonData.beginYear = 2000;
	inCommonData.endYear = 2010;

	int xMin = - 179;
	int xMax = 180;
	int yMin = -55;
	int yMax = 83;

	// Filling in temporary data for all countries
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
	return 0;
}

template int g4mLoad<float>(const set<int> & countriesSet,
							const dataContainersHolder<float> & pHolder,
							inCommonDataT<float> & inCommonData,
							vector< inCellDataT<float> > & tmpData,
							vector<bool> & useCell,
							int & numActiveCells,
							int & numCells);
template int g4mLoad<double>(const set<int> & countriesSet,
							 const dataContainersHolder<double> & pHolder,
							 inCommonDataT<double> & inCommonData,
							 vector< inCellDataT<double> > & tmpData,
							 vector<bool> & useCell,
							 int & numActiveCells,
							 int & numCells);