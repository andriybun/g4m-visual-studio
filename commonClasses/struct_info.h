#ifndef STRUCT_INFO_H_
#define STRUCT_INFO_H_

#include "staticArray.h"

#define add_variable(x) _add_struct_variable(x,#x);

#include <string>
#include <deque>

//////////////////////////////////////////////////////////////////////////
class var_info_t
{
public:
	// Properties
	std::string name;
	std::string type;
	size_t type_id;
	size_t offset;
	size_t size;
	size_t dims_count;
	size_t * dims_length;
	// Methods:
	var_info_t() {};
	~var_info_t()
	{
		if (dims_count > 0)
		{
			delete [] dims_length;
		}
	}
	var_info_t(const var_info_t & orig)
	{
		name = orig.name;
		type = orig.type;
		type_id = orig.type_id;
		offset = orig.offset;
		size = orig.size;
		dims_count = orig.dims_count;
		dims_length = new size_t[dims_count];
		memcpy(dims_length, orig.dims_length, sizeof(size_t) * dims_count);
	}

	var_info_t & operator= (const var_info_t & orig)
	{
		if (this != &orig)
		{
			name = orig.name;
			type = orig.type;
			type_id = orig.type_id;
			offset = orig.offset;
			size = orig.size;
			dims_count = orig.dims_count;
			dims_length = new size_t[dims_count];
			memcpy(dims_length, orig.dims_length, sizeof(size_t) * dims_count);
		}
		return * this;
	}
};

//////////////////////////////////////////////////////////////////////////
template <typename struct_t>
class struct_info_t
{
private:
	size_t m_struct_size;
	std::deque<var_info_t> struct_var_info;
public:
	struct_t m_struct;

	struct_info_t() 
	{
		m_struct_size = sizeof(struct_t);
		define_metadata();
	}

	void define_metadata();

	template <typename var_t>
	void _add_struct_variable(var_t &variable, std::string var_name)
	{
		var_info_t type_info;
		type_info.name = get_var_name(var_name);
		type_info.type = typeid(variable).name();
		type_info.type_id = get_type_id_by_type_name(type_info.type);
		type_info.size = sizeof(variable);
		type_info.offset = (char*)&variable - (char*)&m_struct;
		type_info.dims_count = 0;
		type_info.dims_length = NULL;
		struct_var_info.push_back(type_info);
	}

	template <typename var_t, int DIM>
	void _add_struct_variable(arrayRef(var_t, var_array, DIM), std::string var_name)
	{
		var_info_t type_info;
		type_info.name = get_var_name(var_name);
		type_info.type = typeid(var_array[0]).name();
		type_info.type_id = get_type_id_by_type_name(type_info.type);
		type_info.size = sizeof(var_array[0]);
		type_info.offset = (char*)&var_array - (char*)&m_struct;
		type_info.dims_count = 1;
		type_info.dims_length = new size_t[1];
		type_info.dims_length[0] = DIM;
		struct_var_info.push_back(type_info);
	}

	std::string get_var_name(std::string &var_full_name)
	{
		std::string short_name = "";
		if (var_full_name.empty() == false)
		{
			short char_aligment = 1;
			int dot_pos = (int)var_full_name.rfind('.', var_full_name.size() - 1);
			if (dot_pos < 1) // try ->
			{
				char_aligment = 2;
				dot_pos = (int)var_full_name.rfind("->", var_full_name.size() - 1);
			}
			if (dot_pos > 0)
			{
				short_name = var_full_name.substr(dot_pos + char_aligment, var_full_name.size() - dot_pos);
			}
		}
		return short_name;
	}

	size_t get_type_id_by_type_name(std::string type_name)
	{
		if (type_name == "double")
		{
			return 1;
		}
		else if (type_name == "float")
		{
			return 2;
		}
		else if (type_name == "int")
		{
			return 3;
		}
		else if (type_name == "char")
		{
			return 4;
		}
		else if (type_name == "bool")
		{
			return 5;
		}
	}

	size_t get_var_offset(std::string var_name, size_t idx = 0)
	{
		for (int i = 0; i < struct_var_info.size(); i++)
		{
			if (struct_var_info[i].name == var_name)
			{
				size_t var_offset = struct_var_info[i].offset;
				var_offset += (idx == 0) ? 0 : (struct_var_info[i].size * idx);
				return var_offset;
			}
		}
		return -1;
	}

	size_t get_var_type_id(std::string var_name)
	{
		for (int i = 0; i < struct_var_info.size(); i++)
		{
			if (struct_var_info[i].name == var_name)
			{
				return struct_var_info[i].type_id;
			}
		}
		return -1;
	}

};

#endif