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
#include "readSettings_glob.h"
#include "increment.h"
#include "ageStruct.h"					// max stocking degree is limited to about 1.2
#include "dataStruct.h"					// data structures

#include "dat.h"

#include "dima.h"

using namespace std;

//******************************************************************************
// types
//******************************************************************************
//typedef vector< g4m::ipol<double,double> > cellCol;
//typedef vector<cellCol> dataArray;
typedef vector<g4m::dataStruct> dataDetStruct;
g4m::coeffStruct coeff;
map<string, g4m::ipol<double,double> > lprice; //datamap for land price corrections for current price  scenario (GLOBIOM)
map<string, g4m::ipol<double,double> > wprice; //datamap for wood price corrections for current price  scenario (GLOBIOM)
map<string, g4m::ipol<double,double> > wprod;  //datamap for wood production corrections for POLES regions	(GLOBIOM)
map<string, g4m::ipol<double,double> > cprice; // datamap for carbon price
typedef vector<g4m::ageStruct *> ageStructVector;
typedef vector<dat> datGlobal;
//******************************************************************************
// containers of data
//******************************************************************************
bool GUIcontainers = true;
simUnitsMap sMap = simUnitsMap("simu.bin");
simUnitsData ASU;
set<int> regions;
set<int> years;             // select years for results output
set<int> toAdjust;          // coutry where FM to be adjusted
set<int> doneList;          // countries already adjusted
set<int> countriesList;      // coutry to be considered
set<int>countryregList;      // country and region mixture to be considered

//******************************************************************************
// constants and variables
//******************************************************************************
#ifdef unix
string homeDir = "./data/";
#else
string homeDir = "data\\";
#endif
int ResLatitude, ResLongitude;    // resolutions of model
int eyear, byear;
const double GridStepLat = 0.5;   // step by latitude
const double GridStepLon = 0.5;   // step by longitude
const int nYears = 50;
const int NumberOfCountryregmix = 50; // number of POLES regions mixed with EU27 countries
const int NumberOfCountries = 244; // new country codes
const double unitConv = 1e-6; // unit conversion: 1e-6 - Mt

double MAI_CountryUprotect[NumberOfCountries];
double MAI_CountryAll[NumberOfCountries];
double MAI_countryregmix_up_avg[NumberOfCountryregmix];
//double woodHarvestStat[NumberOfCountries];
double Hurdle_opt[NumberOfCountries];
double afforRate_opt[NumberOfCountries];
double deforRate_opt[NumberOfCountries];
double EmissionsCurCountry[NumberOfCountries+1];
double EmissionsCurAfforCountry[NumberOfCountries+1];
//short int countryNwp[NumberOfCountries];
short int yearNwp[11];
double countryLosses[NumberOfCountries];
double FM_sink_stat[NumberOfCountries];
double FMs[NumberOfCountries];
short int coutryRegion[NumberOfCountries+1];
int numRecords = 0; // number of records in the input data file
//int country_asId[210][2]; //country - asId start and end  
//int xy_asID[3000][2]; // xi and yi correspondence to asID
int numAgeStruct = 0;
//int usedNumAgeStruct = 0;

short int countryCodeOrder[NumberOfCountries];
char counrtyOrderISO[NumberOfCountries+1][4]; 
string countryOrderName[NumberOfCountries+1];
string countryRegName[NumberOfCountryregmix];

g4m::ipol<double,double> sws;  //Schnittholzanteil an Vfm // share of harvestable sawnwood per m3 (diameter, share)
g4m::ipol<double,double> hlv;   //1-Ernteverluste Vornutzung // loosses after first prefinal cut (diameter, share of harvesting loses) ?
g4m::ipol<double,double> hle;   //1-Ernteverluste Endnutzung // losses at final cut (diameter, share of harvesting loses)?
g4m::ipol<double,double> dbv;  //Dekungsbeitrag vornutzung   // income per m3 for thinning (diameter,income)
g4m::ipol<double,double> dbe;  //Dekungsbeitrag endnutzung   //  income per m3 for final harvest (diameter,income)

double LinPrice2050[51];
float CubPrice2050[51];
float MixPrice2050[51];
float LinPrice2030[51];
float CubPrice2030[51] ;
float MixPrice2030[51];
float LinPrice2020[51] ;
float CubPrice2020[51] ;
float MixPrice2020[51];
float LinPrice2015[51] ;
float CubPrice2015[51] ;
float MixPrice2015[51];
float LinPrice2010[51] ;
float CubPrice2010[51] ;
float MixPrice2010[51];

// NPV curves
double profit = 0.;

float minRotNPV[NumberOfCountries];   // country average NPV if max MAI rotation is applied to all forests
float minMedNPV[NumberOfCountries];   // NPV at rotation between min and medium rotation
float medRotNPV[NumberOfCountries];   // NPV at rotation between min and medium rotation
float medMaxNPV[NumberOfCountries];     // NPV at rotation between medium and max biomass rotation
float maxRotNPV[NumberOfCountries];  // country average NPV if max biomass rotation is applied to all forests
float minRot[NumberOfCountries];    // country average min rotation (max harvest)
float maxRot[NumberOfCountries];    // country average max biomass rotation
bool forNPVcurves = false;
bool forNPVcuvvesDyn = false;
bool fmpol = true; // For testing FM response to C price incentive; requires bin files with BAU biomass and NPV
bool bau = false; // Write bin files with BAU biomass and NPV

//if (fmpol){
 vector2d NPVbau = vector2d(46000);
 vector2d biomass_bau = vector2d(46000);
 vector2d Profit_bau = vector2d(46000);
//} 
 int maxDiffCountry = 0;
 double harvDiff[NumberOfCountries];
 griddata2<double> NPVcGrid = griddata2<double>(ResLongitude,ResLatitude,0);  // grid to store current NPV of forest if management is adjusted

int refYear = 2009; // policies start the next year
//---------------------

// Adjusting (increasing) FM sink by disturbance management in "unmanaged" forests 
bool adjustFMsink = true;
//---------------------
double exchangeRate = 1.47; // Euro -> USD exchange rate average for 2008 (all prices in the model are in USD)
string cscenario="";
//int PriceCiS[12] = {0,10,20,30,50,70,100,200,300,500,1000,0};
//int PriceCiS[26] = {0,5,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,220,250,300,1000};
int PriceCiS[11] = {0,5,10,15,20,25,30,35,40,45,50};
double deflator = 0.8807;
double resUse = 0.; // share of harvest residuals that can be used for bioenergy
double tolerance = 0.03;//0.05;//0.1; // tolerance of forest management adjustment to match domestic wood demand
bool cellInteract = true;
//*****************************************************************************
// country outputs
//*****************************************************************************
  bool countryOutput = true;                                                                               

  countryData CountriesNforCover = countryData();
  countryData CountriesNforTotC = countryData();
  countryData CountriesAfforHaYear = countryData();  

  countryData CountriesAfforCYear = countryData();  
  countryData CountriesAfforCYear_ab = countryData();  
  countryData CountriesAfforCYear_bl = countryData();  
  countryData CountriesAfforCYear_biom = countryData();  
  countryData CountriesAfforCYear_dom = countryData();    
  countryData CountriesAfforCYear_soil = countryData();    
//---------  
  countryData CountriesOforCover = countryData();
  countryData CountriesDeforHaYear = countryData();  
    
  countryData CountriesOfor_abC = countryData();
  countryData CountriesOforC_biom = countryData(); 
  countryData CountriesDeforCYear = countryData();  
  countryData CountriesDeforCYear_bl = countryData();   
  countryData CountriesDeforCYear_ab = countryData(); 
  countryData CountriesDeforCYear_biom = countryData();
  countryData CountriesDeforCYear_dom = countryData();   
  countryData CountriesDeforCYear_soil = countryData();     
//---------  
  countryData CountriesWoodHarvestM3Year = countryData();    
  countryData CountriesWoodHarvestPlusM3Year = countryData(); 
  countryData CountriesWoodHarvestFmM3Year = countryData();
  countryData CountriesWoodHarvestDfM3Year = countryData();
  countryData CountriesWoodLoosCYear = countryData();   
  countryData CountriesHarvLossesYear = countryData();
//---------
  countryData CountriesManagedForHa = countryData();     
  countryData CountriesManagedCount = countryData();  
  
  countryData CountriesMAI = countryData();    
  countryData CountriesCAI = countryData();    
  countryData CountriesCAI_new = countryData();      
  countryData CountriesFM = countryData();   
  countryData CountriesFMbm = countryData();     
//---------------------------------------------------------------------------  
  countryData CountryRotation =  countryData(); 
//----------
  countryData CountriesWprod =  countryData();  // test wood production input data
//-----------
  countryData CountriesProfit =  countryData();  // profit due to selling  harvested wood
//---------------------------------------------------------------------------  
  countryData CountryregWoodHarvestM3Year =  countryData(); 
  countryData CountryregWoodHarvestFmM3Year = countryData();
  countryData CountryregWoodHarvestDfM3Year = countryData();  
  countryData CountryregWprod =  countryData();
  //---------------------------------------------------------------------------  
  countryData CountryregRotation =  countryData(); 

//**************************
//Output file  
//**************************
  ofstream fff;  
  
//******************************************************************************
// ASCII grid output
bool gridOutput = true; // to output maps (ascii grids)
    griddata2<double> SD_grid_1990 = griddata2<double>(360/GridStepLon,180/GridStepLat,-9999);        
    griddata2<double> SD_grid_00 = griddata2<double>(360/GridStepLon,180/GridStepLat,-9999);        
    griddata2<double> SD_grid_10 = griddata2<double>(360/GridStepLon,180/GridStepLat,-9999);        
	    
    griddata2<double> harvestm3_grid_20 = griddata2<double>(360/GridStepLon,180/GridStepLat,-9999);     
    griddata2<double> harvestm3_grid_30 = griddata2<double>(360/GridStepLon,180/GridStepLat,-9999); 
    griddata2<double> harvestm3_grid_50 = griddata2<double>(360/GridStepLon,180/GridStepLat,-9999);     
        
    griddata2<double> bmabtC_of_grid_20 = griddata2<double>(360/GridStepLon,180/GridStepLat,-9999);         
    griddata2<double> bmabtC_of_grid_30 = griddata2<double>(360/GridStepLon,180/GridStepLat,-9999);   
    griddata2<double> bmabtC_of_grid_50 = griddata2<double>(360/GridStepLon,180/GridStepLat,-9999);             

    griddata2<double> bmabtC_nf_grid_20 = griddata2<double>(360/GridStepLon,180/GridStepLat,-9999);         
    griddata2<double> bmabtC_nf_grid_30 = griddata2<double>(360/GridStepLon,180/GridStepLat,-9999);   
    griddata2<double> bmabtC_nf_grid_50 = griddata2<double>(360/GridStepLon,180/GridStepLat,-9999);

    griddata2<double> fmgGco2_grid_20 = griddata2<double>(360/GridStepLon,180/GridStepLat,-9999); 
    griddata2<double> fmgGco2_grid_30 = griddata2<double>(360/GridStepLon,180/GridStepLat,-9999);     
    griddata2<double> fmgGco2_grid_50 = griddata2<double>(360/GridStepLon,180/GridStepLat,-9999);         

    griddata2<double> mai_m3ha_grid_20 = griddata2<double>(360/GridStepLon,180/GridStepLat,-9999);        
    griddata2<double> mai_m3ha_grid_30 = griddata2<double>(360/GridStepLon,180/GridStepLat,-9999);        
    griddata2<double> mai_m3ha_grid_50 = griddata2<double>(360/GridStepLon,180/GridStepLat,-9999);        

    griddata2<double> SD_grid_20 = griddata2<double>(360/GridStepLon,180/GridStepLat,-9999);        
    griddata2<double> SD_grid_30 = griddata2<double>(360/GridStepLon,180/GridStepLat,-9999);        
    griddata2<double> SD_grid_50 = griddata2<double>(360/GridStepLon,180/GridStepLat,-9999);    

    griddata2<double> RL_grid_20 = griddata2<double>(360/GridStepLon,180/GridStepLat,-9999);        
    griddata2<double> RL_grid_30 = griddata2<double>(360/GridStepLon,180/GridStepLat,-9999);        
    griddata2<double> RL_grid_50 = griddata2<double>(360/GridStepLon,180/GridStepLat,-9999); 


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
void int2str(int i, char tmp[]);   // defined in misc.h

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