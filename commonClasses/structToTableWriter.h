#ifndef STRUCT_TO_TABLE_WRITER_H_
#define STRUCT_TO_TABLE_WRITER_H_

#include <string>
#include <deque>
#include <vector>
#include <assert.h>

#include "structWriter.h"
#include "countrySummaryBase.h"
#include "../jDataViewClasses/jDataViewClasses.h"

template < typename structT >
class structToTableWriterT : public structWriterT<structT>
{
private:
	tableData & tbData;
public:
	structToTableWriterT(tableData & tbData): tbData(tbData) {}
	~structToTableWriterT(void) {}

	void addOutputParam(std::string param_name/*, distribute_value_t distribute_value = DISTRIBUTE_PROPORTIONALLY*/)
	{
		var_info_lite_t varInfo;
		struct_info_t< structT > mStructInfo;
		varInfo.offset = mStructInfo.get_var_offset(param_name);
		varInfo.type_id = mStructInfo.get_var_type_id(param_name);
		varInfo.simuDataPos = tbData.descr.getCoordinate(tbData.descr.getNDims()-1, param_name);
		assert(varInfo.simuDataPos >= 0);
		//varInfo.distribute_value = distribute_value;
		varsInfoHolder.push_back(varInfo);
	}

	void writeData(structT &mStruct)
	{
		std::deque<var_info_lite_t>::iterator firstVar = varsInfoHolder.begin();
		std::deque<var_info_lite_t>::iterator it = firstVar;
		while (it != varsInfoHolder.end())
		{
			float output = getValue(mStruct, it);
			tbData.insert(it->simuDataPos, output);
			it++;
		}
	}

	void writeData(countrySummaryT< structT > & outCountrySummaryData, vector<int> & countries)
	{
		int year = atoi(tbData.point[tbData.point.size()-1].c_str());
		for (unsigned int countryIdx = 0; countryIdx < countries.size(); countryIdx++)
		{
			tbData.pointPush(countryIdx);
			writeData(outCountrySummaryData.get(countries[countryIdx], year));			
			tbData.pointPop();
		}
	}
};

#endif