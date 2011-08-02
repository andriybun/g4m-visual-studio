//   Name:          GridData class lite + any type - previous
//   Author:        Andriy Bun;
//   Date:          18.11.2009

#ifndef GRIDDATALITE_H_
#define GRIDDATALITE_H_

#include <iostream>
#include <cstdlib>
#include <string>
#include <string.h>
#include <fstream>
#include <typeinfo>

using namespace std;

template <class T>
class griddataLite
{
private:
	int HorResolution;
	int VerResolution;
	T *GridRows;
	T *grid;
public:
	griddataLite(int HR, int VR, T val);
	griddataLite();
	griddataLite(const griddataLite& g);
	~griddataLite();
	void ShowArray();                       // prints array
	void PrintToFile(string fileName, string rastrType);      // print array to file
	void set(int x, int y, T val);     // assigns value val to cell [x][y]
	void inc(int x, int y, T val);     // adds value val to the existing value in cell [x][y]
	T get(int x, int y);               // returns value stored in cell [x][y]
};

  // using class constructor:
  // griddataLite<DATA_TYPE> OBJECT_NAME = griddataLite<DATA_TYPE>(LONGITUDE_RES,LATTITUDE_RES,DEFAULT_VALUE);
  //         EXAMPLE: griddataLite<char> OBJ = griddataLite<char>(36,18,-99.5);
  // using default class constructor:
  // griddataLite<DATA_TYPE> OBJECT_NAME;
  // default parameters are:
  //         LONGITUDE_RES = 720
  //         LATTITUDE_RES = 360
  //         DEFAULT_VALUE = 0
  //         EXAMPLE: griddataLite<double> OBJ_DEF;

#endif
