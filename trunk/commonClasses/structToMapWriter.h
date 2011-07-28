#ifndef STRUCT_TO_MAP_WRITER_H_
#define STRUCT_TO_MAP_WRITER_H_

#include <string>
#include <deque>
#include <assert.h>

#include "structWriter.h"
#include "../jDataViewClasses/jDataViewClasses.h"

template < typename structT >
class structToMapWriterT : public structWriterT<structT>
{
private:
	simUnitsData & simuData;

	using structWriterT<structT>::varsInfoHolder;
public:
	structToMapWriterT(simUnitsData & simuData): simuData(simuData) {}
	~structToMapWriterT(void) {}

	void addOutputParam(std::string param_name, distribute_value_t distribute_value = DISTRIBUTE_PROPORTIONALLY)
	{
		var_info_lite_t varInfo;
		struct_info_t< structT > mStructInfo;
		varInfo.offset = mStructInfo.get_var_offset(param_name);
		varInfo.type_id = mStructInfo.get_var_type_id(param_name);
		varInfo.simuDataPos = simuData.descr.getCoordinate(simuData.descr.getNDims()-1, param_name);
		assert(varInfo.simuDataPos >= 0);
		varInfo.distribute_value = distribute_value;
		varsInfoHolder.push_back(varInfo);
	}

	void writeData(int SIMU, structT & mStruct)
	{
		std::deque<var_info_lite_t>::iterator firstVar = varsInfoHolder.begin();
		std::deque<var_info_lite_t>::iterator it = firstVar;
		while (it != varsInfoHolder.end())
		{
			float output = getValue(mStruct, it);
			simuData.insertByLastCoord(SIMU, it->simuDataPos, output);
			it++;
		}
	}

	void writeData(double x, double y, structT &mStruct)
	{
		std::deque<var_info_lite_t>::iterator firstVar = varsInfoHolder.begin();
		std::deque<var_info_lite_t>::iterator it = firstVar;
		while (it != varsInfoHolder.end())
		{
			float output = getValue(mStruct, it);
			simuData.insertByLastCoord(x, y, it->simuDataPos, output, it->distribute_value);
			it++;
		}
	}
};

#endif