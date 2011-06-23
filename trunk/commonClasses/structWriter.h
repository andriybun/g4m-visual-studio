#ifndef STRUCT_WRITER_H_
#define STRUCT_WRITER_H_

#include <string>
#include <deque>

#include "struct_info.h"

struct var_info_lite_t
{
	size_t offset;
	size_t type_id;
};

template < typename structT >
class structWriter
{
private:
	std::deque<var_info_lite_t> varsInfoHolder;
public:
	structWriter(void) {}
	~structWriter(void) {}

	void addOutputParam(std::string param_name)
	{
		var_info_lite_t varInfo;
		struct_info_t< structT > mStructInfo;
		varInfo.offset = mStructInfo.get_var_offset(param_name);
		varInfo.type_id = mStructInfo.get_var_type_id(param_name);
		varsInfoHolder.push_back(varInfo);
	}

	void getData(structT &mStruct)
	{
		std::deque<var_info_lite_t>::iterator firstVar = varsInfoHolder.begin();
		std::deque<var_info_lite_t>::iterator it = firstVar;
		while (it != varsInfoHolder.end())
		{
			double output;
			switch (it->type_id)
			{
			case 1:
				output = *((double *)((char *)(&mStruct) + it->offset));
				break;
			case 2:
				output = *((float *)((char *)(&mStruct) + it->offset));
				break;
			case 3:
				output = *((int *)((char *)(&mStruct) + it->offset));
				break;
			case 4:
				output = *((char *)((char *)(&mStruct) + it->offset));
				break;
			case 5:
				output = *((bool *)((char *)(&mStruct) + it->offset));
				break;
			}
			printf("%f\n", output);
			it++;
		}
	}
};

#endif