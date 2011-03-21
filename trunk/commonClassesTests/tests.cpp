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
	countryData obj;
	obj.readFromFile("data/country_data.txt");

	//obj.set(3, 2000, 125.334);
	//obj.set(3, 2001, 24498.2);
	//obj.set(3, 2002, 24855.8);
	//obj.set(3, 2003, 25259.2);
	//obj.set(3, 2004, 25668.2);
	//obj.set(3, 2005, 26065.4);
	//obj.set(3, 2006, 26443.7);
	//obj.set(3, 2007, 26800.5);
	//obj.set(3, 2008, 27135.4);
	//obj.set(3, 2009, 27449.2);
	//obj.set(3, 2010, 27743);

	//obj.set(17, 2000, 5996.98);
	//obj.set(17, 2001, 5997.82);
	//obj.set(17, 2002, 5992.3);
	//obj.set(17, 2003, 5993.03);
	//obj.set(17, 2004, 5993.71);
	//obj.set(17, 2005, 5994.34);
	//obj.set(17, 2006, 5994.93);
	//obj.set(17, 2007, 5787.18);
	//obj.set(17, 2008, 5622.92);
	//obj.set(17, 2009, 5306.21);
	//obj.set(17, 2010, 5305.54);

	//cout << obj.getRegionSum(10, 2009) << endl;
	//cout << obj.getAvg(1, 2008) << endl;

	{
		countryData avgRes = obj.getSmoothAvg(5);
		avgRes.printToFile("data/test.txt", 2000, 2010, 1);

		int countryIdx = 18;
		cout << avgRes.get(countryIdx, 2000) << endl;
		cout << avgRes.get(countryIdx, 2001) << endl;
		cout << avgRes.get(countryIdx, 2002) << endl;
		cout << avgRes.get(countryIdx, 2003) << endl;
		cout << avgRes.get(countryIdx, 2004) << endl;
		cout << avgRes.get(countryIdx, 2005) << endl;
		cout << avgRes.get(countryIdx, 2006) << endl;
		cout << avgRes.get(countryIdx, 2007) << endl;
		cout << avgRes.get(countryIdx, 2008) << endl;
		cout << avgRes.get(countryIdx, 2009) << endl;
		cout << avgRes.get(countryIdx, 2010) << endl;
	}

	//{
	//	countryData avgRes = obj.getTimeAvg(3);
	//	avgRes.printToFile("data/test_t_1.txt", 2008, 2013, 1);
	//}
	//{
	//	countryData avgRes = obj.getTimeAvg(3);
	//	avgRes.printToFile("data/test_t_3.txt", 2008, 2013, 3);
	//}

	//obj.getTimeAvg(1, 5);
	//obj.insertCountryToPrint(1);
	//obj.insertCountryToPrint(2);
	//obj.insertCountryToPrint(3);
	//obj.PrintToFile("--del.txt",2008,2009,1, "AVG");
}



void testVector2d()
{
	int dimN = 10;

	vector2d obj = vector2d(dimN);
	for (int i = 0; i < 4; i++)
	{
		vector<double> tmp;
		for (int j = 0; j < dimN; j++)
		{
			tmp.push_back( i * 10 + j );
		}
		obj.push(tmp);
	}
	obj.saveToFile("-del");

	cout << "TEST: " << endl;
	//vector2d obj = vector2d();
	obj.readFromFile("-del.bin");
	cout << "size of the object: " << obj.size() << endl;
	cout << "size of the 2nd element: " << obj[2].size() << endl;
	cout << "last vector's elements: " << endl;
	vector<double> t = obj.pop();
	vector<double> t1 = obj[1];
	for (int j = 0; j < t1.size(); j++)
	{
		cout << t1[j] << "\t";
	}
	cout << endl;
	system("del -del.bin");
}