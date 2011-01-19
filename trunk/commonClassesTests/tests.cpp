#include "tests.h"

void testGriddata2()
{
	// using class constructor:
	// griddata2<DATA_TYPE> OBJECT_NAME = griddata2<DATA_TYPE>(LONGITUDE_RES,LATTITUDE_RES,DEFAULT_VALUE);
	griddata2<unsigned char> OBJ = griddata2<unsigned char>(36,18,99);
	// OBJ.ShowArray();
	// using default class constructor:
	// griddata2<DATA_TYPE> OBJECT_NAME;
	// default parameters are:
	//         LONGITUDE_RES = 720
	//         LATTITUDE_RES = 360
	//         DEFAULT_VALUE = 0
	griddata2<double> OBJ_DEF;
	vector< griddata2<int>  * > vecGridData, newVec;
	for (int i = 0; i < 10; i++)
	{
		griddata2<int> *newEl = new griddata2<int>(2, 2, i);
		vecGridData.push_back(newEl);
	}
	newVec;
	for (int i = 0; i < 10; i++)
	{
		griddata2<int> * newEl = new griddata2<int>;
		*newEl = *vecGridData[i];
		newVec.push_back(newEl);
	}

	for (int i = 0; i < 10; i++)
	{
		cout << vecGridData[i] << endl;
		cout << newVec[i] << endl;
		newVec[i]->ShowArray();
	}
	vecGridData[2]->set(1, 1, -90);
	vecGridData[2]->ShowArray();
	newVec[2]->ShowArray();
}