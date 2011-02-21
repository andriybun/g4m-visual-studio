//  Name: CountryData class with statistics by regions
//  Author: Andriy Bun
//  Date: 29.10.09
//  MG: added printToFile
//  Description: 
//      29.10.2009: added selecting countries to print; if no countries specified - prints all countries;


#ifndef COUNTRYDATA_H_
#define COUNTRYDATA_H_

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <set>

#include "ipol.h"

#define xmin(a,b) (((a)<(b))?(a):(b))
#define xmax(a,b) (((a)<(b))?(b):(a))

const int countriesNum = 242;

using namespace std;

class countryData
{
private:
    typedef set<int> set_t;
    vector< g4m::ipol<int, double> > values;
    vector< g4m::ipol<int, int> > count;
    vector< unsigned char > regions;
    bool printAllCountries;
    void setRegions();
	double computeAvg(double *values, int *count, int timeIdx, int timePeriodWidth, int numYears);
public:
    countryData();
    ~countryData();
    void reset(void);
    void set(int, int, double);
    void inc(int, int, double);
    double get(int countryIdx, int year);
    double getAvg(int countryIdx, int year);
	countryData getTimeAvg(int countryIdx, int numYearsBack);
	countryData getTimeAvgM(int timePeriodWidth);
    double getRegionSum(unsigned char regIdx, int year);
    void insertCountryToPrint(int countryIdx);
    set_t getListOfCountries();
    set_t countriesToPrint;
    void setListOfCountries(set_t S);
    void PrintToFile(string, int, int, int, string);  // print array to file
};

#endif
