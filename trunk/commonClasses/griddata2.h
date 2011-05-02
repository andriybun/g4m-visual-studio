//   Name:          GridData class 2 + any type
//   Author:        Andriy Bun; Mykola Gusti
//   Date:          26.08.2009

#ifndef griddata2_h_
#define griddata2_h_

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <typeinfo>

using namespace std;

template <class T>
class griddata2
{
private:
	int HorResolution;
	int VerResolution;
	int HorNeigh;
	int VerNeigh;
	T *GridRows;
	T *grid, *gridPrev;  
public:
	griddata2(int HR, int VR, T val);
	griddata2();
	griddata2(const griddata2& g);
	griddata2 & operator = (const griddata2& g);
	~griddata2();
	void ShowArray();                       // prints array
	void PrintToFile(string fileName, string rastrType = "ESRI");      // print array to file
	void PrintToFilePrev(string fileName, string rastrType = "ESRI");  // print previous year array to file
	void ShowArrayPrev();                   // prints array for the previous year
	void update();                          // updates values for previous year to current values
	void set(int x, int y, T val);     // assigns value val to cell [x][y]
	void setPrev(int x, int y, T val); // assigns previous year value val to cell [x][y]
	void inc(int x, int y, T val);     // adds value val to the existing value in cell [x][y]
	T get(int x, int y);               // returns value stored in cell [x][y]
	T getPrev(int x, int y);           // returns value for the previous year stored in cell [x][y]
	void SetNeighNum(int n, int m);     // sets number of neighbour cells to be considered
	T GetMax(int x, int y);            // returns maximum value of all neighbours for the previous year
	T GetMin(int x, int y);            // returns minimum value of all neighbours for the previous year
	T GetAvg(int x, int y);            // returns average value for the previous year
};

// using class constructor:
// griddata2<DATA_TYPE> OBJECT_NAME = griddata2<DATA_TYPE>(LONGITUDE_RES,LATTITUDE_RES,DEFAULT_VALUE);
//         EXAMPLE: griddata2<char> OBJ = griddata2<char>(36,18,-99.5);
// using default class constructor:
// griddata2<DATA_TYPE> OBJECT_NAME;
// default parameters are:
//         LONGITUDE_RES = 720
//         LATTITUDE_RES = 360
//         DEFAULT_VALUE = 0
//         EXAMPLE: griddata2<double> OBJ_DEF;

#endif
