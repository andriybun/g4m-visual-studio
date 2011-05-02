//  Name: CountryData class with statistics by regions
//  Author: Andriy Bun
//  Date: 29.10.09
//  Description: 
//
//	Modifications:
//      29.10.2009: added selecting countries to print; if no countries specified - prints all countries.
//		21.02.2011: added getTimeAvg, getSmoothAvg methods; renamed PrintToFile method to printToFile, now it takes full file name,
//					doesn't add extension; added copy constructor and assignment operator.


#ifndef COUNTRYDATA_H_
#define COUNTRYDATA_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <set>

#include "ipol.h"

#define xmin(a,b) (((a)<(b))?(a):(b))
#define xmax(a,b) (((a)<(b))?(b):(a))

const int countriesNum = 245;

using namespace std;

template <class realT>
class countryData
{
private:
	typedef set<int> set_t;
	vector< g4m::ipol<int, realT> > values;
	vector< g4m::ipol<int, int> > count;
	vector< unsigned char > regions;
	set_t countriesToPrint;
	bool printAllCountries;
	void setRegions();
	realT computeAvg(realT *values, int *count, int timeIdx, int timePeriodWidth, int numYears);
public:
	countryData();
	countryData(const countryData & g);
	countryData & operator = (const countryData & g);
	~countryData();
	void reset(void);
	void set(int, int, realT);
	void inc(int, int, realT);
	realT get(int countryIdx, int year);
	realT getAvg(int countryIdx, int year);
	countryData getTimeAvg(int timePeriodWidth);
	countryData getSmoothAvg(int timePeriodWidth, int timeStep = 1);
	realT getRegionSum(unsigned char regIdx, int year);
	void insertCountryToPrint(int countryIdx);
	set_t getListOfCountries();
	void setListOfCountries(set_t S);
	void printToFile(string fileName, int firstYear, int lastYear, int step, string statType = "VAL");  // print array to file
	int readFromFile(string fileName);
};

#endif
