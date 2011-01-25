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

const int countriesNum = 242;

using namespace std;

class countryData
{
private:
    typedef set<int> set_t;
    vector< g4m::ipol<double,double> > countries;
    vector< g4m::ipol<double,int> > count;
    vector< unsigned char > regions;
//    set_t countriesToPrint;
    bool printAllCountries;
    void setRegions();
//    void PrintToFile(string, int, int, int, string);  // print array to file    
public:
    countryData();
    ~countryData();
    void reset(void);
    void set(int, int, double);
    void inc(int, int, double);
    double get(int, int);
    double getAvg(int, int);
    double getRegionSum(unsigned char regID, int year);
    void insertCountryToPrint(int countryID);
    set_t getListOfCountries();
    set_t countriesToPrint;    
    void setListOfCountries(set_t S);
    void PrintToFile(string, int, int, int, string);  // print array to file    
};

#endif
