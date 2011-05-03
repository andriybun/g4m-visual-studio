#ifndef FOREST_H_
#define FOREST_H_

#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <ctime>
#include <algorithm>

// Classes for preparing output for GDataView
#include "jDataViewClasses.h"
// Some common and abstract classes
#include "commonClasses.h"

// g4m interface
#include "intf.h"
#include "model.h"

// Rest of includes:
#include "readSettings.h"
#include "increment.h"
#include "ageStruct.h"					// max stocking degree is limited to about 1.2
#include "dataStruct.h"					// data structures

#include "dat.h"

using namespace std;

#define GridStepLat		(double)0.5		// step by latitude
#define GridStepLon		(double)0.5		// step by longitude
#define nYears			61
#define NumberOfCountryregmix 50		// number of POLES regions mixed with EU27 countries
#define NumberOfCountries 244			// new country codes
#define unitConv		(double)1e-6	// unit conversion: 1e-6 - Mt


typedef vector<g4m::dataStruct> dataDetStruct;
typedef vector<g4m::ageStruct *> ageStructVector;
typedef vector<dat> datGlobal;
typedef struct
{
	string coeffPath, inputPath, outputPath;
	set<string> parametersTable,parametersTableReg, parametersMap;
	bool produceTabs, produceMaps, tabs[3], maps[3];
} settingsT;

//******************************************************************************
// functions
//******************************************************************************
int readInputDet(dataDetStruct &);
void readCoeff(g4m::coeffStruct &coeff);

vector<double> procPlots(g4m::dataStruct &,double,int,double, double,double, double, double);
vector<double> calcPlots(double [],int);
vector<double> plotsData(g4m::dataStruct &,int);

void initManagedForest(dataDetStruct &data_all, g4m::incrementTab &fi, datGlobal &dat_all,
					   ageStructVector &cohort_all, ageStructVector &newCohort_all,
					   griddata2<double> &maiForest, griddata2<double> &thinningForest,
					   griddataLite<char> &rotationType, griddataLite<char> &managedForest,
					   griddata2<double> &rotationForest, griddata2<double> &harvestGrid, griddata2<double> &OforestShGrid);

void adjustManagedForest(dataDetStruct &data_all, g4m::incrementTab &fi, ageStructVector &cohort_all, 
              ageStructVector &newCohort_all, datGlobal &dat_all, griddata2<double> &maiForest, 
              griddata2<double> &thinningForest, griddata2<double> &rotationForest, griddataLite<char> &managedForest,
              griddata2<double> &rotationForestNew, griddata2<double> &thinningForestNew, griddataLite<char> &manageChForest,
              griddataLite<char> &rotationType, griddata2<double> &harvestGrid, int year, griddataLite<char> &unmanaged, double);

void fm_cpol(dataDetStruct &, g4m::incrementTab &, ageStructVector &, 
              ageStructVector &, datGlobal &, griddata2<double> &, 
              griddata2<double> &, griddata2<double> &, griddataLite<char> &,
              griddata2<double> &, griddata2<double> &, griddataLite<char> &,
              griddataLite<char> &, griddata2<double> &, int , griddataLite<char> &, double,double &,double,
              set<int> &, griddata2<double> &,griddataLite<int> &,griddataLite<int> &);
           
void initLoop(int, dataDetStruct &, g4m::incrementTab &, ageStructVector &, 
              ageStructVector &, datGlobal &, griddata2<double> &, griddata2<double> &, griddata2<double> &);

void calc(g4m::dataStruct &it, g4m::incrementTab &fi, g4m::ageStruct &cohort, g4m::ageStruct &newCohort,
		  dat &singleCell, griddataLite<char> &managedForest, griddata2<double> &maiForest, griddata2<double> &rotationForest,
		  griddata2<double> &rotationForestNew, griddata2<double> &thinningForest, griddata2<double> &thinningForestNew, 
		  griddata2<double> &harvestGrid, int year, double priceC, int asID, griddata2<double> &OforestShGrid);
          
void MAI_countryregmix(void);
void woodHarvestStatCountry(void);
void hurdle_aff_deff(void);
void cPrices(void);
//void int2str(int i, char tmp[]);   // defined in misc.h

void woodProductionIndexes(void); // country code and year for wood production file

double forNPV(g4m::dataStruct &, double , int , int , double ,      // estimation of forestry NPV
              double , double , double, g4m::ipol<double,double> , double, double );
double forNPVfd(g4m::dataStruct &, double , int , int , double , 
                g4m::ipol<double,double> , double , double , double );
double forNPVfdc(g4m::dataStruct &, double , int , int , double , 
                g4m::ipol<double,double> , double , double , double,double);       
                        
double npv_calc(g4m::dataStruct &iter, g4m::ageStruct &cohortTmp, double maiV, int year, int rotation, string regprice, string regprice0,double OforestShare, short int used);

double npv_calc10(g4m::dataStruct &iter, g4m::ageStruct &cohortTmp, double maiV, int year, int rotation, string regprice, string regprice0,double OforestShare, short int used);

double npv_calc50(g4m::dataStruct &iter, g4m::ageStruct &cohortTmp, double maiV, int year, int rotation, string regprice, string regprice0,double OforestShare, short int used);

void forNPV_init(void); // initialisation of arrays for estimation of country average forestry NPV

void carbonPrice(void); // definition of carbon price time functions

void listsToConsider(void);
void countryCodes();

#endif