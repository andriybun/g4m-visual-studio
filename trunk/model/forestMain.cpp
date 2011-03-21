//   Name:      g4m + interaction with GUI & GDataView
//   Author:    Andriy Bun, based on works of ...
//   Date:      ** December, 2009

// Modified: Mykola Gusti, 4 Jan 2010

#include "forest.h"							// definitions

#ifdef unix
	string homeDir = "./data/";
#else
	string homeDir = "data\\";
#endif

g4m::coeffStruct coeff;
settingsT settings;

map<string, g4m::ipol<double,double> > lprice; //datamap for land price corrections for current price  scenario (GLOBIOM)
map<string, g4m::ipol<double,double> > wprice; //datamap for wood price corrections for current price  scenario (GLOBIOM)
map<string, g4m::ipol<double,double> > wprod;  //datamap for wood production corrections for POLES regions	(GLOBIOM)
map<string, g4m::ipol<double,double> > cprice; // datamap for carbon price
//******************************************************************************
// containers of data
//******************************************************************************
bool GUIcontainers = true;
simUnitsMap sMap = simUnitsMap("data/simu.bin");
simUnitsData ASU;
set<int> regions;
set<int> years;             // select years for results output
set<int> toAdjust;          // coutry where FM to be adjusted
set<int> doneList;          // countries already adjusted
set<int> countriesList;      // coutry to be considered
set<int> countryregList;      // country and region mixture to be considered

//******************************************************************************
// constants and variables
//******************************************************************************
int ResLatitude, ResLongitude;    // resolutions of model
int eyear, byear;

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
//***********************************  MAIN  ***********************************
//******************************************************************************
int main(int argc, char * argv[])
 {
  time_t start=time(NULL);
  // Read settings from GUI:
  readSettings();
// resolution of model  
  ResLatitude = int(floor(180/GridStepLat));
  ResLongitude = int(floor(360/GridStepLon));

// Setting years for output, countries and regions to be considered for calculations and output
  listsToConsider();

//*******************************
//** Reading coefficients file **
//*******************************
  readCoeff(coeff);
// starting and ending years of calculations
  byear = coeff.bYear;
  eyear = coeff.eYear;
//*****************************************************
//** Reading input files (with resolution 0.5 x 0.5) **
//*****************************************************
//*******************************************
//** Reading detailed input data from file **
//*******************************************
  woodProductionIndexes();
  dataDetStruct plots;                         // structure with data
//  int numRecords = readInputDet(plots);        // plots[<elNum>].<variable>[year]
  numRecords = readInputDet(plots);        // plots[<elNum>].<variable>[year]  
//  cout << "> Time is " << difftime(time(NULL),start) << " sec." << endl;
//******************************************************************************
//*****************************************
//** Add other parameters to output here **
//*****************************************
//  countryData countriesForestArea = countryData();                              // table output
//************************************************
// Output file name suffixe
  string suffix="glob_base_GUI";  
  string scenario[4]={"bau","ref","etgca","dgca"};
//PriceCiS[23] = {0,5,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,1000};
	
	//TODO: uncomment and/or fix
	//carbonPrice(); // initialisation of the C price function

int PriceC = 0; //*12/44;  // Set carbon price scenario
double priceC = 0.;
cscenario = scenario[PriceC];
//   if (PriceC > 0){ suffix += "_Pco2_" + IntToStr(PriceCiS[PriceC]);
   if (PriceC > 0){ suffix += "_Pco2_" + cscenario;  }
   if (PriceC==0) {suffix += "_Pco2_0";}   
//      priceC = PriceCiS[PriceC] * 12/44 * exchangeRate;};
   string fileOut = settings.outputPath +"\\" + "details" + suffix + ".txt";

  fff.open(fileOut.c_str(),ios::out);
  if (fff.is_open()) {   
fff<<"asID"<<"\tyear"<<"\tCountry"<<"\tOforShare"<< "\tAforestShare"<<"\tcab"<<"\tabFM"<<"\tMAI_M3Ha"<<"\trotMAI";
fff<<"\t harvestThM3"<<"\t harvestFctM3"<<"\tharvTotCur"<<"\tharvMAI"<<"\trotBiomass"<<"\tforNPV_CH"<<"\tfVal"<<"\t aval";
fff<<"\t rotationTimeCurr"<<"\t rotationTimeCurrNew";
fff<<"\tTimberPrice"<<"\tthinningForest"<<"\tmanagedForest";
fff<<"\tEmissionsSOCAfforCur"<<"\t abBiomassAffor"<<"\t EmissionsLitterAfforCurHa";
fff<<"\t LitterAfforHa"<<endl;


//    fff << "asID"<<"\tyear"<<"\tCountry"<< "\tpriceCO2"<<"\tdeforHaYear"<<"\tafforHaYear"<<"\tOforestHa"<< "\tAforestHa";
//    fff << "\tdeforCYear"<<"\tdeforCYear_ab"<<"\tdeforCYear_bl"<<"\tdeforCYear_biom"<<"\tdeforCYear_dom"<<"\tdeforCYear_soil";
//    fff << "\tafforCYear"<<"\tafforCYear_ab"<<"\tafforCYear_bl"<<"\tafforCYear_biom"<<"\tafforCYear_dom"<<"\tafforCYear_soil";
//    fff << endl;

}

//** Initializing forest cover array by gridcells **
//**************************************************
  griddata2<double> harvestGrid = griddata2<double>(ResLongitude,ResLatitude,0);
  griddata2<double> maiForest = griddata2<double>(ResLongitude,ResLatitude,0);
  griddata2<double> rotationForest = griddata2<double>(ResLongitude,ResLatitude,0);
  griddata2<double> rotationForestNew = griddata2<double>(ResLongitude,ResLatitude,0);
  griddata2<double> thinningForest = griddata2<double>(ResLongitude,ResLatitude,0);
  griddata2<double> thinningForestNew = griddata2<double>(ResLongitude,ResLatitude,0);
  griddata2<double> OforestShGrid = griddata2<double>(ResLongitude,ResLatitude,0);  

  griddataLite<char> decisionGrid = griddataLite<char>(ResLongitude,ResLatitude,0);
  griddataLite<char> managedForest = griddataLite<char>(ResLongitude,ResLatitude,0);
  griddataLite<char> manageChForest = griddataLite<char>(ResLongitude,ResLatitude,0);
  griddataLite<char> rotationType = griddataLite<char>(ResLongitude,ResLatitude,0);
  griddataLite<char> unmanaged = griddataLite<char>(ResLongitude,ResLatitude,0);
  
  // Setup forest increment table
  g4m::incrementTab fi(-4.25, -1.39, 0.329, 169., 81., 4.48, -1.39, 0.780,
                       -0.176, 1./4.5, 0.5, -1.08, -7.7956, -0.2362, -0.6316,
                        24.62079, 0.46573, -0.02426, 2.58884, -0.30364, -0.5782,
		                0.8513, 0.0289, 1.4720, 0.5, 2., 1.6, 10., 0.25, 600., 1.);

// Setup forest management parameters similar for all countries (cells)
        sws.insert(10, 0.0);
        sws.insert(30, 0.6);
        dbv.insert(0, 2);
        dbe.insert(0, 3);

// Initializing some arrays
//  MAI_country();
  MAI_countryregmix();
  cPrices();
  hurdle_aff_deff();
//  woodHarvestStatCountry();
  woodProductionIndexes();
  forNPV_init();
  countryCodes();
//  listsToConsider();

  for (int i=0; i < NumberOfCountries; i++) {  
    EmissionsCurCountry[i]=0.; 
    EmissionsCurAfforCountry[i]=0.;
  }
  ageStructVector cohort_all;
  ageStructVector newCohort_all;
  cohort_all.reserve(numRecords);
  newCohort_all.reserve(numRecords);
  datGlobal dat_all;

if (GUIcontainers){
  ASU = simUnitsData();                      // Initializing data for simulation units
  ASU.rename("G4M parameters");
  // Adding dimensions' parameters:
  if (settings.maps[0]) ASU.addDim("Scenario", IntToStr(0));
  if (settings.maps[1]) ASU.addDim("Year", years);
  if (settings.maps[2]) ASU.addDim("Parameter", settings.parametersMap);
}

 if (fmpol && PriceC>0) {biomass_bau.readFromFile("biomass_bau"); // reading BAU files
                         NPVbau.readFromFile("NPVbau");}   

cout<<biomass_bau.size()<<"\t"<<biomass_bau[1].size()<<endl;
cout<<NPVbau.size()<<"\t"<<NPVbau[1].size()<<endl;

// ****************************************************************************

//******************************************************************************
//***************************** start calculations *****************************
//******************************************************************************

cout<< "Start initialising cohorts"<< endl;
  initManagedForest(plots, fi, dat_all, cohort_all, newCohort_all, maiForest, thinningForest, rotationType, 
                    managedForest, rotationForest, harvestGrid, OforestShGrid);
//*************************
//**** loop by prices *****
//*************************
//  for (int i=0;i<=0;i++) {         // MG: PriceC loop
string strtmp;
if (GUIcontainers){
    // filling vector of coordinates
    strtmp = IntToStr(PriceC);
    if (settings.maps[0]) ASU.pointPush(strtmp);
}
//cout << "Price C value " << i << endl;    
//    initLoop(i, plots, fi, cohort_all, newCohort_all, dat_all, maiForest, thinningForest, rotationForest);
//    initLoop(i, plots, fi, dat_all, maiForest, thinningForest, rotationForest);
//************************
//**** loop by years *****
//************************
    int year = byear;
    do {
      cout << "Processing year " << year << endl;
//      decision.setYear(year);
      int Age = year-byear;
//      if (year > refYear && PriceC>0) {priceC = cprice[cscenario].v(year);}
cout<<"year=\t"<<year<<"\tscenario="<<cscenario<<"\tprice=\t"<<priceC<<endl;      
      if (year >= byear)
//      if (year>byear && (year==byear+1 || year%5 == 0)) // correct forest management every 5 years
       {
cout << "Adjusting FM .."<< endl;
       adjustManagedForest(plots, fi, cohort_all, 
              newCohort_all, dat_all, maiForest, 
              thinningForest, rotationForest, managedForest,
              rotationForestNew, thinningForestNew, manageChForest,
              rotationType, harvestGrid, year, unmanaged,priceC);
       }


//************************************
//** processing data from all plots **
//************************************
      vector<double> tmpBm;
      vector<double> tmpPr;
  
                   
cout << "starting cell calculations .."<< endl;
      dataDetStruct::iterator it = plots.begin();
      while (it != plots.end()) {
      if (regions.find(it->POLESREG[2000]) != regions.end()) { // Test only some regions
       if (countriesList.find(it->COUNTRY[2000]) != countriesList.end()) { // Test only some countries    
//        if (it->PROTECT[2000]==0) {
       if ((it->PROTECT[2000]==0)&&((it->POTVEG[2000]<10 && it->FOREST[2000]>0 && it->MAIE[2000]>0)||(it->POTVEG[2000]<10 && (it->NPP[2000]>0)||it->MAIN[2000]>0))){
            int asID = it->asID;
            calc(*it, fi, *cohort_all[asID], *newCohort_all[asID], dat_all[asID], managedForest,
                 maiForest, rotationForest, rotationForestNew, thinningForest, thinningForestNew, 
                 harvestGrid,year, priceC, asID, OforestShGrid);
                 
//if (year > byear) cout<< "finished Calc"<<endl;                 
            if (fmpol&&bau&&PriceC==0&&year>refYear){ tmpBm.push_back(dat_all[asID].ObiomassPrev);      
                                                 tmpPr.push_back(profit);
//cout<< "profit = "<<"\t"<<profit<<endl;                                                 
                                                                                          }                
          }                        // End if not protected ...
         }                         // End if country
        }                          // End if region
//            if (fmpol&&PriceC==0&&year>refYear){ tmp.push_back(dat_all[asID].ObiomassPrev);      }                
        it++;
      }                            // End loop by plots
            if (fmpol&&bau&&PriceC==0&&year>refYear){ biomass_bau.push(tmpBm);     
                                                 Profit_bau.push(tmpPr); }  
      
//******************************************
//** writing to containers for GUI output **
//******************************************

if (GUIcontainers)
{
cout<<"writing to GUI containers"<<endl;
      if (years.find(year) != years.end()) {
        strtmp = IntToStr(year);
        if (settings.maps[1]) ASU.pointPush(strtmp);
        dataDetStruct::iterator it = plots.begin();
        while (it != plots.end()) {
//          if (it->PROTECT[2000]==0) {
			if ((it->PROTECT[2000]==0)&&((it->POTVEG[2000]<10 && it->FOREST[2000]>0 && it->MAIE[2000]>0)||(it->POTVEG[2000]<10 && (it->NPP[2000]>0)||it->MAIN[2000]>0)))
			{                          
				if (regions.find(it->POLESREG[2000]) != regions.end()) { // Test only some regions
					int asID = it->asID;
					if (settings.parametersMap.find("stocking degree") != settings.parametersMap.end())
					{
						ASU.insert(dat_all[asID].x, dat_all[asID].y, dat_all[asID].SD, "stocking degree", DISTRIBUTE_PROPORTIONALLY);
					}
					if (settings.parametersMap.find("em_fm_ab mtco2hayear") != settings.parametersMap.end())
					{
						ASU.insert(dat_all[asID].x, dat_all[asID].y, dat_all[asID].FMsink*(-1), "em_fm_ab mtco2hayear", DISTRIBUTE_PROPORTIONALLY);
					}              
					if (settings.parametersMap.find("em_fm_bm mtco2hayear") != settings.parametersMap.end())
					{
						ASU.insert(dat_all[asID].x, dat_all[asID].y, dat_all[asID].FMsink_Bm*(-1), "em_fm_bm mtco2hayear", DISTRIBUTE_PROPORTIONALLY);
					}  
					if (settings.parametersMap.find("area_forest_old ha") != settings.parametersMap.end())
					{
						ASU.insert(dat_all[asID].x, dat_all[asID].y, dat_all[asID].OforestShare*dat_all[asID].LandAreaHa, "area_forest_old ha", DISTRIBUTE_PROPORTIONALLY);
					}               
					if (settings.parametersMap.find("area_forest_new ha") != settings.parametersMap.end())
					{
						ASU.insert(dat_all[asID].x, dat_all[asID].y, dat_all[asID].AforestShare*dat_all[asID].LandAreaHa, "area_forest_new ha", DISTRIBUTE_PROPORTIONALLY);
					}                      
					if (settings.parametersMap.find("cai m3ha") != settings.parametersMap.end())
					{
						ASU.insert(dat_all[asID].x, dat_all[asID].y, dat_all[asID].CAI, "cai m3ha", DISTRIBUTE_PROPORTIONALLY);
					}            
					if (settings.parametersMap.find("area_df hayear") != settings.parametersMap.end())
					{
						ASU.insert(dat_all[asID].x, dat_all[asID].y, dat_all[asID].deforestHaYear, "area_df hayear", DISTRIBUTE_PROPORTIONALLY);
					}  
					if (settings.parametersMap.find("area_af hayear") != settings.parametersMap.end())
					{
						ASU.insert(dat_all[asID].x, dat_all[asID].y, dat_all[asID].afforestHaYear, "area_af hayear", DISTRIBUTE_PROPORTIONALLY);
					}                          
					if (settings.parametersMap.find("harvest_total m3hayear") != settings.parametersMap.end())
					{
						ASU.insert(dat_all[asID].x, dat_all[asID].y, dat_all[asID].harvestTot, "harvest_total m3hayear", DISTRIBUTE_PROPORTIONALLY);
					}      
					if (settings.parametersMap.find("harvest_fc m3hayear") != settings.parametersMap.end())
					{
						ASU.insert(dat_all[asID].x, dat_all[asID].y, dat_all[asID].harvestFcM3Ha, "harvest_fc m3hayear", DISTRIBUTE_PROPORTIONALLY);
					}      
					if (settings.parametersMap.find("harvest_th m3hayear") != settings.parametersMap.end())
					{
						ASU.insert(dat_all[asID].x, dat_all[asID].y, dat_all[asID].harvestThM3Ha, "harvest_th m3hayear", DISTRIBUTE_PROPORTIONALLY);
					}                    
					if (settings.parametersMap.find("biom_fm tcha") != settings.parametersMap.end())
					{
						ASU.insert(dat_all[asID].x, dat_all[asID].y, dat_all[asID].oforestBm, "biom_fm tcha", DISTRIBUTE_PROPORTIONALLY);
					}                    
					if (settings.parametersMap.find("biom_af tcha") != settings.parametersMap.end())
					{
						ASU.insert(dat_all[asID].x, dat_all[asID].y, dat_all[asID].aforestBm, "biom_af tcha", DISTRIBUTE_PROPORTIONALLY);
					}  
					if (settings.parametersMap.find("em_df_bm mtco2hayear") != settings.parametersMap.end())
					{
						ASU.insert(dat_all[asID].x, dat_all[asID].y, dat_all[asID].emissionsD_Bm, "em_df_bm mtco2hayear", DISTRIBUTE_PROPORTIONALLY);
					}  
					if (settings.parametersMap.find("em_df_sl mtco2hayear") != settings.parametersMap.end())
					{
						ASU.insert(dat_all[asID].x, dat_all[asID].y, dat_all[asID].emissionsD_S, "em_df_sl mtco2hayear", DISTRIBUTE_PROPORTIONALLY);
					}  
					if (settings.parametersMap.find("em_af_bm mtco2hayear") != settings.parametersMap.end())
					{
						ASU.insert(dat_all[asID].x, dat_all[asID].y, dat_all[asID].emissionsA_Bm, "em_af_bm mtco2hayear", DISTRIBUTE_PROPORTIONALLY);
					}  
					if (settings.parametersMap.find("em_af_sl mtco2hayear") != settings.parametersMap.end())
					{
						ASU.insert(dat_all[asID].x, dat_all[asID].y, dat_all[asID].emissionsA_S, "em_af_sl mtco2hayear", DISTRIBUTE_PROPORTIONALLY);
					}  
					if (settings.parametersMap.find("rotation year") != settings.parametersMap.end())
					{
						ASU.insert(dat_all[asID].x, dat_all[asID].y, dat_all[asID].Rotation, "rotation year", DISTRIBUTE_PROPORTIONALLY);
					}                                           
// Add parameters you want to output here as for "Forest area":
// The parameters you may want to output must be defined in listOfParameters.txt
// file used by the GUI. The parameters for output must be defined in settings.ini 
// created by the GUI for g4m. 
            }                      // End if Country ...
          }                        // End if not protected
          it++;
        }
        if (settings.maps[1]) ASU.pointPop();            // clearing year from vector of coordinates                          // End loop by plots
      }                            // End loop by years
}
      year++;
    } while (year <= eyear);       // End loop by years

if (GUIcontainers){
//
//set<string> countriesISOList;      // coutry ISO to be considered
//set<int>::iterator it;
//set<string>::iterator itn;
//itn=countriesISOList.begin();
//for (it=countriesList.begin(); it!=countriesList.end(); it++){
////cout<<"countryCode=\t"<<*it<<"\tcountryCodeOrder="<<countryCodeOrder[(*it-1)]<<"\tcountryISO=\t"<<counrtyOrderISO[countryCodeOrder[(*it-1)]]<<endl;
//string tmp = counrtyOrderISO[countryCodeOrder[(*it) - 1]]; 				   	
//countriesISOList.insert(itn,tmp);
//itn++;
//}   		

set<string> countriesNameList;      // coutry Names to be considered in the table GUI
{
set<int>::iterator it;
set<string>::iterator itn;
itn=countriesNameList.begin();
for (it=countriesList.begin(); it!=countriesList.end(); it++){
//cout<<"countryCode=\t"<<*it<<"\tcountryCodeOrder="<<countryCodeOrder[(*it-1)]<<"\tcountryName=\t"<<counrtyOrderName[countryCodeOrder[(*it-1)]]<<endl;
string tmp = countryOrderName[countryCodeOrder[(*it) - 1]]; 				   	
countriesNameList.insert(itn,tmp);
itn++;
}   	
}
cout<<"start writing to GUI table"<<endl;		   
    // Clearing vector of coordinates:
    if (settings.maps[0]) ASU.pointPop();
//  }                                // End loop by prices
//  cout << "> Working time is " << difftime(time(NULL),start) << " sec." << endl;
//  system("pause");
//*************************************
//** table data
//*************************************
  countryData CountriesNforCover_tw = CountriesNforCover.getSmoothAvg(5);
  countryData CountriesNforTotC_tw = CountriesNforTotC.getSmoothAvg(5);
  countryData CountriesAfforHaYear_tw = CountriesAfforHaYear.getSmoothAvg(5);  

  countryData CountriesAfforCYear_tw = CountriesAfforCYear.getSmoothAvg(5);  
  countryData CountriesAfforCYear_ab_tw = CountriesAfforCYear_ab.getSmoothAvg(5);  
  countryData CountriesAfforCYear_bl_tw = CountriesAfforCYear_bl.getSmoothAvg(5);  
  countryData CountriesAfforCYear_biom_tw = CountriesAfforCYear_biom.getSmoothAvg(5);  
  countryData CountriesAfforCYear_dom_tw = CountriesAfforCYear_dom.getSmoothAvg(5);    
  countryData CountriesAfforCYear_soil_tw = CountriesAfforCYear_soil.getSmoothAvg(5);    
//---------  
  countryData CountriesOforCover_tw = CountriesOforCover.getSmoothAvg(5);
  countryData CountriesDeforHaYear_tw = CountriesDeforHaYear.getSmoothAvg(5);  
    
  countryData CountriesOfor_abC_tw = CountriesOfor_abC.getSmoothAvg(5);
  countryData CountriesOforC_biom_tw = CountriesOforC_biom.getSmoothAvg(5); 
  countryData CountriesDeforCYear_tw = CountriesDeforCYear.getSmoothAvg(5);  
  countryData CountriesDeforCYear_bl_tw = CountriesDeforCYear_bl.getSmoothAvg(5);   
  countryData CountriesDeforCYear_ab_tw = CountriesDeforCYear_ab.getSmoothAvg(5); 
  countryData CountriesDeforCYear_biom_tw = CountriesDeforCYear_biom.getSmoothAvg(5);
  countryData CountriesDeforCYear_dom_tw = CountriesDeforCYear_dom.getSmoothAvg(5);   
  countryData CountriesDeforCYear_soil_tw = CountriesDeforCYear_soil.getSmoothAvg(5);     
//---------  
  countryData CountriesWoodHarvestM3Year_tw = CountriesWoodHarvestM3Year.getSmoothAvg(5);
  countryData CountriesWoodHarvestPlusM3Year_tw = CountriesWoodHarvestPlusM3Year.getSmoothAvg(5); 
  countryData CountriesWoodHarvestFmM3Year_tw = CountriesWoodHarvestFmM3Year.getSmoothAvg(5);
  countryData CountriesWoodHarvestDfM3Year_tw = CountriesWoodHarvestDfM3Year.getSmoothAvg(5);
  countryData CountriesWoodLoosCYear_tw = CountriesWoodLoosCYear.getSmoothAvg(5);   
  countryData CountriesHarvLossesYear_tw = CountriesHarvLossesYear.getSmoothAvg(5);
//---------
  countryData CountriesManagedForHa_tw = CountriesManagedForHa.getSmoothAvg(5);     
  countryData CountriesManagedCount_tw = CountriesManagedCount.getSmoothAvg(5);  
  
  countryData CountriesMAI_tw = CountriesMAI.getSmoothAvg(5);    
  countryData CountriesCAI_tw = CountriesCAI.getSmoothAvg(5);    
  countryData CountriesCAI_new_tw = CountriesCAI_new.getSmoothAvg(5);      
  countryData CountriesFM_tw = CountriesFM.getSmoothAvg(5);   
  countryData CountriesFMbm_tw = CountriesFMbm.getSmoothAvg(5);     
//---------------------------------------------------------------------------  
  countryData CountryRotation_tw =  CountryRotation.getSmoothAvg(5); 
//----------
  countryData CountriesWprod_tw =  CountriesWprod.getSmoothAvg(5);  // test wood production input data
//-----------
//  countryData CountriesProfit =  CountriesProfit.getSmoothAvg(5);  // profit due to selling  harvested wood
//---------------------------------------------------------------------------  
  countryData CountryregWoodHarvestM3Year_tw =  CountryregWoodHarvestM3Year.getSmoothAvg(5); 
  countryData CountryregWoodHarvestFmM3Year_tw = CountryregWoodHarvestFmM3Year.getSmoothAvg(5);
  countryData CountryregWoodHarvestDfM3Year_tw = CountryregWoodHarvestDfM3Year.getSmoothAvg(5);  
  countryData CountryregWprod_tw =  CountryregWprod.getSmoothAvg(5);
  //---------------------------------------------------------------------------  
  countryData CountryregRotation_tw =  CountryregRotation.getSmoothAvg(5); 
  //---------------------------------------------------------------------------  
  tableData obj;
  vector<string> point;
  if (settings.produceTabs) {
    if (settings.tabs[0]) obj.addDim("Scenario", IntToStr(0));
    if (settings.tabs[1]) obj.addDim("Year", years);
    if (settings.tabs[2]) obj.addDim("Country", countriesNameList);
    /*if (settings.tabs[3])*/ obj.addDim("Parameter", settings.parametersTable);
    for (int prc = 0; prc < 1; prc++) {
      if (settings.tabs[0]) point.push_back(IntToStr(prc));
      for (int year = byear; year <= eyear; year++) {
        if (years.find(year) != years.end()) {
          if (settings.tabs[1]) point.push_back(IntToStr(year));
          for (int countryCode = 1; countryCode < 244; countryCode++) {
            if (countriesList.find(countryCode) != countriesList.end()) {
			  std::string country_code = countryOrderName[countryCodeOrder[countryCode-1]]; 
              if (settings.tabs[2]) point.push_back(country_code);
              if (settings.parametersTable.find("em_fm_ab mtco2year") != settings.parametersTable.end()) {
                point.push_back("em_fm_ab mtco2year");
                obj.insert(CountriesFM_tw.get(countryCode, year)*(-1), point);
                point.pop_back();
              }
              if (settings.parametersTable.find("em_fm_bm mtco2year") != settings.parametersTable.end()) {
                point.push_back("em_fm_bm mtco2year");
                obj.insert(CountriesFMbm_tw.get(countryCode, year)*(-1), point);
                point.pop_back();
              }       
              if (settings.parametersTable.find("area_forest_old ha") != settings.parametersTable.end()) {
                point.push_back("area_forest_old ha");
                obj.insert(CountriesOforCover_tw.get(countryCode, year), point);
                point.pop_back();
              }              
              if (settings.parametersTable.find("area_forest_new ha") != settings.parametersTable.end()) {
                point.push_back("area_forest_new ha");
                obj.insert(CountriesNforCover_tw.get(countryCode, year), point);
                point.pop_back();
              }                         
              if (settings.parametersTable.find("cai m3ha") != settings.parametersTable.end()) {
                point.push_back("cai m3ha");
                obj.insert(CountriesCAI_tw.get(countryCode, year)/CountriesOforCover.get(countryCode, year), point);
                point.pop_back();
              }                         
              if (settings.parametersTable.find("area_df hayear") != settings.parametersTable.end()) {
                point.push_back("area_df hayear");
                obj.insert(CountriesDeforHaYear_tw.get(countryCode, year), point);
                point.pop_back();
              }       
              if (settings.parametersTable.find("area_af hayear") != settings.parametersTable.end()) {
                point.push_back("area_af hayear");
                obj.insert(CountriesAfforHaYear_tw.get(countryCode, year), point);
                point.pop_back();
              }                     
              if (settings.parametersTable.find("harvest_total m3year") != settings.parametersTable.end()) {
                point.push_back("harvest_total m3year");
                obj.insert(CountriesWoodHarvestM3Year_tw.get(countryCode, year), point);
                point.pop_back();
              }                            	
              if (settings.parametersTable.find("harvest_fm m3hayear") != settings.parametersTable.end()) {
                point.push_back("harvest_fm m3hayear");
                obj.insert(CountriesWoodHarvestFmM3Year_tw.get(countryCode, year)/CountriesManagedForHa.get(countryCode, year), point);
                point.pop_back();
              }                            	
               if (settings.parametersTable.find("harvest_df m3hayear") != settings.parametersTable.end()) {
                point.push_back("harvest_df m3hayear");
                obj.insert(CountriesWoodHarvestDfM3Year_tw.get(countryCode, year)/CountriesDeforHaYear.get(countryCode, year), point);
                point.pop_back();
              }                     
              if (settings.parametersTable.find("biom_fm tc") != settings.parametersTable.end()) {
                point.push_back("biom_fm tc");
                obj.insert(CountriesOforC_biom_tw.get(countryCode, year), point);
                point.pop_back();
              }                            	
              if (settings.parametersTable.find("biom_af tc") != settings.parametersTable.end()) {
                point.push_back("biom_af tc");
                obj.insert(CountriesNforTotC_tw.get(countryCode, year), point);
                point.pop_back();
              }       
              if (settings.parametersTable.find("em_df_bm mtco2year") != settings.parametersTable.end()) {
                point.push_back("em_df_bm mtco2year");
                obj.insert(CountriesDeforCYear_biom_tw.get(countryCode, year), point);
                point.pop_back();
              }      
              if (settings.parametersTable.find("em_df_sl mtco2year") != settings.parametersTable.end()) {
                point.push_back("em_df_sl mtco2year");
                obj.insert(CountriesDeforCYear_soil_tw.get(countryCode, year), point);
                point.pop_back();
              }      
              if (settings.parametersTable.find("em_af_bm mtco2year") != settings.parametersTable.end()) {
                point.push_back("em_af_bm mtco2year");
                obj.insert(CountriesAfforCYear_biom_tw.get(countryCode, year), point);
                point.pop_back();
              }                    
              if (settings.parametersTable.find("em_af_sl mtco2year") != settings.parametersTable.end()) {
                point.push_back("em_af_sl mtco2year");
                obj.insert(CountriesAfforCYear_soil_tw.get(countryCode, year), point);
                point.pop_back();
              }       
              if (settings.parametersTable.find("rotation_avg year") != settings.parametersTable.end()) {
                point.push_back("rotation_avg year");
                obj.insert(CountryRotation_tw.getAvg(countryCode, year), point);
                point.pop_back();
              }                                                   

// Add parameters you want to output here as for "Forest area":
// The parameters you may want to output must be defined in listOfParameters.txt
// file used by the GUI. The parameters for output must be defined in settings.ini 
// created by the GUI for g4m. 
            if (settings.tabs[2]) point.pop_back();
            }
          }
          if (settings.tabs[1]) point.pop_back();
        }
      }
      if (settings.tabs[0]) point.pop_back();
    }
  }

//----------------
//--------------------------------------------------

set<string> countryregNameList;      // coutry Names to be considered in the table GUI
{
set<int>::iterator it;
set<string>::iterator itn;
itn=countryregNameList.begin();
for (it=countryregList.begin(); it!=countryregList.end(); it++){
//cout<<"countryCode=\t"<<*it<<"\tcountryCodeOrder="<<countryCodeOrder[(*it-1)]<<"\tcountryName=\t"<<counrtyOrderName[countryCodeOrder[(*it-1)]]<<endl;
string tmp = countryRegName[(*it)]; 				   	
countryregNameList.insert(itn,tmp);
itn++;
}  
} 
//--------  
  tableData objReg;
  if (settings.produceTabs) {
    if (settings.tabs[0]) objReg.addDim("Scenario", IntToStr(0));
    if (settings.tabs[1]) objReg.addDim("Year", years);
    if (settings.tabs[2]) objReg.addDim("CountryRegMix", countryregNameList);
    /*if (settings.tabs[3])*/ objReg.addDim("Parameter", settings.parametersTableReg);
    point.clear();
    for (int prc = 0; prc < 1; prc++) {
      if (settings.tabs[0]) point.push_back(IntToStr(prc));
      for (int year = byear; year <= eyear; year++) {
        if (years.find(year) != years.end()) {
          if (settings.tabs[1]) point.push_back(IntToStr(year));
          for (int countryreg = 1; countryreg < 50; countryreg++) {
//              if (settings.tabs[2]) point.push_back(IntToStr(countryreg));
              if (settings.tabs[2]) point.push_back(countryRegName[countryreg]);
              if (settings.parametersTableReg.find("harvest_total m3year") != settings.parametersTableReg.end()) {
                point.push_back("harvest_total m3year");
                objReg.insert(CountryregWoodHarvestM3Year_tw.get(countryreg, year), point);
                point.pop_back();
              }                            	
              if (settings.parametersTableReg.find("harvest_demand m3year") != settings.parametersTableReg.end()) {
                point.push_back("harvest_demand m3year");
                objReg.insert(CountryregWprod_tw.get(countryreg, year), point);
                point.pop_back();
              }                            	
              if (settings.parametersTableReg.find("harvest_fm m3year") != settings.parametersTableReg.end()) {
                point.push_back("harvest_fm m3year");
                objReg.insert(CountryregWoodHarvestFmM3Year_tw.get(countryreg, year), point);
                point.pop_back();
              }                            	
               if (settings.parametersTableReg.find("harvest_df m3year") != settings.parametersTableReg.end()) {
                point.push_back("harvest_df m3year");
                objReg.insert(CountryregWoodHarvestDfM3Year_tw.get(countryreg, year), point);
                point.pop_back();
              }                                            	
              if (settings.parametersTableReg.find("rotation_avg year") != settings.parametersTableReg.end()) {
                point.push_back("rotation_avg year");
                objReg.insert(CountryregRotation_tw.getAvg(countryreg, year), point);
                point.pop_back();
              }                   
       
// Add parameters you want to output here as for "Forest area":
// The parameters you may want to output must be defined in listOfParameters.txt
// file used by the GUI. The parameters for output must be defined in settings.ini 
// created by the GUI for g4m. 
            if (settings.tabs[2]) point.pop_back();
            
          }
          if (settings.tabs[1]) point.pop_back();
        }
      }
      if (settings.tabs[0]) point.pop_back();
    }
  }  
  
cout << "****" << endl;
//*************************************
//** Saving data to files
//*************************************
  if (settings.produceTabs) {
    obj.rename("G4M results for countries");
    point.clear();
    if (settings.tabs[0]) point.push_back("Scenario");
    if (settings.tabs[1]) point.push_back("Year");
    if (settings.tabs[2]) point.push_back("Country");
    /*if (settings.tabs[3])*/ point.push_back("Parameter");
    obj.renameDims(point);

    obj.SaveToFile(settings.outputPath, "tabs_gui"+suffix);

    objReg.rename("G4M results for countryregions");
    point.clear();
    if (settings.tabs[0]) point.push_back("Scenario");
    if (settings.tabs[1]) point.push_back("Year");
    if (settings.tabs[2]) point.push_back("CountryRegMix");
    /*if (settings.tabs[3])*/ point.push_back("Parameter");
    objReg.renameDims(point);

    objReg.SaveToFile(settings.outputPath, "tabs_reg_gui"+suffix);
  }  
//**********************************
  if (settings.produceMaps) {
//    point.clear();
//    if (settings.tabs[0]) point.push_back("Scenario");
//    if (settings.tabs[1]) point.push_back("Year");
//    if (settings.tabs[2]) point.push_back("Parameter");
//    obj.renameDims(point);
    ASU.SaveToFile(settings.outputPath, "-EU"+suffix);
  }
//  system("pause");
 //******************************
cout << "written to GUI files"<<endl;
}

//------------------------------------------------------------------------------
  if ((forNPVcurves) || (forNPVcuvvesDyn)){ 
    ofstream f;
    string fileName = "NPV"+suffix+".txt";
    f.open(fileName.c_str(),ios::out);
      if (f.is_open()) {
         f << "CountryN/Rotation\t minRot\t minMed\t medRot\t medMax\t maxRot\t\t rotMinYears\t rotMaxYears"<<endl; 

        for (int i=0;i<209;i++)
		{
			set<int> countriesToPrint = CountriesNforCover.getListOfCountries();
			if (countriesToPrint.find(i+1) != countriesToPrint.end())
			{
				f<<i+1<<"\t";
				f<<minRotNPV[i]<<"\t";
				f<<minMedNPV[i]<<"\t";
				f<<medRotNPV[i]<<"\t";
				f<<medMaxNPV[i]<<"\t";
				f<<maxRotNPV[i]<<"\t\t";
				f<<minRot[i]<<"\t";
				f<<maxRot[i]<<endl;                                            
			}
		}
       f.close();
    }
  }
            

//    minRotNPV[i]=0.;   // country average NPV if max MAI rotation is applied to all forests
//    minMedNPV[i]=0.;   // NPV at rotation between min and medium rotation
//    medRotNPV[i]=0.;   // NPV at rotation between min and medium rotation
//    medMaxNPV[i]=0.;     // NPV at rotation between medium and max biomass rotation
//    maxRotNPV[i]=0.;  // country average NPV if max biomass rotation is applied to all forests
//    minRot[i]=0.;    // country average min rotation (max harvest)
//    maxRot[i]=0.;    // country average max biomass rotation
//---------------------  

if (fmpol&&bau&&PriceC==0){
cout << "writing bau files"<<endl;
dataDetStruct::iterator it;
int year = refYear+1;
 do {
    it = plots.begin();
    vector<double> tmp_npv;    
    while (it != plots.end()) {
//        if (it->PROTECT[2000]==0) {
        if ((it->PROTECT[2000]==0)&&((it->POTVEG[2000]<10 && it->FOREST[2000]>0 && it->MAIE[2000]>0)||(it->POTVEG[2000]<10 && (it->NPP[2000]>0)||it->MAIN[2000]>0))){                          
              if (regions.find(it->POLESREG[2000]) != regions.end()) { // Test only some regions
                  int asID = it->asID;                  
                  double sum = 0.;
                  int j=year;
                  do {
                      sum += Profit_bau[j-refYear-1][asID]/(pow(1+it->R[2000],(j-year)));
                      j++;
//                      } while (j <= eyear);
                      } while ((j-year<=50)&&(j <= eyear));                      
//cout<< "sum="<<"\t"<<sum<<endl;
                      tmp_npv.push_back(sum);
              }
        }
       it++;
   }
                  NPVbau.push(tmp_npv);
                  year++;
 } while (year <= eyear);
// } while ((year<=2050)&&(year <= eyear));
//}                  

//           if (fmpol && PriceC==0){
                biomass_bau.saveToFile("biomass_bau");
                NPVbau.saveToFile("NPVbau");
            
cout<< "finished writing bau files"<<endl;
 }
//------------------------------------------------------------------------------ 
string prefix="out\\";
if (countryOutput){
     CountriesNforCover.printToFile(prefix + "newForestHa"+suffix+".txt",coeff.bYear,coeff.eYear,1);
     CountriesAfforHaYear.printToFile(prefix + "afforHaYear"+suffix+".txt",coeff.bYear,coeff.eYear,1);          

  
     CountriesNforTotC.printToFile(prefix + "afforSinkC" + suffix+".txt",coeff.bYear,coeff.eYear,1); 
     CountriesAfforCYear.printToFile(prefix + "afforSinkGgCO2Year" + suffix+".txt",coeff.bYear,coeff.eYear,1); 
     CountriesAfforCYear_ab.printToFile(prefix + "afforSinkGgCO2Year_ab" + suffix+".txt",coeff.bYear,coeff.eYear,1);  
     CountriesAfforCYear_bl.printToFile(prefix + "afforSinkGgCO2Year_bl" + suffix+".txt",coeff.bYear,coeff.eYear,1);   
     CountriesAfforCYear_biom.printToFile(prefix + "afforSinkGgCO2Year_biom" + suffix+".txt",coeff.bYear,coeff.eYear,1);
     CountriesAfforCYear_dom.printToFile(prefix + "afforSinkGgCO2Year_dom" + suffix+".txt",coeff.bYear,coeff.eYear,1);    
     CountriesAfforCYear_soil.printToFile(prefix + "afforSinkGgCO2Year_soil" + suffix+".txt",coeff.bYear,coeff.eYear,1);
    
// //--------- 
     CountriesOforCover.printToFile(prefix + "OforestHa" + suffix+".txt",coeff.bYear,coeff.eYear,1);

     CountriesDeforHaYear.printToFile(prefix + "deforHaYear" + suffix+".txt",coeff.bYear,coeff.eYear,1); 
     
     CountriesOfor_abC.printToFile(prefix + "OforC_ab" + suffix+".txt",coeff.bYear,coeff.eYear,1);
     CountriesOforC_biom.printToFile(prefix + "OforC_biom" + suffix+".txt",coeff.bYear,coeff.eYear,1);  
     CountriesDeforCYear.printToFile(prefix + "deforEmGgCO2Year_tot" + suffix+".txt",coeff.bYear,coeff.eYear,1);   
     CountriesDeforCYear_bl.printToFile(prefix + "deforEmGgCO2Year_bl" + suffix+".txt",coeff.bYear,coeff.eYear,1);   
     CountriesDeforCYear_ab.printToFile(prefix + "deforEmGgCO2Year_ab" + suffix+".txt",coeff.bYear,coeff.eYear,1); 
     CountriesDeforCYear_biom.printToFile(prefix + "deforEmGgCO2Year_biom" + suffix+".txt",coeff.bYear,coeff.eYear,1);     
     CountriesDeforCYear_dom.printToFile(prefix + "deforEmGgCO2Year_dom" + suffix+".txt",coeff.bYear,coeff.eYear,1);     
     CountriesDeforCYear_soil.printToFile(prefix + "deforEmGgCO2Year_soil" + suffix+".txt",coeff.bYear,coeff.eYear,1);      

////--------- 

     CountriesWoodHarvestM3Year.printToFile(prefix + "harvest_m3_year" + suffix+".txt",coeff.bYear,coeff.eYear,1); 
     CountriesWoodHarvestPlusM3Year.printToFile(prefix + "harvestPlus_m3_year" + suffix+".txt",coeff.bYear,coeff.eYear,1);      
     CountriesWoodHarvestFmM3Year.printToFile(prefix + "harvestFm_m3_year" + suffix+".txt",coeff.bYear,coeff.eYear,1);      
     CountriesWoodHarvestDfM3Year.printToFile(prefix + "harvestDf_m3_year" + suffix+".txt",coeff.bYear,coeff.eYear,1);           
     CountriesWoodLoosCYear.printToFile(prefix + "harvest_lostGgCO2Year" + suffix+".txt",coeff.bYear,coeff.eYear,1);
     CountriesHarvLossesYear.printToFile(prefix + "harvLossesYear" + suffix+".txt",coeff.bYear,coeff.eYear,1);
////---------     
     CountriesManagedForHa.printToFile(prefix + "managedForHa" + suffix+".txt",coeff.bYear,coeff.eYear,1);
     CountriesManagedCount.printToFile(prefix + "managedCount" + suffix+".txt",coeff.bYear,coeff.eYear,1); 
     
     CountriesMAI.printToFile(prefix + "mai" + suffix+".txt",coeff.bYear,coeff.bYear,1,"AVG");
     CountriesCAI.printToFile(prefix + "cai" + suffix+".txt",coeff.bYear,coeff.eYear,1);     // total current increment m3/year
     CountriesCAI_new.printToFile(prefix + "caiNew" + suffix+".txt",coeff.bYear,coeff.eYear,1); // total current increment m3/year

     CountriesFM.printToFile(prefix + "FM_GgCO2Year" + suffix+".txt",coeff.bYear,coeff.eYear,1);
     CountriesFMbm.printToFile(prefix + "FMbm_GgCO2Year" + suffix+".txt",coeff.bYear,coeff.eYear,1);     
//------------------------------------------------------------------------------ 

//     CountriesWprod.printToFile(prefix + "wprod_test" + suffix,coeff.bYear,coeff.eYear,1);
//------------------------------------------------------------------------------     
     CountriesProfit.printToFile(prefix + "profit" + suffix+".txt",coeff.bYear,coeff.eYear,1); 

////------------------------------------------------------------------------------
CountryregWprod.printToFile(prefix + "wprod_reg" + suffix+".txt",coeff.bYear,coeff.eYear,1);
CountryregWoodHarvestM3Year.printToFile(prefix + "harvest_reg" + suffix+".txt",coeff.bYear,coeff.eYear,1);

}
    fff.close(); 
//  } 

//-- Writing ASCII grid to file -------------------------------------------------
if (gridOutput){ 
//SD_grid_1990.printToFile("sd_1990" + suffix);
SD_grid_00.PrintToFile("sd_2000" + suffix);
SD_grid_10.PrintToFile("sd_2010" + suffix);

//harvestm3_grid_20.printToFile("havest_20" + suffix);
//bmabtC_of_grid_20.printToFile("bmabtC_of_20" + suffix);
//bmabtC_nf_grid_20.printToFile("bmabtC_nf_20" + suffix);
//fmgGco2_grid_20.printToFile("fmgGco2_20" + suffix);
//mai_m3ha_grid_20.printToFile("mai_m3ha_20" + suffix);
//SD_grid_20.printToFile("sd_20" + suffix);
//RL_grid_20.printToFile("rl_20" + suffix);

//harvestm3_grid_30.printToFile("havest_30" + suffix);
//bmabtC_of_grid_30.printToFile("bmabtC_of_30" + suffix);
//bmabtC_nf_grid_30.printToFile("bmabtC_nf_30" + suffix);
//fmgGco2_grid_30.printToFile("fmgGco2_30" + suffix);
//mai_m3ha_grid_30.printToFile("mai_m3ha_30" + suffix);
//SD_grid_30.printToFile("sd_30" + suffix);
//RL_grid_30.printToFile("rl_30" + suffix);

//harvestm3_grid_50.printToFile("havest_50" + suffix);
//bmabtC_of_grid_50.printToFile("bmabtC_of_50" + suffix);
//bmabtC_nf_grid_50.printToFile("bmabtC_nf_50" + suffix);
//fmgGco2_grid_50.printToFile("fmgGco2_50" + suffix);
//mai_m3ha_grid_50.printToFile("mai_m3ha_50" + suffix);
//SD_grid_50.printToFile("sd_50" + suffix);
//RL_grid_50.printToFile("rl_50" + suffix);
}
//-----------------------------------------------------------------------------
                               // End loop by prices
  cout << "> Working time is " << difftime(time(NULL),start)/60. << " min." << endl;
//  system("pause");
	return 0;
}
//******************************************************************************
// end main
//******************************************************************************


