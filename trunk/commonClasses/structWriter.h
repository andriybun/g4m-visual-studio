#ifndef STRUCT_WRITER_H_
#define STRUCT_WRITER_H_

#include <string>
#include <deque>
#include <assert.h>

#include "struct_info.h"

struct var_info_lite_t
{
	size_t offset;
	size_t type_id;
	size_t simuDataPos;
	distribute_value_t distribute_value;
};

template < typename structT >
class structWriterT
{
protected:
	std::deque<var_info_lite_t> varsInfoHolder;
public:
	structWriterT() {}
	~structWriterT(void) {}

	float getValue(structT & mStruct, std::deque<var_info_lite_t>::iterator & it)
	{
		float output;
		switch (it->type_id)
		{
		case 1:
			output = (float)(*((double *)((char *)(&mStruct) + it->offset)));
			break;
		case 2:
			output = *((float *)((char *)(&mStruct) + it->offset));
			break;
		case 3:
			output = (float)(*((int *)((char *)(&mStruct) + it->offset)));
			break;
		case 4:
			output = (float)(*((char *)((char *)(&mStruct) + it->offset)));
			break;
		case 5:
			output = (float)(*((bool *)((char *)(&mStruct) + it->offset)));
			break;
		}
		return output;
	}

};

#endif