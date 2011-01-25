#include <iostream>
#include <vector>

#include "tests.h"

using namespace std;

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



void testCountryData()
{
	countryData obj = countryData();
	for (int i = 0; i < 200; i++)
	{
		obj.set(i, 2008, i*i-2*i);
	}
	for (int i = 0; i < 200; i++)
	{
		obj.inc(i, 2009, i*i*i-4*i);
	}
	for (int i = 0; i < 200; i++)
	{
		obj.inc(i, 2009, -i*i+2*i);
	}
	//  obj.set(1, 2008, 6);
	//  obj.set(1, 2009, 5);
	//  obj.inc(1, 2009, 4);
	//  obj.set(2, 2008, 1);
	//  obj.set(2, 2009, 2);
	//  obj.inc(3, 2009, 3);
	//  cout << obj.getRegionSum(20, 2009) << endl;
	//  cout << obj.getRegionSum(20, 2010) << endl;
	//  cout << obj.getRegionSum(17, 2008) << endl;
	//  cout << obj.getRegionSum(17, 2009) << endl;
	//  cout << "********************" << endl;
	//  cout << obj.getAvg(1, 2008) << endl;
	//  cout << obj.getAvg(1, 2009) << endl;
	//  cout << obj.getAvg(2, 2008) << endl;
	//  cout << obj.getAvg(3, 2009) << endl;
	//  cout << "********************" << endl;
	//  obj.insertCountryToPrint(1);
	//  obj.insertCountryToPrint(2);
	//  obj.insertCountryToPrint(3);
	obj.PrintToFile("--del.txt",2008,2009,1, "AVG");
}

