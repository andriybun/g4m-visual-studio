#include "forest.h" 

extern g4m::coeffStruct coeff;

extern set<int> regions;
extern set<int> toAdjust;
extern set<int> doneList;
extern set<int> countriesList;
extern set<int> countryregList;

extern map<string, g4m::ipol<double,double> > wprod;

extern vector2d NPVbau;

extern double FMs[NumberOfCountries];
extern double FM_sink_stat[NumberOfCountries];
extern double harvDiff[NumberOfCountries];

extern double resUse;
extern double tolerance;

extern int eyear, byear, refYear;
extern int ResLatitude, ResLongitude;
extern int maxDiffCountry;

extern bool fmpol;
extern bool adjustFMsink;

void adjustManagedForest(dataDetStruct &data_all, g4m::incrementTab &fi, ageStructVector &cohort_all, 
              ageStructVector &newCohort_all, datGlobal &dat_all, griddata2<double> &maiForest, 
              griddata2<double> &thinningForest, griddata2<double> &rotationForest, griddataLite<char> &managedForest,
              griddata2<double> &rotationForestNew, griddata2<double> &thinningForestNew, griddataLite<char> &manageChForest,
              griddataLite<char> &rotationType, griddata2<double> &harvestGrid, int year, griddataLite<char> &unmanaged, double priceC) 
{
bool harvControl = true; // Additional information to control output of the fm_cpol module
toAdjust.clear();
//toAdjust = countriesList;     // List of considered countries
toAdjust = countryregList;     // List of considered countries & regions
//    toAdjust.clear();
//    toAdjust.insert(11);     // test Austria
//    toAdjust.insert(114);     // test Austria
//    toAdjust.insert(61);     // test Finland
//    toAdjust.insert(69);     // test Germany 

  double woodHarvest[NumberOfCountryregmix];
  woodHarvest[0]=0.;
  for (int i=1; i<NumberOfCountryregmix; i++){
    woodHarvest[i]= 0.;
//    woodHarvest[i]=CountriesWoodHarvestPlusM3Year.get(i, year-1); 
    FMs[i]=0.;
//cout<<"woodHarvest["<<i<<"]= "<<woodHarvest[i]<<endl;
//    woodLost[i]=0.;
//    woodHarvestStat[i]=0.;
//    managedCount[i]=0;
  }


if (year == byear) {
         dataDetStruct::iterator iter = data_all.begin();
         iter = data_all.begin();
         while (iter != data_all.end()) {
           if (regions.find(iter->POLESREG[2000]) != regions.end()) { // Test only some regions
            short int country = iter->COUNTRY[2000];
            if (countriesList.find(country) != countriesList.end()) { // Test only some countries                  
             if (iter->PROTECT[2000]==0) {
               int xi = (iter->x);
               int yi = (iter->y);
               if (thinningForest.get(xi,yi)<0) {
//cout<<"thinning<0 = \t"<<thinningForest.get(xi,yi)<<endl;
               unmanaged.set(xi,yi,1);
//cout<<"thinning<0=\t"<<thinningForest.get(xi,yi)<<"\t unmanaged=\t"<<unmanaged.get(xi,yi)<<endl;               
               }
            }
           }
          }
           iter++;
        }
}

//------------------------------------------------------------------------------
//----------------------- Gradualy adjust thinning -----------------------------
//------------------------------------------------------------------------------ 
 dataDetStruct::iterator iter = data_all.begin(); 
 while (iter != data_all.end())
 {
  int region = iter->POLESREG[2000];
  if (regions.find(region) != regions.end()) { // Test only some regions   
   short int country = iter->COUNTRY[2000];
   if (countriesList.find(country) != countriesList.end()) { // Test only some countries                  
    if ((iter->PROTECT[2000]==0)&&((iter->POTVEG[2000]<10 && iter->FOREST[2000]>0 && iter->MAIE[2000]>0)||(iter->POTVEG[2000]<10 && (iter->NPP[2000]>0)||iter->MAIN[2000]>0))){ 
        int asID = iter->asID;
        int xi = (iter->x);
        int yi = (iter->y);
    //cout<<"thinning<0=\t"<<thinningForest.get(xi,yi)<<"\t unmanaged=\t"<<unmanaged.get(xi,yi)<<endl;     
        double thinningTmp = thinningForest.get(xi,yi);
        if (thinningTmp>1 && manageChForest.get(xi,yi)>0) {thinningTmp -= 0.025; 
              if (thinningTmp<1) thinningTmp = 1;
              thinningForest.set(xi,yi,thinningTmp); cohort_all[asID]->setStockingdegree(thinningTmp);}
    //    if (thinningTmp>1 && manageChForest.get(xi,yi)>0) {thinningTmp -= 0.1; thinningForest.set(xi,yi,thinningTmp); cohort_all[asID]->setStockingdegree(thinningTmp);}
    //    if (thinningTmp>1 && manageChForest.get(xi,yi)>0) {thinningTmp -= 0.05; thinningForest.set(xi,yi,thinningTmp); cohort_all[asID]->setStockingdegree(thinningTmp);}
        double thinningTmpNew = thinningForestNew.get(xi,yi);
        if (thinningTmpNew>1) {thinningTmpNew -= 0.025; 
               if (thinningTmpNew<1) thinningTmpNew = 1;
               thinningForestNew.set(xi,yi,thinningTmpNew); newCohort_all[asID]->setStockingdegree(thinningTmpNew);} 
    //    if (thinningTmpNew>1) {thinningTmpNew -= 0.1; thinningForestNew.set(xi,yi,thinningTmpNew); newCohort_all[asID]->setStockingdegree(thinningTmpNew);} 
    //    if (thinningTmpNew>1) {thinningTmpNew -= 0.05; thinningForestNew.set(xi,yi,thinningTmpNew); newCohort_all[asID]->setStockingdegree(thinningTmpNew);} 
     }
    } 
  }
 iter++; 
 }



//------------------------------------------------------------------------------
       

//       adjustManagedForest(plots, fi, cohort_all, 
//              newCohort_all, dat_all, maiForest, 
//              thinningForest, rotationForest, managedForest,
//              rotationForestNew, thinningForestNew, manageChForest,
//              rotationType, harvestGrid, year, unmanaged);

// void fm_Cpol(dataDetStruct &data_all, g4m::incrementTab &fi, ageStructVector &cohort_all, 
//              ageStructVector &newCohort_all, datGlobal &dat_all, griddata2<double> &maiForest, 
//              griddata2<double> &thinningForest, griddata2<double> &rotationForest, griddataLite<char> &managedForest,
//              griddata2<double> &rotationForestNew, griddata2<double> &thinningForestNew, griddataLite<char> &manageChForest,
//              griddataLite<char> &rotationType, griddata2<double> &harvestGrid, int year, griddataLite<char> &unmanaged, int fm_hurdle, double &maxDiff) 

if (fmpol && priceC>0 && year>refYear)
{
double maxDiff=0;
double fm_hurdle=1.;
double diffMaxDiff = 1.;
double fm_hurdlei[] = {0,0,0,0,0,0};
double maxDiffi[] = {0,0,0,0,0,0};
double maxDiff0[NumberOfCountryregmix+1];

  set<int> useChange;
  griddata2<double> thinningForestO = griddata2<double>(ResLongitude, ResLatitude,0.);
  griddataLite<int> rotationForestO = griddataLite<int>(ResLongitude, ResLatitude,0);
  griddataLite<int> rotationForestNewO = griddataLite<int>(ResLongitude, ResLatitude,0);
  
fm_cpol(data_all, fi, cohort_all,
        newCohort_all, dat_all, maiForest, 
        thinningForest, rotationForest, managedForest,
        rotationForestNew, thinningForestNew, manageChForest,
        rotationType, harvestGrid, year, unmanaged, fm_hurdle,maxDiff, priceC,
        useChange, thinningForestO,rotationForestO,rotationForestNewO);
cout<<"maxDiff = "<<"\t"<<maxDiff<<"\t"<<"fm_hurdle = "<<"\t"<<fm_hurdle<<"\t"<<"diffMaxDiff = "<<"\t"<<diffMaxDiff<<endl;        
//    maxDiff0[0] = maxDiff;

    for (int i=0;i<NumberOfCountryregmix;i++){maxDiff0[i] = harvDiff[i];}
    doneList.clear();

  while (doneList.size()< countryregList.size()
//             && maxDiff > 0.05
             ){

    while (doneList.find(maxDiffCountry)!=doneList.end()){
        harvDiff[maxDiffCountry]=0;
        maxDiff = *max_element(harvDiff+1,harvDiff+NumberOfCountryregmix+1);
        for (int j=0;j<=NumberOfCountryregmix;j++){if (harvDiff[j] == maxDiff){maxDiffCountry = j; 
                                                                 break;}}
    }
//    if (maxDiff<=0.05) break;
    if (maxDiff<=0.03) break;    
                                                                 
    toAdjust.clear();
    toAdjust.insert(maxDiffCountry);       
    int countryDone = maxDiffCountry;
    maxDiffi[0] = maxDiff0[maxDiffCountry];
    int i = 1;
    double maxDiffPrev = maxDiff0[maxDiffCountry];
        fm_hurdle=1;
//        while (maxDiff > 0.05
        while (maxDiff > 0.03        
//                               && fm_hurdle > -1 
                               && fm_hurdle > -2      // Try for Ireland 
    //                                          && diffMaxDiff >= 0
                                                   && (i <= 25 && fm_hurdle >= -1) // Try for Ireland 
                                                       && (i <= 50) // Try for Ireland 
                                                   
                                                                ) { 

          if (diffMaxDiff <= 0.02){                                                              
            if (fm_hurdle<2.5 && fm_hurdle>0) {fm_hurdle+=0.3;}
            else if (fm_hurdle < 100000){fm_hurdle *= 100000.;}
            else if (fm_hurdle > 100000 && fm_hurdle < 300000){fm_hurdle *= 4.;}      // Try for Ireland
            else if (fm_hurdle == 1000000){fm_hurdle = -1.;}    // Try for Ireland        
            else if (fm_hurdle == -1) {fm_hurdle=-2.;}                      // Try for Ireland        
            else  {fm_hurdle=-2.;} 
          }     

    //      fm_hurdle=0.5;
//cout<<"fm_hurdle_loop = \t"<<fm_hurdle<<endl;
          fm_cpol(data_all, fi, cohort_all,
            newCohort_all, dat_all, maiForest, 
            thinningForest, rotationForest, managedForest,
            rotationForestNew, thinningForestNew, manageChForest,
            rotationType, harvestGrid, year, unmanaged, fm_hurdle,maxDiff, priceC,
            useChange, thinningForestO,rotationForestO,rotationForestNewO);
                    
            diffMaxDiff = maxDiffPrev - maxDiff;
            maxDiffPrev = maxDiff;
            maxDiffi[i] = maxDiff;
            fm_hurdlei[i] = fm_hurdle;
cout<<"maxDiff = "<<"\t"<<maxDiff<<"\t"<<"fm_hurdle_loop = "<<"\t"<<fm_hurdle<<"\t"<<"diffMaxDiff = "<<"\t"<<diffMaxDiff<<endl;        
            i++;
        }
    
////         if (maxDiff > 0.05) {
//         if (maxDiff > 0.03) {
//         double minMaxDiff = *min_element(maxDiffi,maxDiffi+6);
//         int k = 100;
//         for (int j=0;j<=5;j++){if (maxDiffi[j] == minMaxDiff){k=j; 
//                                                                 break;}}
//       
//             if (k<100&&k!=5){
//                fm_cpol(data_all, fi, cohort_all,
//                        newCohort_all, dat_all, maiForest, 
//                        thinningForest, rotationForest, managedForest,
//                        rotationForestNew, thinningForestNew, manageChForest,
//                        rotationType, harvestGrid, year, unmanaged, fm_hurdlei[k],maxDiff, priceC,
//                        useChange, thinningForestO,rotationForestO,rotationForestNewO);
//cout<<"maxDiff = "<<"\t"<<maxDiff<<"\t"<<"fm_hurdle[k] = "<<"\t"<<fm_hurdlei[k]<<"\t"<<"k = "<<"\t"<<k<<endl;  
//             }             
//         } 
      doneList.insert(countryDone);
  } 

cout<<"----------------------"<<endl;
//for (int i=0;i<=8;i++){
//cout<<"maxDiffi[i]=\t"<<maxDiffi[i]<<"\t fm_hurdlei[i]= \t"<<fm_hurdlei[i]<<"\t i=\t"<<i<<endl;
//cout<<"----"<<endl;
//}
bool NPV_postcontrol = false;
if (NPV_postcontrol){
iter = data_all.begin();
while (iter != data_all.end()) {
  short int region = iter->POLESREG[2000];
  if (regions.find(region) != regions.end()) { // Test only some regions         
    short int country = iter->COUNTRY[2000];
//    short int countryreg = (int)iter->COUNTRYREGMIX.v();                                                             
    if (countriesList.find(country) != countriesList.end()) {  
     int asID = iter->asID;
     if ((iter->PROTECT[2000]==0)&&((iter->POTVEG[2000]<10 && iter->FOREST[2000]>0 && iter->MAIE[2000]>0)||(iter->POTVEG[2000]<10 && (iter->NPP[2000]>0)||iter->MAIN[2000]>0))){
    char regionch[2];
    int2str(region,regionch);
    string regprice = "re"+string(regionch)+"price0";
    string regprice0 = "re"+string(regionch)+"price0";
//    char countrych[3];
//    int2str(country,countrych);
//    string countryprice = "re"+string(countrych)+"price0";
//    country = iter->COUNTRY[2000];     
    int xi = (iter->x);
    int yi = (iter->y);

  coeff.PriceC.clear();
  coeff.PriceC.insert(0, priceC * iter->CORRUPTION.v());   
  double NPVtmp = 0.;
  double maiV = maiForest.get(xi,yi)*iter->FTIMBER.v();
  g4m::ageStruct cohortTmp = *cohort_all[asID];
  short int used = 0;
  if (thinningForest.get(xi,yi)>0){used = 1;}
  NPVtmp = npv_calc(*iter, cohortTmp, maiV,year,rotationForest.get(xi,yi),
                         regprice,regprice0,dat_all[asID].OforestShare,used)
                                  *dat_all[asID].LandAreaHa * dat_all[asID].OforestShare;    

cout<<"asID=\t"<<asID<<"\tcountry=\t"<<country<<"\tNPVbau=\t"<<NPVbau[year-refYear-1][asID]<<"\tNPVcur=\t"<<NPVtmp<<endl;
    }
   }
  }
  iter++;
 }
}// end for NPV_postcontrol

}
else
{

iter = data_all.begin();

while (iter != data_all.end()) {
  short int region = iter->POLESREG[2000];
  if (regions.find(region) != regions.end()) { // Test only some regions         
     short int country = iter->COUNTRY[2000];
     if (countriesList.find(country) != countriesList.end()) { // Test only some countries       
       
       if ((iter->PROTECT[2000]==0)&&((iter->POTVEG[2000]<10 && iter->FOREST[2000]>0 && iter->MAIE[2000]>0)||(iter->POTVEG[2000]<10 && (iter->NPP[2000]>0)||iter->MAIN[2000]>0))){
//          short int countryreg = (int)iter->COUNTRYREGMIX.v();                                                             
          short int countryreg = (int)iter->COUNTRYREGMIX.v();                                                             
          int asID = iter->asID;
          int xi = (iter->x);
          int yi = (iter->y);
          double sawnW = 0.;
          double restW = 0.;
          double sawnThW = 0.;
          double restThW = 0.;
          double sawnWnew = 0.;
          double restWnew = 0.;
          double sawnThWnew = 0.;
          double restThWnew = 0.;
        
          double harvestTmp = 0;
          double newHarvestTmp = 0;
//if (country==135){cout<<"thinningForest(FM0B)\t"<<thinningForest.get(xi,yi)<<"\tmanagedForest\t"<<int(managedForest.get(xi,yi))<<endl;}
//          if (dat_all[asID].ObiomassPrev >0 && iter->CABOVEHA.v() > 0 && maiForest.get(xi,yi) > 0 && thinningForest.get(xi,yi) >0)  
          if (thinningForest.get(xi,yi) >0)  
          {
//                double biomassRotTh = fi.gUt(dat_all[asID].ObiomassPrev, maiForest.get(xi,yi), thinningForest.get(xi,yi));     // rotation time to get current biomass (without thinning)  
                g4m::ageStruct cohortTmp = *cohort_all[asID];
                g4m::ageStruct cohortTmpNew = *newCohort_all[asID];
            	harvestTmp = harvestGrid.get(xi,yi);
                g4m::ageStruct::v resTmp = cohortTmp.aging();
                sawnW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
                restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
                sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
                restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.

                g4m::ageStruct::v resTmpNew = cohortTmpNew.aging();
                sawnWnew = resTmpNew.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
                restWnew = resTmpNew.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
                sawnThWnew = resTmpNew.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
                restThWnew = resTmpNew.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            

                newHarvestTmp = ((sawnW + restW + sawnThW + restThW + resUse*resTmp.hv) * (dat_all[asID].OforestShare-dat_all[asID].deforestShare)  +
                             (sawnWnew + restWnew + sawnThWnew + restThWnew +resUse*resTmpNew.hv) * dat_all[asID].AforestShare) * 
                              dat_all[asID].LandAreaHa * iter->FTIMBER.v() + dat_all[asID].deforWoodTotM3; // Expected total current harvested wood in the cell, m3

//                newHarvestTmp = ((sawnW + restW + sawnThW + restThW + resUse*resTmp.hv) * (dat_all[asID].OforestShare)  +
//                             (sawnWnew + restWnew + sawnThWnew + restThWnew +resUse*resTmpNew.hv) * dat_all[asID].AforestShare) * 
//                              dat_all[asID].LandAreaHa * iter->FTIMBER.v() + dat_all[asID].deforWoodTotM3; // Expected total current harvested wood in the cell, m3

//if (country==11){ cout<<"countryFM = \t"<<country<<"\t deforWoodTotM3 = \t"<<dat_all[asID].deforWoodTotM3<<"\tdeforest=\t"<<dat_all[asID].deforestShare<<"\toforest=\t"<<dat_all[asID].OforestShare<<endl;       }
//if (country==11){ cout<<"countryFM = \t"<<country<<"\t deforWoodTotM3 = \t"<<dat_all[asID].deforWoodTotM3;
//cout<<"\t harvest=\t"<<(sawnW + restW + sawnThW + restThW + resUse*resTmp.hv)*(dat_all[asID].OforestShare)*iter->FTIMBER.v();
//cout<<"\tnewHarvest=\t"<<(sawnWnew + restWnew + sawnThWnew + restThWnew +resUse*resTmpNew.hv) * dat_all[asID].AforestShare*iter->FTIMBER.v();
//cout<<"\t thinning=\t"<<thinningForest.get(xi,yi)<<"\trot=\t"<<rotationForest.get(xi,yi)<<endl;}
                 woodHarvest[countryreg] += (newHarvestTmp);
                 harvestGrid.set(xi,yi,newHarvestTmp);                       
        
           }else{
                 woodHarvest[countryreg] += dat_all[asID].deforWoodTotM3;
                 harvestGrid.set(xi,yi,dat_all[asID].deforWoodTotM3); 
           }                   
         }
      }
   }
  iter++;
}
//cout<<"year=\t"<<year<<"\t inputHarvest(47)=\t"<<woodHarvest[47]<<"\t harvestCalcPrev=\t"<<CountriesWoodHarvestM3Year.get(11, year-1)<<endl;


//year=2000;


 
//------------------------------------------------------------------------------
// 
// -------Zero Adjust thinning if population density changed --------------------
//
//------------------------------------------------------------------------------


if ((year > 2000) && ((year) % 10 == 0))
{
//cout<<"Start Zero Adjust thinning if population density changed"<<endl;
  iter = data_all.begin();
 while (iter != data_all.end())
 {
 short int region = iter->POLESREG[2000];
 if (regions.find(region) != regions.end()) { // Test only some regions         
  short int countryreg = (int)iter->COUNTRYREGMIX.v();                                                             
  if (toAdjust.find(countryreg) != toAdjust.end()) { // do only for some countries         
   if ((iter->PROTECT[2000]==0)&&((iter->POTVEG[2000]<10 && iter->FOREST[2000]>0 && iter->MAIE[2000]>0)||(iter->POTVEG[2000]<10 && (iter->NPP[2000]>0)||iter->MAIN[2000]>0))){
    int asID = iter->asID;
    short int country = iter->COUNTRY[2000];
    char regionch[2];
    int2str(region,regionch);
    string regprice = "re"+string(regionch)+"price0";
    string regprice0 = "re"+string(regionch)+"price0";
    char countrych[3];
    int2str(countryreg,countrych);
    string countryprice = "re"+string(countrych)+"price0";
    countryreg = iter->COUNTRYREGMIX[2000]; 
    int xi = (iter->x);
    int yi = (iter->y);
    
//  if (woodHarvest[country] < 0.9 * wprod[countryprice].v(year))
//  if (woodHarvest[country] < 0.85 * wprod[countryprice].v(year))  
  if (woodHarvest[countryreg] < 0.88 * wprod[countryprice].v(year))   
    {
//if (iter->COUNTRY[2000] == 197){cout<<"woodHarvest_0= "<<woodHarvest[region-1]<<"\t 0.9*woodHarvestStat= "<<0.9*wprod[countryprice].v(year) <<endl;}
     if (managedForest.get(xi,yi)<=0)
      {
      if ((iter->POPDENS.v(year) >0) && (iter->GDP.v(year) > 0))
      {
      double sawnW = 0.;
      double restW = 0.;
      double sawnThW = 0.;
      double restThW = 0.;
      double sawnWnew = 0.;
      double restWnew = 0.;
      double sawnThWnew = 0.;
      double restThWnew = 0.;
//      double harvestTmp = 0.;
      double newHarvestTmp = 0.;
      int biomassRot = 0;
      int rotMAI = 0;
      int rotMaxBmTh = 0;
      int Rotation = 0;
      double SD = 1.5;

//      int managedForestTmp = 0;
//      double thinningForestTmp = 0;
//      int rotationForestTmp = 0;
//      double thinningForestNewTmp = 0;
//      int managedForestNewTmp = 0;
//      int rotationForestNewTmp = 0;

      double countryHarvestTmp = woodHarvest[countryreg];
      
    g4m::ageStruct cohortTmpNew = *newCohort_all[asID]; 
    g4m::ageStruct cohortTmp = *cohort_all[asID];
   	double harvestTmp = harvestGrid.get(xi,yi);              

//cout << "xi= "<<xi<<"\t yi= "<<yi<<"\t MAI= "<< MAI<<"\t mai= "<< maiForest.get(xi,yi)<<"\t NPP= "<<iter->NPP[2000]<<"\t dNPP= "<<data["NPP"].v()<<endl; 

  if (dat_all[asID].ObiomassPrev >0 && iter->CABOVEHA.v() > 0 && maiForest.get(xi,yi) > 0)  
  {
          biomassRot = fi.gU(dat_all[asID].ObiomassPrev, maiForest.get(xi,yi), 1);     // rotation time to get current biomass (with thinning)  
          rotMAI = fi.gTopt(maiForest.get(xi,yi), 1);
          rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), 3);         
  }else if (dat_all[asID].prevPlantPhytHaBmGr >0)  
  {
          rotMAI = fi.gTopt(maiForest.get(xi,yi), 1);
          rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), 3);         
          biomassRot = rotMAI;
  }     
      int rotationForestTmp = rotationForest.get(xi,yi);
      int managedForestTmp = managedForest.get(xi,yi);
      double thinningForestTmp = thinningForest.get(xi,yi);
      int rotationForestNewTmp = rotationForestNew.get(xi,yi);
//     int managedForestNewTmp = managedForestNew.get(xi,yi);
      double thinningForestNewTmp = thinningForestNew.get(xi,yi);
      
//---------------------------------------------------
           int newForAge = newCohort_all[asID]->getActiveAge();
           if (newForAge > biomassRot)  // New forest age > rotation -> change FM for the new forest
           {

             if (managedForest.get(xi,yi) == 0)
              {
                         managedForest.set(xi,yi,3);
                         Rotation = rotMAI;
                         rotationType.set(xi,yi,1);
              }else 
              {          managedForest.set(xi,yi,2);
                         Rotation = rotMaxBmTh;
                         rotationType.set(xi,yi,3);
              }

                rotationForest.set(xi,yi,Rotation);	
                thinningForest.set(xi,yi,SD);
                manageChForest.set(xi,yi,1);	
                cohortTmp.setRotPeriod(Rotation);
                cohortTmp.setStockingdegree(SD);
                cohort_all[asID]->setRotPeriod(Rotation);
                cohort_all[asID]->setStockingdegree(SD);

                
                rotationForestNew.set(xi,yi,Rotation);	
                thinningForestNew.set(xi,yi,SD);	
                cohortTmpNew.setRotPeriod(Rotation);
                cohortTmpNew.setStockingdegree(SD);        
                newCohort_all[asID]->setRotPeriod(Rotation);
                newCohort_all[asID]->setStockingdegree(SD);  

           }else // New forest age <= rotation -> don't change FM for the new forest
           {
                   if (managedForest.get(xi,yi) == 0)
                   {
                         managedForest.set(xi,yi,3);
                         Rotation = rotMAI;
                         rotationType.set(xi,yi,1);
                   }else 
                   {     managedForest.set(xi,yi,2);
                         Rotation = rotMaxBmTh;
                         rotationType.set(xi,yi,3);
                   }

                rotationForest.set(xi,yi,Rotation);	
                thinningForest.set(xi,yi,SD);	
//                manageChForest.set(xi,yi,1);
                cohortTmp.setRotPeriod(Rotation);
                cohortTmp.setStockingdegree(SD);
                cohort_all[asID]->setRotPeriod(Rotation);
                cohort_all[asID]->setStockingdegree(SD);
             } // End    else // New forest age < rotation -> don't change FM for the new forest
                g4m::ageStruct::v resTmp = cohortTmp.aging();
                sawnW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
                restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
                sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
                restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.

                g4m::ageStruct::v resTmpNew = cohortTmpNew.aging();
                sawnWnew = resTmpNew.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
                restWnew = resTmpNew.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
                sawnThWnew = resTmpNew.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
                restThWnew = resTmpNew.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
                newHarvestTmp = ((sawnW + restW + sawnThW + restThW + resUse*resTmp.hv) * (dat_all[asID].OforestShare-dat_all[asID].deforestShare)  +
                             (sawnWnew + restWnew + sawnThWnew + restThWnew +resUse*resTmpNew.hv) * dat_all[asID].AforestShare) * 
                              dat_all[asID].LandAreaHa * iter->FTIMBER.v() + dat_all[asID].deforWoodTotM3; // Total current harvested wood in the cell, m3
                
                 
            
//              	harvestGrid.set(xi,yi,newHarvestTmp);
//                woodHarvest[countryreg] += (newHarvestTmp-harvestTmp);
                countryHarvestTmp += (newHarvestTmp-harvestTmp);
                
                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[countryreg] - wprod[countryprice].v(year)) 
                                   ){
                    woodHarvest[countryreg] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,newHarvestTmp);
           	        manageChForest.set(xi,yi,1);
                }else{ // return old values
                rotationForest.set(xi,yi,rotationForestTmp);	
                thinningForest.set(xi,yi,thinningForestTmp);
                managedForest.set(xi,yi,managedForestTmp);	                   
                cohort_all[asID]->setRotPeriod(rotationForestTmp);
                cohort_all[asID]->setStockingdegree(thinningForestTmp);
                rotationForestNew.set(xi,yi,rotationForestNewTmp);	
                thinningForestNew.set(xi,yi,thinningForestNewTmp);
//                managedForestNew.set(xi,yi,managedForestNewTmp)	                   
                newCohort_all[asID]->setRotPeriod(rotationForestNewTmp);
                newCohort_all[asID]->setStockingdegree(thinningForestNewTmp);                 
                }


        }  //End  if ((data["POPDENS"].v(1990) >0) && (data["GDP"].v(1990) > 0))
      } // End  if (managedForest(xi,yi)<=0)

// }else if (woodHarvest[countryreg] > 1.1 * wprod[countryprice].v(year)) 
// }else if (woodHarvest[countryreg] > 1.15 * wprod[countryprice].v(year))  
 }else if (woodHarvest[countryreg] > 1.12 * wprod[countryprice].v(year))  
 {
//if (iter->COUNTRY[2000] == 197){cout<<"woodHarvest_1= "<<woodHarvest[region-1]<<"\t 1.1*woodHarvestStat= "<<1.1*wprod[countryprice].v(year) <<endl;}       
    if (managedForest.get(xi,yi)>0) 
        {
     if ((iter->POPDENS.v(year) == 0) && (iter->GDP.v(year) == 0))
     {

      double sawnW = 0.;
      double restW = 0.;
      double sawnThW = 0.;
      double restThW = 0.;
      double sawnWnew = 0.;
      double restWnew = 0.;
      double sawnThWnew = 0.;
      double restThWnew = 0.;
//      double harvWoodTmp = 0.;
//      double harvestTmp = 0.;
      double newHarvestTmp = 0.;
      int biomassRotTh = 0;
      int rotMAI = 0;
      int rotMaxBm = 0;
      int Rotation = 0;

//      int managedForestTmp = 0;
//      double thinningForestTmp = 0;
//      int rotationForestTmp = 0;
//      double thinningForestNewTmp = 0;
//      int managedForestNewTmp = 0;
//      int rotationForestNewTmp = 0;

      double countryHarvestTmp = woodHarvest[countryreg];
      
   	double harvestTmp = harvestGrid.get(xi,yi);              

//cout << "xi= "<<xi<<"\t yi= "<<yi<<"\t MAI= "<< MAI<<"\t mai= "<< maiForest.get(xi,yi)<<"\t NPP= "<<iter->NPP[2000]<<"\t dNPP= "<<data["NPP"].v()<<endl; 

  if (dat_all[asID].ObiomassPrev >0 && iter->CABOVEHA.v() > 0 && maiForest.get(xi,yi) > 0)  
  {
          biomassRotTh = fi.gUt(dat_all[asID].ObiomassPrev, maiForest.get(xi,yi), thinningForest.get(xi,yi));     // rotation time to get current biomass (without thinning)  
          rotMAI = fi.gTopt(maiForest.get(xi,yi), 1);
          rotMaxBm = fi.gTopt(maiForest.get(xi,yi), 2);  
  }else if (dat_all[asID].prevPlantPhytHaBmGr >0)  
  {
          rotMAI = fi.gTopt(maiForest.get(xi,yi), 1);
          rotMaxBm = fi.gTopt(maiForest.get(xi,yi), 2); 
          biomassRotTh = rotMAI; 
  }     
      int rotationForestTmp = rotationForest.get(xi,yi);
      int managedForestTmp = managedForest.get(xi,yi);
      double thinningForestTmp = thinningForest.get(xi,yi);
      int rotationForestNewTmp = rotationForestNew.get(xi,yi);
//      managedForestNewTmp = managedForestNew.get(xi,yi);
      int thinningForestNewTmp = thinningForestNew.get(xi,yi);
//---------------------------------------------------
//           int newForAge = newCohort_all[asID]->getActiveAge();
//           if (newForAge > biomassRotTh)  // New forest age > rotation -> change FM for the new forest
//            {
//              g4m::ageStruct cohortTmpNew = *newCohort_all[asID]; 
                         
             if (managedForest.get(xi,yi) == 2)
                  {
                         managedForest.set(xi,yi,-1);
                         Rotation = rotMaxBm;
                         rotationType.set(xi,yi,1);
                   }else 
                   {     managedForest.set(xi,yi,-2);
                         Rotation = rotMaxBm;
                         rotationType.set(xi,yi,3);
                    }

                rotationForest.set(xi,yi,Rotation);	
                thinningForest.set(xi,yi,-1.);
//                manageChForest.set(xi,yi,1);	
//                cohortTmp.setRotPeriod(Rotation);
//                cohortTmp.setStockingdegree(-1.);
                cohort_all[asID]->setRotPeriod(Rotation);
                cohort_all[asID]->setStockingdegree(-1.);
//                g4m::ageStruct::v resTmp = cohortTmp.aging();
//                sawnW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
//                restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
//                sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
//                restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
        
                
                rotationForestNew.set(xi,yi,Rotation);	
                thinningForestNew.set(xi,yi,-1.);	
//                cohortTmpNew.setRotPeriod(Rotation);
//                cohortTmpNew.setStockingdegree(-1.);        
                newCohort_all[asID]->setRotPeriod(Rotation);
                newCohort_all[asID]->setStockingdegree(-1.);  
//                g4m::ageStruct::v resTmpNew = cohortTmpNew.aging();
//                sawnWnew = resTmpNew.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
//                restWnew = resTmpNew.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
//                sawnThWnew = resTmpNew.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
//                restThWnew = resTmpNew.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
//                newHarvestTmp = ((sawnW + restW + sawnThW + restThW + resUse*resTmp.hv) * (dat_all[asID].OforestShare-dat_all[asID].deforestShare)  +
//                             (sawnWnew + restWnew + sawnThWnew + restThWnew +resUse*resTmpNew.hv) * dat_all[asID].AforestShare) * 
//                              dat_all[asID].LandAreaHa * iter->FTIMBER.v(); // Total current harvested wood in the cell, m3


//           }else // New forest age <= rotation -> don't change FM for the new forest
//           {
//             if (managedForest.get(xi,yi) == 2)
//                  {
//                         managedForest.set(xi,yi,-1);
//                         Rotation = rotMaxBm;
//                         rotationType.set(xi,yi,1);
//                   }else 
//                   {     managedForest.set(xi,yi,-2);
//                         Rotation = rotMaxBm;
//                         rotationType.set(xi,yi,3);
//                    }
//
//                rotationForest.set(xi,yi,Rotation);	
//                thinningForest.set(xi,yi,-1.);
////                manageChForest.set(xi,yi,1);	
//                cohortTmp.setRotPeriod(Rotation);
//                cohortTmp.setStockingdegree(-1.);
//                cohort_all[asID]->setRotPeriod(Rotation);
//                cohort_all[asID]->setStockingdegree(-1.);
//                g4m::ageStruct::v resTmp = cohortTmp.aging();
//                g4m::ageStruct::v resTmp = cohort_all[asID]->aging();                
//                sawnW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
//                restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
//                sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
//                restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
//        
//                 
//                 newHarvestTmp = (sawnW + restW + sawnThW + restThW + resUse*resTmp.hv) * (dat_all[asID].OforestShare-dat_all[asID].deforestShare) * 
//                                   dat_all[asID].LandAreaHa * iter->FTIMBER.v();
//            } // End    else // New forest age < rotation -> don't change FM for the new forest
//              	harvestGrid.set(xi,yi,newHarvestTmp);
//                woodHarvest[countryreg] += (newHarvestTmp-harvestTmp);
//                countryHarvestTmp += (newHarvestTmp-harvestTmp);
                countryHarvestTmp = countryHarvestTmp - harvestTmp+dat_all[asID].deforWoodTotM3;             
                
                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[countryreg] - wprod[countryprice].v(year)) 
                                   ){
                    woodHarvest[countryreg] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,dat_all[asID].deforWoodTotM3);
           	        manageChForest.set(xi,yi,1);
                }else{ // return old values
                rotationForest.set(xi,yi,rotationForestTmp);	
                thinningForest.set(xi,yi,thinningForestTmp);
                managedForest.set(xi,yi,managedForestTmp);	                   
                cohort_all[asID]->setRotPeriod(rotationForestTmp);
                cohort_all[asID]->setStockingdegree(thinningForestTmp);
                rotationForestNew.set(xi,yi,rotationForestNewTmp);	
                thinningForestNew.set(xi,yi,thinningForestNewTmp);
//                managedForestNew.set(xi,yi,managedForestNewTmp)	                   
                newCohort_all[asID]->setRotPeriod(rotationForestNewTmp);
                newCohort_all[asID]->setStockingdegree(thinningForestNewTmp);                 
                }


          } // End  if ((data["POPDENS"].v(1990) == 0) && (data["GDP"].v(1990) == 0))
        } // End  if ((managedForest(xi,yi)>0))
 } // End   else if (woodHarvest[region-1] >= (1.1 * wprod[region].v(year))   



//cout<< managedForest.get(xi,yi)<<endl;


/*  
cout <<"country=\t"<<Country0<<"\t woodHarvest=\t"<< woodHarvest[Country0-1];
cout <<"\t rotation=\t"<< Rotation<<"\t thinning= \t"<<thinningForest.get(xi,yi);
cout <<"\t sawnThW=\t"<<sawnThW*forestArea0*4<<"\t restThW=\t"<<restThW*forestArea0*4<<"\t";
cout <<"\t sawnW= \t"<<sawnW*forestArea0*4<<"\t restW= \t"<<restW*forestArea0*4;
cout << "\t sawnThWha=\t" << sawnThW << "\t restThWha=\t"<<restThW;
cout <<"\t sawnWha= \t"<<sawnW<<"\t restWha= \t"<<restW<<"\t abbiomassO= \t"<< abBiomassO<<"\t abbiomass= \t"<<data["BIOMASS"].v()<<"\t";
cout <<endl;
*/
//cout<< "managedForestSeting....\n";
   } //End for PROTECT == 0
 } // End for some countries 
 } // Test only some regions   
iter++;
  } // End for WHILE (cell loop) 
} // End if year

 
 
  
//cout<<"Start First pass = adjust rotation time "<<endl;  
//----First pass = adjust rotation time -------  
//dataDetStruct::iterator iter = data_all.begin();
iter = data_all.begin();
while (iter != data_all.end()) {
  short int region = iter->POLESREG[2000];
  if (regions.find(region) != regions.end()) { // Test only some regions         
   short int countryreg = iter->COUNTRYREGMIX[2000];
   if (toAdjust.find(countryreg) != toAdjust.end()) {  
   if ((iter->PROTECT[2000]==0)&&((iter->POTVEG[2000]<10 && iter->FOREST[2000]>0 && iter->MAIE[2000]>0)||(iter->POTVEG[2000]<10 && (iter->NPP[2000]>0)||iter->MAIN[2000]>0))){
    int asID = iter->asID;
    short int country = iter->COUNTRY[2000];
    char regionch[2];
    int2str(region,regionch);
    string regprice = "re"+string(regionch)+"price0";
    string regprice0 = "re"+string(regionch)+"price0";
    char countrych[3];
    int2str(countryreg,countrych);
    string countryprice = "re"+string(countrych)+"price0";
    countryreg = iter->COUNTRYREGMIX[2000];     
    int xi = (iter->x);
    int yi = (iter->y);
  double sawnW = 0.;
  double restW = 0.;
  double sawnThW = 0.;
  double restThW = 0.;
  double sawnWnew = 0.;
  double restWnew = 0.;
  double sawnThWnew = 0.;
  double restThWnew = 0.;
//  double harvWoodTmp = 0.;
  int biomassRotTh = 0;
  int rotMAI = 0;
  int rotMaxBmTh = 0;
  int Rotation = 0;
//  double harvestTmp = 0;
  double newHarvestTmp = 0;
  
//  int rotationForestTmp = 0;
//  int rotationForestNewTmp = 0;
  double countryHarvestTmp = woodHarvest[countryreg];  
  
//  if (woodHarvest[countryreg] < 0.97 * wprod[countryprice].v(year))
  if (woodHarvest[countryreg] < 0.99 * wprod[countryprice].v(year))  
  {
    if (managedForest.get(xi,yi)>=2)
     {
          if (dat_all[asID].ObiomassPrev >0 && iter->CABOVEHA.v() > 0 && maiForest.get(xi,yi) > 0)  
          {
//                  biomassRotTh = fi.gUt(dat_all[asID].ObiomassPrev, maiForest.get(xi,yi), thinningForest.get(xi,yi));     // rotation time to get current biomass (with thinning)  
                  rotMAI = fi.gTopt(maiForest.get(xi,yi), 1);
                  rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), 3);         
          }else if (dat_all[asID].prevPlantPhytHaBmGr > 0)
          {
//                  biomassRotTh = fi.gUt(dat_all[asID].prevPlantPhytHaBmGr, maiForest.get(xi,yi), thinningForest.get(xi,yi));     // rotation time to get current biomass (with thinning)  
                  rotMAI = fi.gTopt(maiForest.get(xi,yi), 1);
                  rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), 3);   
          }                       
             

         int rotationForestTmp = rotationForest.get(xi,yi);
         int rotationForestNewTmp = rotationForestNew.get(xi,yi);
        g4m::ageStruct cohortTmpNew = *newCohort_all[asID];                                     
        g4m::ageStruct cohortTmp = *cohort_all[asID];
        int newForAge = newCohort_all[asID]->getActiveAge();         
    	double harvestTmp = harvestGrid.get(xi,yi);
       if (rotMAI < rotationForest.get(xi,yi)) // TEST for AT
       {
//          Rotation = rotationForest.get(xi,yi) - 10;
          Rotation = rotationForest.get(xi,yi) - 5;          
          if (Rotation < rotMAI) {Rotation = rotMAI;}

        rotationForest.set(xi,yi,Rotation);		   
        cohortTmp.setRotPeriod(Rotation);
        cohort_all[asID]->setRotPeriod(Rotation);          
       }else  if (rotMAI > rotationForest.get(xi,yi))
//       {Rotation = rotationForest.get(xi,yi) + 10;
       {Rotation = rotationForest.get(xi,yi) + 5;
        if (Rotation > rotMAI) {Rotation = rotMAI;}

        rotationForest.set(xi,yi,Rotation);		   
        cohortTmp.setRotPeriod(Rotation);
        cohort_all[asID]->setRotPeriod(Rotation);
        }
           if ((newForAge > biomassRotTh) && (rotMAI < biomassRotTh)) {
//             Rotation = rotationForestNew.get(xi,yi) - 10;
             Rotation = rotationForestNew.get(xi,yi) - 5;
             if (Rotation < rotMAI) {Rotation = rotMAI;}

             cohortTmpNew.setRotPeriod(Rotation);
             newCohort_all[asID]->setRotPeriod(Rotation);    
             rotationForestNew.set(xi,yi,Rotation);
             }
         g4m::ageStruct::v resTmp = cohortTmp.aging();
         sawnW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
         restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
         sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
         restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.

         g4m::ageStruct::v resTmpNew = cohortTmpNew.aging();
         sawnWnew = resTmpNew.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
         restWnew = resTmpNew.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
         sawnThWnew = resTmpNew.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
         restThWnew = resTmpNew.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
         newHarvestTmp = ((sawnW + restW + sawnThW + restThW + resUse*resTmp.hv) * (dat_all[asID].OforestShare-dat_all[asID].deforestShare)  +
                     (sawnWnew + restWnew + sawnThWnew + restThWnew +resUse*resTmpNew.hv) * dat_all[asID].AforestShare) * 
                      dat_all[asID].LandAreaHa * iter->FTIMBER.v() + dat_all[asID].deforWoodTotM3; // Total current harvested wood in the cell, m3           }

                countryHarvestTmp += (newHarvestTmp-harvestTmp);
                

                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[countryreg] - wprod[countryprice].v(year)) 
                                   ){
//cout<<"woodHarvest=\t"<<woodHarvest[countryreg]<<"\tcountryHarvestTmp=\t"<<countryHarvestTmp<<endl;
                    woodHarvest[countryreg] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,newHarvestTmp);
                }else{ // return old values
                rotationForest.set(xi,yi,rotationForestTmp);	
                cohort_all[asID]->setRotPeriod(rotationForestTmp);
                rotationForestNew.set(xi,yi,rotationForestNewTmp);	
                newCohort_all[asID]->setRotPeriod(rotationForestNewTmp);
                }  
//        }

       }  // end for if (managedForest(xi,yi)>=2)          
    
    } //end for if (woodHarvest[region] <= 0.9 * wprod[region].v(year))
//    else if (woodHarvest[countryreg] > 1.03 * wprod[countryprice].v(year)) 
    else if (woodHarvest[countryreg] > 1.01 * wprod[countryprice].v(year))     
    {
     if ((managedForest.get(xi,yi)>0) && (managedForest.get(xi,yi)<=2))
     {
          if (dat_all[asID].ObiomassPrev >0 && iter->CABOVEHA.v() > 0 && maiForest.get(xi,yi) > 0)  
          {
//                  biomassRotTh = fi.gUt(dat_all[asID].ObiomassPrev, maiForest.get(xi,yi), thinningForest.get(xi,yi));     // rotation time to get current biomass (with thinning)  
                  rotMAI = fi.gTopt(maiForest.get(xi,yi), 1);
                  rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), 3);         
          }else if (dat_all[asID].prevPlantPhytHaBmGr > 0)
          {
//                  biomassRotTh = fi.gUt(dat_all[asID].prevPlantPhytHaBmGr, maiForest.get(xi,yi), thinningForest.get(xi,yi));     // rotation time to get current biomass (with thinning)  
                  rotMAI = fi.gTopt(maiForest.get(xi,yi), 1);
                  rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), 3);   
          }                       
         int rotationForestTmp = rotationForest.get(xi,yi);
         int rotationForestNewTmp = rotationForestNew.get(xi,yi);
                                               
       if (rotMaxBmTh > rotationForest.get(xi,yi)) //TEST for AT
       {
          g4m::ageStruct cohortTmp = *cohort_all[asID];
          g4m::ageStruct cohortTmpNew = *newCohort_all[asID]; 
          int newForAge = newCohort_all[asID]->getActiveAge();  

     	  double harvestTmp = harvestGrid.get(xi,yi);


//        Rotation = rotationForest.get(xi,yi) + 10;
        Rotation = rotationForest.get(xi,yi) + 5;
        if (Rotation > rotMaxBmTh) {Rotation = rotMaxBmTh;}
        rotationForest.set(xi,yi,Rotation);		   
        cohortTmp.setRotPeriod(Rotation);
        cohort_all[asID]->setRotPeriod(Rotation);       
             if ((newForAge > biomassRotTh) && (rotMaxBmTh > biomassRotTh)) {
//             Rotation = rotationForestNew.get(xi,yi) + 10;
             Rotation = rotationForestNew.get(xi,yi) + 5;
             if (Rotation > rotMaxBmTh) {Rotation = rotMaxBmTh;}                          
             cohortTmpNew.setRotPeriod(Rotation);
             newCohort_all[asID]->setRotPeriod(Rotation);    
             rotationForestNew.set(xi,yi,Rotation);
           }

        g4m::ageStruct::v resTmp = cohortTmp.aging();
        sawnW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
        restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
        sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
        restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.

        g4m::ageStruct::v resTmpNew = cohortTmpNew.aging();
        sawnWnew = resTmpNew.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
        restWnew = resTmpNew.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
        sawnThWnew = resTmpNew.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
        restThWnew = resTmpNew.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
        newHarvestTmp = ((sawnW + restW + sawnThW + restThW + resUse*resTmp.hv) * (dat_all[asID].OforestShare-dat_all[asID].deforestShare)  +
                         (sawnWnew + restWnew + sawnThWnew + restThWnew +resUse*resTmpNew.hv) * dat_all[asID].AforestShare) * 
                          dat_all[asID].LandAreaHa * iter->FTIMBER.v() + dat_all[asID].deforWoodTotM3; // Total current harvested wood in the cell, m3

                countryHarvestTmp += (newHarvestTmp-harvestTmp);
                
                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[countryreg] - wprod[countryprice].v(year)) 
                                   ){
                    woodHarvest[countryreg] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,newHarvestTmp);
                }else{ // return old values
                rotationForest.set(xi,yi,rotationForestTmp);	
                cohort_all[asID]->setRotPeriod(rotationForestTmp);
                rotationForestNew.set(xi,yi,rotationForestNewTmp);	
                newCohort_all[asID]->setRotPeriod(rotationForestNewTmp);
                }  
        }
        
       }  // end for if (managedForest(xi,yi)<=2)       
      }   //end for esle if (woodHarvest[region] >= 1.1 * wprod[region].v(year))
  } //End Protect
 }  // End some countries
}  // Test only some regions 
  iter++;
} // End While

// ----- End of First pass


//----Second pass = adjust rotation time -------  
//dataDetStruct::iterator iter = data_all.begin();
iter = data_all.begin();
while (iter != data_all.end()) {
  short int region = iter->POLESREG[2000];
  if (regions.find(region) != regions.end()) { // Test only some regions         
     short int countryreg = iter->COUNTRYREGMIX[2000];
     if (toAdjust.find(countryreg) != toAdjust.end()) {  
       if ((iter->PROTECT[2000]==0)&&((iter->POTVEG[2000]<10 && iter->FOREST[2000]>0 && iter->MAIE[2000]>0)||(iter->POTVEG[2000]<10 && (iter->NPP[2000]>0)||iter->MAIN[2000]>0))){
        int asID = iter->asID;
        short int country = iter->COUNTRY[2000];
        char regionch[2];
    int2str(region,regionch);
    string regprice = "re"+string(regionch)+"price0";
    string regprice0 = "re"+string(regionch)+"price0";
    char countrych[3];
    int2str(countryreg,countrych);
    string countryprice = "re"+string(countrych)+"price0";
    countryreg = iter->COUNTRYREGMIX[2000];     
    int xi = (iter->x);
    int yi = (iter->y);
  double sawnW = 0.;
  double restW = 0.;
  double sawnThW = 0.;
  double restThW = 0.;
  double sawnWnew = 0.;
  double restWnew = 0.;
  double sawnThWnew = 0.;
  double restThWnew = 0.;
  int biomassRotTh = 0;
  int rotMAI = 0;
  int rotMaxBmTh = 0;
  int Rotation = 0;
//  double harvestTmp = 0;
  double newHarvestTmp = 0;
  
//  int rotationForestTmp = 0;
//  int rotationForestNewTmp = 0;
  double countryHarvestTmp = woodHarvest[countryreg];  
  
//  if (woodHarvest[countryreg] < 0.9 * wprod[countryprice].v(year))
//  if (woodHarvest[countryreg] < 0.95 * wprod[countryprice].v(year))
  if (woodHarvest[countryreg] < 0.98 * wprod[countryprice].v(year))  
  {
    if (managedForest.get(xi,yi)>0)
     {
          if (dat_all[asID].ObiomassPrev >0 && iter->CABOVEHA.v() > 0 && maiForest.get(xi,yi) > 0)  
          {
//                  biomassRotTh = fi.gUt(dat_all[asID].ObiomassPrev, maiForest.get(xi,yi), thinningForest.get(xi,yi));     // rotation time to get current biomass (with thinning)  
                  rotMAI = fi.gTopt(maiForest.get(xi,yi), 1);
                  rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), 3);         
          }else if (dat_all[asID].prevPlantPhytHaBmGr > 0)
          {
//                  biomassRotTh = fi.gUt(dat_all[asID].prevPlantPhytHaBmGr, maiForest.get(xi,yi), thinningForest.get(xi,yi));     // rotation time to get current biomass (with thinning)  
                  rotMAI = fi.gTopt(maiForest.get(xi,yi), 1);
                  rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), 3);   
          }                       

         int rotationForestTmp = rotationForest.get(xi,yi);
         int rotationForestNewTmp = rotationForestNew.get(xi,yi);
          g4m::ageStruct cohortTmpNew = *newCohort_all[asID];                                    
          g4m::ageStruct cohortTmp = *cohort_all[asID];
        int newForAge = newCohort_all[asID]->getActiveAge();         
    	double harvestTmp = harvestGrid.get(xi,yi);
       if (rotMAI < rotationForest.get(xi,yi)) // TEST for AT
       {
//          Rotation = rotationForest.get(xi,yi) - 10;
          Rotation = rotationForest.get(xi,yi) - 5;          
          if (Rotation < rotMAI) {Rotation = rotMAI;}

        rotationForest.set(xi,yi,Rotation);		   
        cohortTmp.setRotPeriod(Rotation);
        cohort_all[asID]->setRotPeriod(Rotation);
       }else if (rotMAI > rotationForest.get(xi,yi))
//       {Rotation = rotationForest.get(xi,yi) + 10;
       {Rotation = rotationForest.get(xi,yi) + 5;
        if (Rotation > rotMAI) {Rotation = rotMAI;}
        
        rotationForest.set(xi,yi,Rotation);		   
        cohortTmp.setRotPeriod(Rotation);
        cohort_all[asID]->setRotPeriod(Rotation);        
        }
        
           if ((newForAge > biomassRotTh) && (rotMAI < biomassRotTh)) {
//             Rotation = rotationForestNew.get(xi,yi) - 10;
             Rotation = rotationForestNew.get(xi,yi) - 5;
             if (Rotation < rotMAI) {Rotation = rotMAI;}
             cohortTmpNew.setRotPeriod(Rotation);
             newCohort_all[asID]->setRotPeriod(Rotation);    
             rotationForestNew.set(xi,yi,Rotation);
             }
        g4m::ageStruct::v resTmp = cohortTmp.aging();
        sawnW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
        restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
        sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
        restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.

        g4m::ageStruct::v resTmpNew = cohortTmpNew.aging();
        sawnWnew = resTmpNew.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
        restWnew = resTmpNew.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
        sawnThWnew = resTmpNew.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
        restThWnew = resTmpNew.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
        newHarvestTmp = ((sawnW + restW + sawnThW + restThW + resUse*resTmp.hv) * (dat_all[asID].OforestShare-dat_all[asID].deforestShare)  +
                         (sawnWnew + restWnew + sawnThWnew + restThWnew +resUse*resTmpNew.hv) * dat_all[asID].AforestShare) * 
                          dat_all[asID].LandAreaHa * iter->FTIMBER.v() + dat_all[asID].deforWoodTotM3; // Total current harvested wood in the cell, m3

                countryHarvestTmp += (newHarvestTmp-harvestTmp);
                
                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[countryreg] - wprod[countryprice].v(year)) 
                                   ){
                    woodHarvest[countryreg] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,newHarvestTmp);
                }else{ // return old values
                rotationForest.set(xi,yi,rotationForestTmp);	
                cohort_all[asID]->setRotPeriod(rotationForestTmp);
                rotationForestNew.set(xi,yi,rotationForestNewTmp);	
                newCohort_all[asID]->setRotPeriod(rotationForestNewTmp);
                }  
//        }

       }  // end for if (managedForest(xi,yi)>=2)          
    
    } //end for if (woodHarvest[region] <= 0.9 * wprod[region].v(year))
//    else if (woodHarvest[countryreg] > 1.1 * wprod[countryprice].v(year)) 
//    else if (woodHarvest[countryreg] > 1.05 * wprod[countryprice].v(year)) 
//    else if (woodHarvest[countryreg] > 1.03 * wprod[countryprice].v(year)) 
    else if (woodHarvest[countryreg] > 1.02 * wprod[countryprice].v(year)) 
    {
//     if (managedForest.get(xi,yi)<=2)
     if (managedForest.get(xi,yi)>0)
     {
          if (dat_all[asID].ObiomassPrev >0 && iter->CABOVEHA.v() > 0 && maiForest.get(xi,yi) > 0)  
          {
//                  biomassRotTh = fi.gUt(dat_all[asID].ObiomassPrev, maiForest.get(xi,yi), thinningForest.get(xi,yi));     // rotation time to get current biomass (with thinning)  
                  rotMAI = fi.gTopt(maiForest.get(xi,yi), 1);
                  rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), 3);         
          }else if (dat_all[asID].prevPlantPhytHaBmGr > 0)
          {
//                  biomassRotTh = fi.gUt(dat_all[asID].prevPlantPhytHaBmGr, maiForest.get(xi,yi), thinningForest.get(xi,yi));     // rotation time to get current biomass (with thinning)  
                  rotMAI = fi.gTopt(maiForest.get(xi,yi), 1);
                  rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), 3);   
          }                       

         int rotationForestTmp = rotationForest.get(xi,yi);
         int rotationForestNewTmp = rotationForestNew.get(xi,yi);

          g4m::ageStruct cohortTmp = *cohort_all[asID];
          g4m::ageStruct cohortTmpNew = *newCohort_all[asID]; 
          int newForAge = newCohort_all[asID]->getActiveAge();  

    	double harvestTmp = harvestGrid.get(xi,yi);
       if (rotMaxBmTh > rotationForest.get(xi,yi)) //TEST for AT
       {
//        Rotation = rotationForest.get(xi,yi) + 10;
        Rotation = rotationForest.get(xi,yi) + 5;
        if (Rotation > rotMaxBmTh) {Rotation = rotMaxBmTh;}
        rotationForest.set(xi,yi,Rotation);		   
        cohortTmp.setRotPeriod(Rotation);
        cohort_all[asID]->setRotPeriod(Rotation);       

             if ((newForAge > biomassRotTh) && (rotMaxBmTh > biomassRotTh)) {
//             Rotation = rotationForestNew.get(xi,yi) + 10;
             Rotation = rotationForestNew.get(xi,yi) + 5;
             if (Rotation > rotMaxBmTh) {Rotation = rotMaxBmTh;}                          
             cohortTmpNew.setRotPeriod(Rotation);
             newCohort_all[asID]->setRotPeriod(Rotation);    
             rotationForestNew.set(xi,yi,Rotation);
             }
        g4m::ageStruct::v resTmp = cohortTmp.aging();
        sawnW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
        restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
        sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
        restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.

        g4m::ageStruct::v resTmpNew = cohortTmpNew.aging();
        sawnWnew = resTmpNew.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
        restWnew = resTmpNew.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
        sawnThWnew = resTmpNew.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
        restThWnew = resTmpNew.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
        newHarvestTmp = ((sawnW + restW + sawnThW + restThW + resUse*resTmp.hv) * (dat_all[asID].OforestShare-dat_all[asID].deforestShare)  +
                         (sawnWnew + restWnew + sawnThWnew + restThWnew +resUse*resTmpNew.hv) * dat_all[asID].AforestShare) * 
                          dat_all[asID].LandAreaHa * iter->FTIMBER.v() + dat_all[asID].deforWoodTotM3; // Total current harvested wood in the cell, m3

                countryHarvestTmp += (newHarvestTmp-harvestTmp);
                
                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[countryreg] - wprod[countryprice].v(year)) 
                                   ){
                    woodHarvest[countryreg] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,newHarvestTmp);
                }else{ // return old values
                rotationForest.set(xi,yi,rotationForestTmp);	
                cohort_all[asID]->setRotPeriod(rotationForestTmp);
                rotationForestNew.set(xi,yi,rotationForestNewTmp);	
                newCohort_all[asID]->setRotPeriod(rotationForestNewTmp);
                }  
        }
        
       }  // end for if (managedForest(xi,yi)<=2)       
      }   //end for esle if (woodHarvest[region] >= 1.1 * wprod[region].v(year))
  } //End Protect
 }  // End some countries
}  // Test only some regions 
  iter++;
} // End While
// ----- End of Second pass






////------------------------------------------------------------------------------
//// 
//// -------Third pass: Adjust thinning -----------------------------------------------
////
////------------------------------------------------------------------------------
//cout<<"Start Third pass = Adjust thinning"<< endl;
 iter = data_all.begin();
 while (iter != data_all.end())
 {
  short int region = iter->POLESREG[2000];
  if (regions.find(region) != regions.end()) { // Test only some regions          
    short int countryreg = iter->COUNTRYREGMIX[2000];
    if (toAdjust.find(countryreg) != toAdjust.end()) {  
	if ((iter->PROTECT[2000]==0)&&((iter->POTVEG[2000]<10 && iter->FOREST[2000]>0 && iter->MAIE[2000]>0)||(iter->POTVEG[2000]<10 && (iter->NPP[2000]>0)||iter->MAIN[2000]>0))){
    int xi = (iter->x);
    int yi = (iter->y);
   if (maiForest.get(xi,yi)>0)
   { 
    int asID = iter->asID;
    short int country = iter->COUNTRY[2000];                            
    char regionch[2];
    int2str(region,regionch);
    string regprice = "re"+string(regionch)+"price0";
    string regprice0 = "re"+string(regionch)+"price0";
    char countrych[3];
    int2str(countryreg,countrych);
    string countryprice = "re"+string(countrych)+"price0";
    countryreg = iter->COUNTRYREGMIX[2000];     
//  if (floor(woodHarvest[countryreg]) < 0.95 * wprod[countryprice].v(year))
//  if (floor(woodHarvest[countryreg]) < 0.94 * wprod[countryprice].v(year))  
  if (floor(woodHarvest[countryreg]) < 0.97 * wprod[countryprice].v(year))  
   {
    if ((managedForest.get(xi,yi)<=0))
     {
      double sawnW = 0.;
      double restW = 0.;
      double sawnThW = 0.;
      double restThW = 0.;
      double sawnWnew = 0.;
      double restWnew = 0.;
      double sawnThWnew = 0.;
      double restThWnew = 0.;
//      double harvWoodTmp = 0.;
//      double harvestTmp = 0.;
      double newHarvestTmp = 0.;
      int biomassRot = 0;
      int biomassRotTh2 = 0;
      int rotMAI = 0;
      int rotMaxBmTh = 0;
      int Rotation = 0;

//      int managedForestTmp = 0;
//      double thinningForestTmp = 0;
//      int rotationForestTmp = 0;
//      double thinningForestNewTmp = 0;
//      int managedForestNewTmp = 0;
//      int rotationForestNewTmp = 0;

      double countryHarvestTmp = woodHarvest[countryreg]; 
      
      double stockingDegree = 1.5;     
      g4m::ageStruct cohortTmpNew = *newCohort_all[asID]; 
      g4m::ageStruct cohortTmp = *cohort_all[asID];
   	  double harvestTmp = harvestGrid.get(xi,yi);              

  if (dat_all[asID].ObiomassPrev >0 && iter->CABOVEHA.v() > 0 && maiForest.get(xi,yi) > 0)  
  {
//          biomassRot = fi.gU(dat_all[asID].ObiomassPrev, maiForest.get(xi,yi), 1);     // rotation time to get current biomass (with thinning)  
          biomassRotTh2 = fi.gUt(iter->CABOVEHA.v(), maiForest.get(xi,yi), stockingDegree);     // rotation time to get current biomass (with thinning)            
          rotMAI = fi.gTopt(maiForest.get(xi,yi), 1);
          rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), 3);         
  }else if (dat_all[asID].prevPlantPhytHaBmGr >0)  
  {
//          biomassRot = fi.gU(dat_all[asID].ObiomassPrev, maiForest.get(xi,yi), 1);     // rotation time to get current biomass (with thinning)  

          rotMAI = fi.gTopt(maiForest.get(xi,yi), 1);
          rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), 3);         
          biomassRotTh2 = rotMaxBmTh;
  }     

      int rotationForestTmp = rotationForest.get(xi,yi);
      int managedForestTmp = managedForest.get(xi,yi);
      double thinningForestTmp = thinningForest.get(xi,yi);
      int rotationForestNewTmp = rotationForestNew.get(xi,yi);
//      managedForestNewTmp = managedForestNew.get(xi,yi);
      double thinningForestNewTmp = thinningForestNew.get(xi,yi);

//---------------------------------------------------
           int newForAge = newCohort_all[asID]->getActiveAge();  
           if (newForAge > biomassRot)  // New forest age > rotation -> change FM for the new forest
            {
             if (managedForest.get(xi,yi) == 0)
             {
                         managedForest.set(xi,yi,3);
                         Rotation = biomassRotTh2+1;
                         if (Rotation < rotMAI) {Rotation = rotMAI;}
                         rotationType.set(xi,yi,1);
             }else 
             {           managedForest.set(xi,yi,2);
                         Rotation = biomassRotTh2+1;
                         if (Rotation < rotMAI) {Rotation = rotMAI;}
//                         if (Rotation > rotMaxBmTh) {Rotation = rotMaxBmTh;}
                         rotationType.set(xi,yi,3);
              }

                rotationForest.set(xi,yi,Rotation);	
                thinningForest.set(xi,yi,stockingDegree);	
                cohortTmp.setRotPeriod(Rotation);
                cohortTmp.setStockingdegree(stockingDegree);
                cohort_all[asID]->setRotPeriod(Rotation);
                cohort_all[asID]->setStockingdegree(stockingDegree);
                
                rotationForestNew.set(xi,yi,Rotation);	
                thinningForestNew.set(xi,yi,stockingDegree);	
                cohortTmpNew.setRotPeriod(Rotation);
                cohortTmpNew.setStockingdegree(stockingDegree);        
                newCohort_all[asID]->setRotPeriod(Rotation);
                newCohort_all[asID]->setStockingdegree(stockingDegree);  

             }else{
                  if (managedForest.get(xi,yi) == 0)
                  {
                         managedForest.set(xi,yi,3);
                         Rotation = biomassRotTh2+1;
                         if (Rotation < rotMAI) {Rotation = rotMAI;}
                         rotationType.set(xi,yi,1);
                   }else 
                   {     managedForest.set(xi,yi,2);
                         Rotation = biomassRotTh2+1;
                         if (Rotation < rotMAI) {Rotation = rotMAI;}                         
//                         if (Rotation > rotMaxBmTh) {Rotation = rotMaxBmTh;}
                         rotationType.set(xi,yi,3);
                    }

                rotationForest.set(xi,yi,Rotation);	
                thinningForest.set(xi,yi,stockingDegree);	
                cohortTmp.setRotPeriod(Rotation);
                cohortTmp.setStockingdegree(stockingDegree);
                cohort_all[asID]->setRotPeriod(Rotation);
                cohort_all[asID]->setStockingdegree(stockingDegree);
             } // End    else // New forest age < rotation -> don't change FM for the new forest
                g4m::ageStruct::v resTmp = cohortTmp.aging();
                sawnW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
                restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
                sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
                restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.

                g4m::ageStruct::v resTmpNew = cohortTmpNew.aging();
                sawnWnew = resTmpNew.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
                restWnew = resTmpNew.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
                sawnThWnew = resTmpNew.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
                restThWnew = resTmpNew.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
                newHarvestTmp = ((sawnW + restW + sawnThW + restThW + resUse*resTmp.hv) * (dat_all[asID].OforestShare-dat_all[asID].deforestShare)  +
                             (sawnWnew + restWnew + sawnThWnew + restThWnew +resUse*resTmpNew.hv) * dat_all[asID].AforestShare) * 
                              dat_all[asID].LandAreaHa * iter->FTIMBER.v() + dat_all[asID].deforWoodTotM3; // Total current harvested wood in the cell, m3
                
                 
                countryHarvestTmp += (newHarvestTmp-harvestTmp);
                
                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[countryreg] - wprod[countryprice].v(year)) 
                                   ){
                    woodHarvest[countryreg] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,newHarvestTmp);
           	        manageChForest.set(xi,yi,1);
                }else{ // return old values
                rotationForest.set(xi,yi,rotationForestTmp);	
                thinningForest.set(xi,yi,thinningForestTmp);
                managedForest.set(xi,yi,managedForestTmp);	                   
                cohort_all[asID]->setRotPeriod(rotationForestTmp);
                cohort_all[asID]->setStockingdegree(thinningForestTmp);
                rotationForestNew.set(xi,yi,rotationForestNewTmp);	
                thinningForestNew.set(xi,yi,thinningForestNewTmp);
//                managedForestNew.set(xi,yi,managedForestNewTmp)	                   
                newCohort_all[asID]->setRotPeriod(rotationForestNewTmp);
                newCohort_all[asID]->setStockingdegree(thinningForestNewTmp);                 
                }
        } // End  if ((managedForest(xi,yi)<=0) & (managedForest(xi,yi)>-2)
   
// }else if (floor(woodHarvest[countryreg]) > 1.05 * wprod[countryprice].v(year)) 
// }else if (floor(woodHarvest[countryreg]) > 1.2 * wprod[countryprice].v(year))  
// }else if (floor(woodHarvest[countryreg]) > 1.1 * wprod[countryprice].v(year))  
// }else if (floor(woodHarvest[countryreg]) > 1.06 * wprod[countryprice].v(year))  
// }else if (floor(woodHarvest[countryreg]) > 1.04 * wprod[countryprice].v(year))   
 }else if (floor(woodHarvest[countryreg]) > 1.03 * wprod[countryprice].v(year))   
 {
    if ((managedForest.get(xi,yi)>0) && (managedForest.get(xi,yi)<3))
     {
      double sawnW = 0.;
      double restW = 0.;
      double sawnThW = 0.;
      double restThW = 0.;
      double sawnWnew = 0.;
      double restWnew = 0.;
      double sawnThWnew = 0.;
      double restThWnew = 0.;
//      double harvestTmp = 0.;      
      double newHarvestTmp = 0;
      int biomassRot = 0;
      int rotMAI = 0;
      int rotMaxBm = 0;
      int Rotation = 0;
      
//      int managedForestTmp = 0;
//      double thinningForestTmp = 0;
//      int rotationForestTmp = 0;
//      double thinningForestNewTmp = 0;
//      int managedForestNewTmp = 0;
//      int rotationForestNewTmp = 0;

      double countryHarvestTmp = woodHarvest[countryreg];
      
//      g4m::ageStruct cohortTmp = *cohort_all[asID];
   	  double harvestTmp = harvestGrid.get(xi,yi);              
 
  if (dat_all[asID].ObiomassPrev >0 && iter->CABOVEHA.v() > 0 && maiForest.get(xi,yi) > 0)  
  {
//          biomassRotTh = fi.gUt(dat_all[asID].ObiomassPrev, maiForest.get(xi,yi), thinningForest.get(xi,yi));     // rotation time to get current biomass (without thinning)  
//          biomassRot = fi.gU(dat_all[asID].ObiomassPrev, maiForest.get(xi,yi), 1);     // rotation time to get current biomass (without thinning)  
          rotMaxBm = fi.gTopt(maiForest.get(xi,yi), 2);  
  }else if (dat_all[asID].prevPlantPhytHaBmGr >0)  
  {
//          biomassRotTh = fi.gUt(dat_all[asID].ObiomassPrev, maiForest.get(xi,yi), thinningForest.get(xi,yi));     // rotation time to get current biomass (without thinning)  
//          biomassRot = fi.gU(dat_all[asID].ObiomassPrev, maiForest.get(xi,yi), 1);     // rotation time to get current biomass (without thinning)  
          rotMaxBm = fi.gTopt(maiForest.get(xi,yi), 2);  
  }     

      int rotationForestTmp = rotationForest.get(xi,yi);
      int managedForestTmp = managedForest.get(xi,yi);
      double thinningForestTmp = thinningForest.get(xi,yi);
      int rotationForestNewTmp = rotationForestNew.get(xi,yi);
//      managedForestNewTmp = managedForestNew.get(xi,yi);
      double thinningForestNewTmp = thinningForestNew.get(xi,yi);

//---------------------------------------------------
//           int newForAge = newCohort_all[asID]->getActiveAge();  
//cout<<"newForAge= "<<newForAge<<endl;
//           if (newForAge > biomassRot)  // New forest age > rotation -> change FM for the new forest
//            {
//cout<<"biomassRot = "<< biomassRot<<endl;
//              g4m::ageStruct cohortTmpNew = *newCohort_all[asID]; 
             if (managedForest.get(xi,yi) == 2)
                  {
// cout<<"if (managedForest.get(xi,yi) == 2)"<<endl;
                         managedForest.set(xi,yi,-1);
//                         Rotation = biomassRot+1;
                          if (rotationForestTmp < rotMaxBm) {
                              Rotation = rotMaxBm;
                            }else{     
                              Rotation = rotationForestTmp;  }
//                         if (Rotation > rotMaxBm) {Rotation = rotMaxBm;}
                         rotationType.set(xi,yi,1);
                   }else 
                   {     managedForest.set(xi,yi,-2);
//                         Rotation = biomassRot+1;
                          if (rotationForestTmp < rotMaxBm) {
                              Rotation = rotMaxBm;
                            }else{     
                              Rotation = rotationForestTmp;  }
//                         if (Rotation > rotMaxBm) {Rotation = rotMaxBm;}
                         rotationType.set(xi,yi,3);
//cout<<"if (managedForest.get(xi,yi) == 2) ELSE"<<endl;
                    }

                rotationForest.set(xi,yi,Rotation);	
                thinningForest.set(xi,yi,-1.);
//                cohortTmp.setRotPeriod(Rotation);
//                cohortTmp.setStockingdegree(-1.);
                cohort_all[asID]->setRotPeriod(Rotation);
                cohort_all[asID]->setStockingdegree(-1.);

//                g4m::ageStruct::v resTmp = cohortTmp.aging();
//                sawnW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
//                restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
//                sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
//                restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.

                rotationForestNew.set(xi,yi,Rotation);	
                thinningForestNew.set(xi,yi,-1.);	
//                cohortTmpNew.setRotPeriod(Rotation);
//                cohortTmpNew.setStockingdegree(-1.);        
                newCohort_all[asID]->setRotPeriod(Rotation);
                newCohort_all[asID]->setStockingdegree(-1.);  
//                g4m::ageStruct::v resTmpNew = cohortTmpNew.aging();
//                sawnWnew = resTmpNew.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
//                restWnew = resTmpNew.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
//                sawnThWnew = resTmpNew.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
//                restThWnew = resTmpNew.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
//
//                newHarvestTmp = ((sawnW + restW + sawnThW + restThW + resUse*resTmp.hv) * (dat_all[asID].OforestShare-dat_all[asID].deforestShare)  +
//                             (sawnWnew + restWnew + sawnThWnew + restThWnew +resUse*resTmpNew.hv) * dat_all[asID].AforestShare) * 
//                              dat_all[asID].LandAreaHa * iter->FTIMBER.v() + dat_all[asID].deforWoodTotM3; // Total current harvested wood in the cell, m3

//           }else // New forest age <= rotation -> don't change FM for the new forest
//           {
//             if (managedForest.get(xi,yi) == 2)
//                  {
//                         managedForest.set(xi,yi,-1);
////                         Rotation = rotationForest.get(xi,yi) + 10;
////                         if (Rotation > rotMaxBm) {Rotation = rotMaxBm;}
////                         Rotation = biomassRot+1;
//                          if (rotationForestTmp < rotMaxBm) {
//                              Rotation = rotMaxBm;
//                            }else{     
//                              Rotation = rotationForestTmp;  }
//                         rotationType.set(xi,yi,1);
//                   }else 
//                   {     managedForest.set(xi,yi,-2);
////                         Rotation = rotationForest.get(xi,yi) + 10;
////                         if (Rotation > rotMaxBm) {Rotation = rotMaxBm;}
////                         Rotation = biomassRot+1;
//                          if (rotationForestTmp < rotMaxBm) {
//                              Rotation = rotMaxBm;
//                            }else{     
//                              Rotation = rotationForestTmp;  }
//                         rotationType.set(xi,yi,3);
//                    }
//                rotationForest.set(xi,yi,Rotation);	
//                thinningForest.set(xi,yi,-1.);	
//                cohortTmp.setRotPeriod(Rotation);
//                cohortTmp.setStockingdegree(-1.);
//                cohort_all[asID]->setRotPeriod(Rotation);
//                cohort_all[asID]->setStockingdegree(-1.);
////                g4m::ageStruct::v resTmp = cohortTmp.aging();
////                sawnW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
////                restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
////                sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
////                restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
////                 
////                 newHarvestTmp = (sawnW + restW + sawnThW + restThW + resUse*resTmp.hv) * (dat_all[asID].OforestShare-dat_all[asID].deforestShare) * 
////                                   dat_all[asID].LandAreaHa * iter->FTIMBER.v() + dat_all[asID].deforWoodTotM3;
//            } // End    else // New forest age < rotation -> don't change FM for the new forest

//                countryHarvestTmp += (newHarvestTmp-harvestTmp);
                countryHarvestTmp = countryHarvestTmp-harvestTmp+dat_all[asID].deforWoodTotM3;                
                
                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[countryreg] - wprod[countryprice].v(year)) 
                                   ){
                    woodHarvest[countryreg] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,dat_all[asID].deforWoodTotM3);
           	        manageChForest.set(xi,yi,1);
                }else{ // return old values
                rotationForest.set(xi,yi,rotationForestTmp);	
                thinningForest.set(xi,yi,thinningForestTmp);
                managedForest.set(xi,yi,managedForestTmp);	                   
                cohort_all[asID]->setRotPeriod(rotationForestTmp);
                cohort_all[asID]->setStockingdegree(thinningForestTmp);
                rotationForestNew.set(xi,yi,rotationForestNewTmp);	
                thinningForestNew.set(xi,yi,thinningForestNewTmp);
//                managedForestNew.set(xi,yi,managedForestNewTmp)	                   
                newCohort_all[asID]->setRotPeriod(rotationForestNewTmp);
                newCohort_all[asID]->setStockingdegree(thinningForestNewTmp);                 
                }
//         } // End if ((cohort_all[asID]->getArea(0) == 0.)||(cohort_all[asID]->getArea(0) >= 1/400.)){
        } // End  if ((managedForest(xi,yi)>0) & (managedForest(xi,yi)<3)
 } // End   else if (woodHarvest[region-1] >= (1.1 * wprod[region].v(year))   


//cout<< managedForest.get(xi,yi)<<endl;


/*  
cout <<"country=\t"<<Country0<<"\t woodHarvest=\t"<< woodHarvest[Country0-1];
cout <<"\t rotation=\t"<< Rotation<<"\t thinning= \t"<<thinningForest.get(xi,yi);
cout <<"\t sawnThW=\t"<<sawnThW*forestArea0*4<<"\t restThW=\t"<<restThW*forestArea0*4<<"\t";
cout <<"\t sawnW= \t"<<sawnW*forestArea0*4<<"\t restW= \t"<<restW*forestArea0*4;
cout << "\t sawnThWha=\t" << sawnThW << "\t restThWha=\t"<<restThW;
cout <<"\t sawnWha= \t"<<sawnW<<"\t restWha= \t"<<restW<<"\t abbiomassO= \t"<< abBiomassO<<"\t abbiomass= \t"<<iter->"BIOMASS"].v()<<"\t";
cout <<endl;
*/
//cout<< "managedForestSeting....\n";
   } //End for mai>0
  } //End for PROTECT == 0
 }  // End some countries
 } // Test only some regions   
iter++;
  } // End for WHILE (cell loop) 


//***********************************************************************************************
 iter = data_all.begin();
 while (iter != data_all.end())
 {
  short int region = iter->POLESREG[2000];
  if (regions.find(region) != regions.end()) { // Test only some regions          
     
     short int countryreg = iter->COUNTRYREGMIX[2000];
     if (toAdjust.find(countryreg) != toAdjust.end()) {  
     if ((iter->PROTECT[2000]==0)&&((iter->POTVEG[2000]<10 && iter->FOREST[2000]>0 && iter->MAIE[2000]>0)||(iter->POTVEG[2000]<10 && (iter->NPP[2000]>0)||iter->MAIN[2000]>0))){
    int asID = iter->asID;
    short int country = iter->COUNTRY[2000];
    char regionch[2];
    int2str(region,regionch);
    string regprice = "re"+string(regionch)+"price0";
    string regprice0 = "re"+string(regionch)+"price0";
    char countrych[3];
    int2str(countryreg,countrych);
    string countryprice = "re"+string(countrych)+"price0";
    countryreg = iter->COUNTRYREGMIX[2000];     
    int xi = (iter->x);
    int yi = (iter->y);

//if (floor(woodHarvest[countryreg]) > 1.35 * wprod[countryprice].v(year))  
//if (floor(woodHarvest[countryreg]) > 1.15 * wprod[countryprice].v(year))
//if (floor(woodHarvest[countryreg]) > 1.1 * wprod[countryprice].v(year))  
//if (floor(woodHarvest[countryreg]) > 1.08 * wprod[countryprice].v(year))  
if (floor(woodHarvest[countryreg]) > 1.05 * wprod[countryprice].v(year))  
 {
    if ((managedForest.get(xi,yi)>0))
     {
      double sawnW = 0.;
      double restW = 0.;
      double sawnThW = 0.;
      double restThW = 0.;
      double sawnWnew = 0.;
      double restWnew = 0.;
      double sawnThWnew = 0.;
      double restThWnew = 0.;
//      double harvWoodTmp = 0.;
//      double harvestTmp = 0.;      
      double newHarvestTmp = 0;
      int biomassRot = 0;
//      int biomassRotTh =0;
      int rotMAI = 0;
      int rotMaxBm = 0;
      int Rotation = 0;
      
//      int managedForestTmp = 0;
//      double thinningForestTmp = 0;
//      int rotationForestTmp = 0;
//      double thinningForestNewTmp = 0;
//      int managedForestNewTmp = 0;
//      int rotationForestNewTmp = 0;

      double countryHarvestTmp = woodHarvest[countryreg];
      
//      g4m::ageStruct cohortTmp = *cohort_all[asID];
   	  double harvestTmp = harvestGrid.get(xi,yi);              

//cout << "xi= "<<xi<<"\t yi= "<<yi<<"\t MAI= "<< MAI<<"\t mai= "<< maiForest.get(xi,yi)<<"\t NPP= "<<iter->NPP[2000]<<"\t dNPP= "<<iter->"NPP"].v()<<endl; 

  if (dat_all[asID].ObiomassPrev >0 && iter->CABOVEHA.v() > 0 && maiForest.get(xi,yi) > 0)  
  {
//          biomassRotTh = fi.gUt(dat_all[asID].ObiomassPrev, maiForest.get(xi,yi), thinningForest.get(xi,yi));     // rotation time to get current biomass (without thinning)  
//          biomassRot = fi.gU(dat_all[asID].ObiomassPrev, maiForest.get(xi,yi), 1);     // rotation time to get current biomass (without thinning)  
          rotMaxBm = fi.gTopt(maiForest.get(xi,yi), 2);  
  }else if (dat_all[asID].prevPlantPhytHaBmGr >0)  
  {
//          biomassRotTh = fi.gUt(dat_all[asID].ObiomassPrev, maiForest.get(xi,yi), thinningForest.get(xi,yi));     // rotation time to get current biomass (without thinning)  
//          biomassRot = fi.gU(dat_all[asID].ObiomassPrev, maiForest.get(xi,yi), 1);     // rotation time to get current biomass (without thinning)  
          rotMaxBm = fi.gTopt(maiForest.get(xi,yi), 2);  
  }     
      int rotationForestTmp = rotationForest.get(xi,yi);
      int managedForestTmp = managedForest.get(xi,yi);
      double thinningForestTmp = thinningForest.get(xi,yi);
      int rotationForestNewTmp = rotationForestNew.get(xi,yi);
//      managedForestNewTmp = managedForestNew.get(xi,yi);
      double thinningForestNewTmp = thinningForestNew.get(xi,yi);

//---------------------------------------------------
//           int newForAge = newCohort_all[asID]->getActiveAge();  
//cout<<"newForAge= "<<newForAge<<endl;
//           if (newForAge > biomassRot)  // New forest age > rotation -> change FM for the new forest
            {
//              g4m::ageStruct cohortTmpNew = *newCohort_all[asID]; 
             if (managedForest.get(xi,yi) == 3)
                  {
// cout<<"if (managedForest.get(xi,yi) == 2)"<<endl;
                         managedForest.set(xi,yi,0);
//                         Rotation = biomassRot+1;
                          if (rotationForestTmp < rotMaxBm) {
                              Rotation = rotMaxBm;
                            }else{     
                              Rotation = rotationForestTmp;  }
//                         if (Rotation > rotMaxBm) {Rotation = rotMaxBm;}
                         rotationType.set(xi,yi,1);
                   }else if (managedForest.get(xi,yi) == 2)
                   {
// cout<<"if (managedForest.get(xi,yi) == 2)"<<endl;
                         managedForest.set(xi,yi,-1);
//                         Rotation = biomassRot+1;
                          if (rotationForestTmp < rotMaxBm) {
                              Rotation = rotMaxBm;
                            }else{     
                              Rotation = rotationForestTmp;  }
//                         if (Rotation > rotMaxBm) {Rotation = rotMaxBm;}
                         rotationType.set(xi,yi,1);
                   }else 
                   {     managedForest.set(xi,yi,-2);
//                         Rotation = biomassRot+1;
                          if (rotationForestTmp < rotMaxBm) {
                              Rotation = rotMaxBm;
                            }else{     
                              Rotation = rotationForestTmp;  }
//                         if (Rotation > rotMaxBm) {Rotation = rotMaxBm;}
                         rotationType.set(xi,yi,3);
//cout<<"if (managedForest.get(xi,yi) == 2) ELSE"<<endl;
                    }

                rotationForest.set(xi,yi,Rotation);	
                thinningForest.set(xi,yi,-1.);
//                cohortTmp.setRotPeriod(Rotation);
//                cohortTmp.setStockingdegree(-1.);                
                cohort_all[asID]->setRotPeriod(Rotation);
                cohort_all[asID]->setStockingdegree(-1.);

//                g4m::ageStruct::v resTmp = cohortTmp.aging();
//                sawnW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
//                restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
//                sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
//                restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.

                rotationForestNew.set(xi,yi,Rotation);	
                thinningForestNew.set(xi,yi,-1.);	
//                cohortTmpNew.setRotPeriod(Rotation);
//                cohortTmpNew.setStockingdegree(-1.);        
                newCohort_all[asID]->setRotPeriod(Rotation);
                newCohort_all[asID]->setStockingdegree(-1.);  
//                g4m::ageStruct::v resTmpNew = cohortTmpNew.aging();
//                g4m::ageStruct::v resTmpNew = newCohort_all[asID]->aging();
//                sawnWnew = resTmpNew.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
//                restWnew = resTmpNew.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
//                sawnThWnew = resTmpNew.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
//                restThWnew = resTmpNew.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
//  
//
//                newHarvestTmp = ((sawnW + restW + sawnThW + restThW + resUse*resTmp.hv) * (dat_all[asID].OforestShare-dat_all[asID].deforestShare)  +
//                             (sawnWnew + restWnew + sawnThWnew + restThWnew +resUse*resTmpNew.hv) * dat_all[asID].AforestShare) * 
//                              dat_all[asID].LandAreaHa * iter->FTIMBER.v() + dat_all[asID].deforWoodTotM3; // Total current harvested wood in the cell, m3

//           }else // New forest age <= rotation -> don't change FM for the new forest
//           {
//               if (managedForest.get(xi,yi) == 3)
//                  {
//// cout<<"if (managedForest.get(xi,yi) == 2)"<<endl;
//                         managedForest.set(xi,yi,0);
////                         Rotation = biomassRot+1;
//                          if (rotationForestTmp < rotMaxBm) {
//                              Rotation = rotMaxBm;
//                            }else{     
//                              Rotation = rotationForestTmp;  }
////                         if (Rotation > rotMaxBm) {Rotation = rotMaxBm;}
//                         rotationType.set(xi,yi,1);
//                   }else if (managedForest.get(xi,yi) == 2)
//                   {
//// cout<<"if (managedForest.get(xi,yi) == 2)"<<endl;
//                         managedForest.set(xi,yi,-1);
////                         Rotation = biomassRot+1;
//                          if (rotationForestTmp < rotMaxBm) {
//                              Rotation = rotMaxBm;
//                            }else{     
//                              Rotation = rotationForestTmp;  }
////                         if (Rotation > rotMaxBm) {Rotation = rotMaxBm;}
//                         rotationType.set(xi,yi,1);
//                   }else 
//                   {     managedForest.set(xi,yi,-2);
////                         Rotation = biomassRot+1;
//                          if (rotationForestTmp < rotMaxBm) {
//                              Rotation = rotMaxBm;
//                            }else{     
//                              Rotation = rotationForestTmp;  }
////                         if (Rotation > rotMaxBm) {Rotation = rotMaxBm;}
//                         rotationType.set(xi,yi,3);
////cout<<"if (managedForest.get(xi,yi) == 2) ELSE"<<endl;
//                    }
//                rotationForest.set(xi,yi,Rotation);	
//                thinningForest.set(xi,yi,-1.);	
//                cohortTmp.setRotPeriod(Rotation);
//                cohortTmp.setStockingdegree(-1.);
//                cohort_all[asID]->setRotPeriod(Rotation);
//                cohort_all[asID]->setStockingdegree(-1.);
////                g4m::ageStruct::v resTmp = cohortTmp.aging();
////                sawnW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
////                restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
////                sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
////                restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
////        
////                 
////                 newHarvestTmp = (sawnW + restW + sawnThW + restThW + resUse*resTmp.hv) * (dat_all[asID].OforestShare-dat_all[asID].deforestShare) * 
////                                   dat_all[asID].LandAreaHa * iter->FTIMBER.v() + dat_all[asID].deforWoodTotM3;
            } // End    else // New forest age < rotation -> don't change FM for the new forest

//                countryHarvestTmp += (newHarvestTmp-harvestTmp);
                countryHarvestTmp = countryHarvestTmp-harvestTmp+dat_all[asID].deforWoodTotM3;                
                
                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[countryreg] - wprod[countryprice].v(year)) 
                                   ){
                    woodHarvest[countryreg] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,dat_all[asID].deforWoodTotM3);
           	        manageChForest.set(xi,yi,1);
                }else{ // return old values
                rotationForest.set(xi,yi,rotationForestTmp);	
                thinningForest.set(xi,yi,thinningForestTmp);
                managedForest.set(xi,yi,managedForestTmp);	                   
                cohort_all[asID]->setRotPeriod(rotationForestTmp);
                cohort_all[asID]->setStockingdegree(thinningForestTmp);
                rotationForestNew.set(xi,yi,rotationForestNewTmp);	
                thinningForestNew.set(xi,yi,thinningForestNewTmp);
//                managedForestNew.set(xi,yi,managedForestNewTmp)	                   
                newCohort_all[asID]->setRotPeriod(rotationForestNewTmp);
                newCohort_all[asID]->setStockingdegree(thinningForestNewTmp);                 
                }
//         } // End if ((cohort_all[asID]->getArea(0) == 0.)||(cohort_all[asID]->getArea(0) >= 1/400.)){

        } // End  if ((managedForest(xi,yi)>0) & (managedForest(xi,yi)<3)
 } // End   else if (woodHarvest[region-1] >= (1.1 * wprod[region].v(year))   


//cout<< managedForest.get(xi,yi)<<endl;


/*  
cout <<"country=\t"<<Country0<<"\t woodHarvest=\t"<< woodHarvest[Country0-1];
cout <<"\t rotation=\t"<< Rotation<<"\t thinning= \t"<<thinningForest.get(xi,yi);
cout <<"\t sawnThW=\t"<<sawnThW*forestArea0*4<<"\t restThW=\t"<<restThW*forestArea0*4<<"\t";
cout <<"\t sawnW= \t"<<sawnW*forestArea0*4<<"\t restW= \t"<<restW*forestArea0*4;
cout << "\t sawnThWha=\t" << sawnThW << "\t restThWha=\t"<<restThW;
cout <<"\t sawnWha= \t"<<sawnW<<"\t restWha= \t"<<restW<<"\t abbiomassO= \t"<< abBiomassO<<"\t abbiomass= \t"<<iter->"BIOMASS"].v()<<"\t";
cout <<endl;
*/
//cout<< "managedForestSeting....\n";


   } //End for PROTECT == 0
  } //End some countries
 } // Test only some regions   
iter++;
} // End for WHILE (cell loop) 

//cout << "Third pass is finished"<< endl;

//*****************************************************************************


//******************************************************************************
//**************************Forth Pass********************
//******************************************************************************
//cout << "Start forth pass" << endl;
iter = data_all.begin();
while (iter != data_all.end()) {
 short int region = iter->POLESREG[2000];
 if (regions.find(region) != regions.end()) { // Test only some regions
  short int countryreg = iter->COUNTRYREGMIX[2000];     
  if (toAdjust.find(countryreg) != toAdjust.end()) {  
   if ((iter->PROTECT[2000]==0)&&((iter->POTVEG[2000]<10 && iter->FOREST[2000]>0 && iter->MAIE[2000]>0)||(iter->POTVEG[2000]<10 && (iter->NPP[2000]>0)||iter->MAIN[2000]>0))){
    short int country = iter->COUNTRY[2000];
    int asID = iter->asID;
    char regionch[2];
    int2str(region,regionch);
    string regprice = "re"+string(regionch)+"price0";
    string regprice0 = "re"+string(regionch)+"price0";
    char countrych[3];
    int2str(countryreg,countrych);
    string countryprice = "re"+string(countrych)+"price0";
    countryreg = iter->COUNTRYREGMIX[2000];     
    int xi = (iter->x);
    int yi = (iter->y);
    
   
      double sawnW = 0.;
      double restW = 0.;
      double sawnThW = 0.;
      double restThW = 0.;
      double sawnWnew = 0.;
      double restWnew = 0.;
      double sawnThWnew = 0.;
      double restThWnew = 0.;
      double harvWoodTmp = 0.;
      int biomassRotTh = 0;
      int biomassRot = 0;  
      int rotMAI = 0;
      int rotMaxBmTh = 0;
      int rotMaxBm = 0;  
      int Rotation = 0;
//      double harvestTmp = 0;
      double newHarvestTmp = 0;
      
//      int rotationForestTmp = 0;
//      int rotationForestNewTmp = 0;
      double countryHarvestTmp = woodHarvest[countryreg];
      
//  if (floor(woodHarvest[countryreg]) < 0.97 * wprod[countryprice].v(year))
  if (floor(woodHarvest[countryreg]) < 0.99 * wprod[countryprice].v(year))
    {if (managedForest.get(xi,yi)>1)
     {
      if (dat_all[asID].ObiomassPrev >0 && iter->CABOVEHA.v() > 0 && maiForest.get(xi,yi) > 0)  
      {
              biomassRotTh = fi.gUt(dat_all[asID].ObiomassPrev, maiForest.get(xi,yi), thinningForest.get(xi,yi));     // rotation time to get current biomass (with thinning)  
              rotMAI = fi.gTopt(maiForest.get(xi,yi), 1);
              rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), 3);         
      }else if (dat_all[asID].prevPlantPhytHaBmGr >0)  
      {
              rotMAI = fi.gTopt(maiForest.get(xi,yi), 1);
              rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), 3);         
              biomassRotTh =  rotMAI;
      }            
       int rotationForestTmp = rotationForest.get(xi,yi);
       int rotationForestNewTmp = rotationForestNew.get(xi,yi);
        g4m::ageStruct cohortTmpNew = *newCohort_all[asID];                                     
        g4m::ageStruct cohortTmp = *cohort_all[asID];
        int newForAge = newCohort_all[asID]->getActiveAge();          
    	double harvestTmp = harvestGrid.get(xi,yi);
//       if (rotMAI < rotationForest.get(xi,yi))  //TEST AT
//       {
//        Rotation = rotationForest.get(xi,yi) - 10;
//        if (Rotation < rotMAI) {Rotation = rotMAI;}

       if (rotMAI < rotationForest.get(xi,yi)) // TEST for AT
       {
//          Rotation = rotationForest.get(xi,yi) - 10;
          Rotation = rotationForest.get(xi,yi) - 5;          
          if (Rotation < rotMAI) {Rotation = rotMAI;}
          
        rotationForest.set(xi,yi,Rotation);		   
        cohortTmp.setRotPeriod(Rotation);
        cohort_all[asID]->setRotPeriod(Rotation);
       }else  if (rotMAI > rotationForest.get(xi,yi))
//       {Rotation = rotationForest.get(xi,yi) + 10;
       {Rotation = rotationForest.get(xi,yi) + 5;
        if (Rotation > rotMAI) {Rotation = rotMAI;}
        
        rotationForest.set(xi,yi,Rotation);		   
        cohortTmp.setRotPeriod(Rotation);
        cohort_all[asID]->setRotPeriod(Rotation);        
        }
           if ((newForAge > biomassRotTh) && (rotMAI < biomassRotTh)) {
//             Rotation = rotationForestNew.get(xi,yi) - 10;
             Rotation = rotationForestNew.get(xi,yi) - 5;             
             if (Rotation < rotMAI) {Rotation = rotMAI;}
             cohortTmpNew.setRotPeriod(Rotation);
             newCohort_all[asID]->setRotPeriod(Rotation);    
             rotationForestNew.set(xi,yi,Rotation);
             }
        g4m::ageStruct::v resTmp = cohortTmp.aging();
        sawnW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
        restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
        sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
        restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
        g4m::ageStruct::v resTmpNew = cohortTmpNew.aging();
        sawnWnew = resTmpNew.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
        restWnew = resTmpNew.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
        sawnThWnew = resTmpNew.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
        restThWnew = resTmpNew.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
        newHarvestTmp = ((sawnW + restW + sawnThW + restThW + resUse*resTmp.hv) * (dat_all[asID].OforestShare-dat_all[asID].deforestShare)  +
                         (sawnWnew + restWnew + sawnThWnew + restThWnew +resUse*resTmpNew.hv) * dat_all[asID].AforestShare) * 
                          dat_all[asID].LandAreaHa * iter->FTIMBER.v() + dat_all[asID].deforWoodTotM3; // Total current harvested wood in the cell, m3

                countryHarvestTmp += (newHarvestTmp-harvestTmp);
                
                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[countryreg] - wprod[countryprice].v(year)) 
                                   ){
                    woodHarvest[countryreg] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,newHarvestTmp);
                }else{ // return old values
                rotationForest.set(xi,yi,rotationForestTmp);	
                cohort_all[asID]->setRotPeriod(rotationForestTmp);
                rotationForestNew.set(xi,yi,rotationForestNewTmp);	
                newCohort_all[asID]->setRotPeriod(rotationForestNewTmp);
                }        
//        }
//        } //End if ((cohort_all[asID]->getArea(0) == 0.)||(cohort_all[asID]->getArea(0) >= 1/400.)){
       }  // end for if (managedForest(xi,yi)>=2)          
    
    } //end for if (woodHarvest[region] <= 0.9 * wprod[region].v(year))
//    else if (floor(woodHarvest[countryreg]) > 1.03 * wprod[countryprice].v(year)) 
    else if (floor(woodHarvest[countryreg]) > 1.01 * wprod[countryprice].v(year))     
    {if ((managedForest.get(xi,yi)>0) && (managedForest.get(xi,yi)<3))
     {
      if (dat_all[asID].ObiomassPrev >0 && iter->CABOVEHA.v() > 0 && maiForest.get(xi,yi) > 0)  
      {
              biomassRotTh = fi.gUt(dat_all[asID].ObiomassPrev, maiForest.get(xi,yi), thinningForest.get(xi,yi));     // rotation time to get current biomass (with thinning)  
              rotMAI = fi.gTopt(maiForest.get(xi,yi), 1);
              rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), 3);         
      }else if (dat_all[asID].prevPlantPhytHaBmGr >0)  
      {
              rotMAI = fi.gTopt(maiForest.get(xi,yi), 1);
              rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), 3);         
              biomassRotTh = rotMAI;
      }     

       int rotationForestTmp = rotationForest.get(xi,yi);
       int rotationForestNewTmp = rotationForestNew.get(xi,yi);
                                      
          g4m::ageStruct cohortTmp = *cohort_all[asID];
          g4m::ageStruct cohortTmpNew = *newCohort_all[asID]; 
          int newForAge = newCohort_all[asID]->getActiveAge();  
    	  double harvestTmp = harvestGrid.get(xi,yi);

       if (rotMaxBmTh > rotationForest.get(xi,yi)) //TEST AT
       {
//        Rotation = rotationForest.get(xi,yi) + 10;
        Rotation = rotationForest.get(xi,yi) + 5;
        if (Rotation > rotMaxBmTh) {Rotation = rotMaxBmTh;}


        rotationForest.set(xi,yi,Rotation);		   
        cohortTmp.setRotPeriod(Rotation);
        cohort_all[asID]->setRotPeriod(Rotation);       

           if ((newForAge > biomassRotTh) && (rotMaxBmTh > biomassRotTh)) {
//             Rotation = rotationForestNew.get(xi,yi) + 10;
             Rotation = rotationForestNew.get(xi,yi) + 5;             
             if (Rotation > rotMaxBmTh) {Rotation = rotMaxBmTh;}
             cohortTmpNew.setRotPeriod(Rotation);
             newCohort_all[asID]->setRotPeriod(Rotation);    
             rotationForestNew.set(xi,yi,Rotation);
             }
        g4m::ageStruct::v resTmp = cohortTmp.aging();
        sawnW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
        restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
        sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
        restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
        g4m::ageStruct::v resTmpNew = cohortTmpNew.aging();
        sawnWnew = resTmpNew.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
        restWnew = resTmpNew.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
        sawnThWnew = resTmpNew.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
        restThWnew = resTmpNew.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
        newHarvestTmp = ((sawnW + restW + sawnThW + restThW + resUse*resTmp.hv) * (dat_all[asID].OforestShare-dat_all[asID].deforestShare)  +
                         (sawnWnew + restWnew + sawnThWnew + restThWnew + resUse*resTmpNew.hv) * dat_all[asID].AforestShare) * 
                          dat_all[asID].LandAreaHa * iter->FTIMBER.v() + dat_all[asID].deforWoodTotM3; // Total current harvested wood in the cell, m3
                countryHarvestTmp += (newHarvestTmp-harvestTmp);
                
                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[countryreg] - wprod[countryprice].v(year)) 
                                   ){
                    woodHarvest[countryreg] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,newHarvestTmp);
                }else{ // return old values
                rotationForest.set(xi,yi,rotationForestTmp);	
                cohort_all[asID]->setRotPeriod(rotationForestTmp);
                rotationForestNew.set(xi,yi,rotationForestNewTmp);	
                newCohort_all[asID]->setRotPeriod(rotationForestNewTmp);
                }        
        }
       }  // end for if (managedForest(xi,yi)<=2)  
      }   //end for esle if (woodHarvest[region] >= 1.1 * wprod[region].v(year))
  } // End protect
 } // End some countries
 } // Test only some regions
  iter++;
} // End While
// 
//************************End of Forth Pass************************************
//cout << "End of Forth pass"<<endl;


//******************************************************************************
//**************************Fifth Pass********************
//******************************************************************************
//cout << "Start fifth pass" << endl;
iter = data_all.begin();
while (iter != data_all.end()) {
 short int region = iter->POLESREG[2000];
 if (regions.find(region) != regions.end()) { // Test only some regions
  short int countryreg = iter->COUNTRYREGMIX[2000];
  if (toAdjust.find(countryreg) != toAdjust.end()) {  
   if ((iter->PROTECT[2000]==0)&&((iter->POTVEG[2000]<10 && iter->FOREST[2000]>0 && iter->MAIE[2000]>0)||(iter->POTVEG[2000]<10 && (iter->NPP[2000]>0)||iter->MAIN[2000]>0))){
    short int country = iter->COUNTRY[2000];
    int asID = iter->asID;
    char regionch[2];
    int2str(region,regionch);
    string regprice = "re"+string(regionch)+"price0";
    string regprice0 = "re"+string(regionch)+"price0";
    char countrych[3];
    int2str(countryreg,countrych);
    string countryprice = "re"+string(countrych)+"price0";
    countryreg = iter->COUNTRYREGMIX[2000]; 
    
    int xi = (iter->x);
    int yi = (iter->y);
    
   
  double sawnW = 0.;
  double restW = 0.;
  double sawnThW = 0.;
  double restThW = 0.;
  double sawnWnew = 0.;
  double restWnew = 0.;
  double sawnThWnew = 0.;
  double restThWnew = 0.;
  double harvWoodTmp = 0.;
  int biomassRotTh = 0;
  int biomassRot = 0;  
  int rotMAI = 0;
  int rotMaxBmTh = 0;
  int rotMaxBm = 0;  
  int Rotation = 0;
//  double harvestTmp = 0;
  double newHarvestTmp = 0;
  
//  int rotationForestTmp = 0;
//  int rotationForestNewTmp = 0;
  double countryHarvestTmp = woodHarvest[countryreg];
//  if (floor(woodHarvest[countryreg]) < 0.9 * wprod[countryprice].v(year))
//  if (floor(woodHarvest[countryreg]) < 0.95 * wprod[countryprice].v(year))
  if (floor(woodHarvest[countryreg]) < 0.98 * wprod[countryprice].v(year))
    {if (managedForest.get(xi,yi)>0)
     {
      if (dat_all[asID].ObiomassPrev >0 && iter->CABOVEHA.v() > 0 && maiForest.get(xi,yi) > 0)  
      {
              biomassRotTh = fi.gUt(dat_all[asID].ObiomassPrev, maiForest.get(xi,yi), thinningForest.get(xi,yi));     // rotation time to get current biomass (with thinning)  
              rotMAI = fi.gTopt(maiForest.get(xi,yi), 1);
              rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), 3);         
   
      }else if (dat_all[asID].prevPlantPhytHaBmGr >0)  
      {
              rotMAI = fi.gTopt(maiForest.get(xi,yi), 1);
              rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), 3);         
              biomassRotTh = rotMAI;
      }     

           int rotationForestTmp = rotationForest.get(xi,yi);
           int rotationForestNewTmp = rotationForestNew.get(xi,yi);
           g4m::ageStruct cohortTmp = *cohort_all[asID];
           g4m::ageStruct cohortTmpNew = *newCohort_all[asID];            
           int newForAge = newCohort_all[asID]->getActiveAge();          
           double harvestTmp = harvestGrid.get(xi,yi);
//       if (rotMAI < rotationForest.get(xi,yi))  //TEST AT
//       {
//        Rotation = rotationForest.get(xi,yi) - 10;
//        if (Rotation < rotMAI) {Rotation = rotMAI;}

       if (rotMAI < rotationForest.get(xi,yi)) // TEST for AT
       {
//          Rotation = rotationForest.get(xi,yi) - 10;
          Rotation = rotationForest.get(xi,yi) - 5;          
          if (Rotation < rotMAI) {Rotation = rotMAI;}

        rotationForest.set(xi,yi,Rotation);		   
        cohortTmp.setRotPeriod(Rotation);
        cohort_all[asID]->setRotPeriod(Rotation);
       }else  if (rotMAI > rotationForest.get(xi,yi))
//       {Rotation = rotationForest.get(xi,yi) + 10;
       {Rotation = rotationForest.get(xi,yi) + 5;       
        if (Rotation > rotMAI) {Rotation = rotMAI;}
        else {Rotation = rotMAI;}

        rotationForest.set(xi,yi,Rotation);		   
        cohortTmp.setRotPeriod(Rotation);
        cohort_all[asID]->setRotPeriod(Rotation);
        }


           if ((newForAge > biomassRotTh) && (rotMAI < biomassRotTh)) {
//             Rotation = rotationForestNew.get(xi,yi) - 10;
             Rotation = rotationForestNew.get(xi,yi) - 5;
             if (Rotation < rotMAI) {Rotation = rotMAI;}
             cohortTmpNew.setRotPeriod(Rotation);
             newCohort_all[asID]->setRotPeriod(Rotation);    
             rotationForestNew.set(xi,yi,Rotation);
             }
        g4m::ageStruct::v resTmp = cohortTmp.aging();
        sawnW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
        restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
        sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
        restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
        g4m::ageStruct::v resTmpNew = cohortTmpNew.aging();
        sawnWnew = resTmpNew.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
        restWnew = resTmpNew.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
        sawnThWnew = resTmpNew.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
        restThWnew = resTmpNew.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
        newHarvestTmp = ((sawnW + restW + sawnThW + restThW + resUse*resTmp.hv) * (dat_all[asID].OforestShare-dat_all[asID].deforestShare)  +
                         (sawnWnew + restWnew + sawnThWnew + restThWnew +resUse*resTmpNew.hv) * dat_all[asID].AforestShare) * 
                          dat_all[asID].LandAreaHa * iter->FTIMBER.v() + dat_all[asID].deforWoodTotM3; // Total current harvested wood in the cell, m3

                countryHarvestTmp += (newHarvestTmp-harvestTmp);
                
                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[countryreg] - wprod[countryprice].v(year)) 
                                   ){
                    woodHarvest[countryreg] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,newHarvestTmp);
                }else{ // return old values
                rotationForest.set(xi,yi,rotationForestTmp);	
                cohort_all[asID]->setRotPeriod(rotationForestTmp);
                rotationForestNew.set(xi,yi,rotationForestNewTmp);	
                newCohort_all[asID]->setRotPeriod(rotationForestNewTmp);
                }        
//        }
       }  // end for if (managedForest(xi,yi)>=2)          
    
    } //end for if (woodHarvest[region] <= 0.9 * wprod[region].v(year))
//    else if (floor(woodHarvest[countryreg]) > 1.1 * wprod[countryprice].v(year)) 
//    else if (floor(woodHarvest[countryreg]) > 1.05 * wprod[countryprice].v(year)) 
//    else if (floor(woodHarvest[countryreg]) > 1.03 * wprod[countryprice].v(year))     
    else if (floor(woodHarvest[countryreg]) > 1.02 * wprod[countryprice].v(year))     
    {if (managedForest.get(xi,yi)>0) 
     {
      if (dat_all[asID].ObiomassPrev >0 && iter->CABOVEHA.v() > 0 && maiForest.get(xi,yi) > 0)  
      {
              biomassRotTh = fi.gUt(dat_all[asID].ObiomassPrev, maiForest.get(xi,yi), thinningForest.get(xi,yi));     // rotation time to get current biomass (with thinning)  
              rotMAI = fi.gTopt(maiForest.get(xi,yi), 1);
              rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), 3);         
      }else if (dat_all[asID].prevPlantPhytHaBmGr >0)  
      {
              rotMAI = fi.gTopt(maiForest.get(xi,yi), 1);
              rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), 3);         
              biomassRotTh = rotMAI;
      }     

           int rotationForestTmp = rotationForest.get(xi,yi);
           int rotationForestNewTmp = rotationForestNew.get(xi,yi);

          g4m::ageStruct cohortTmp = *cohort_all[asID];
          g4m::ageStruct cohortTmpNew = *newCohort_all[asID]; 
          int newForAge = newCohort_all[asID]->getActiveAge();  
    	  double harvestTmp = harvestGrid.get(xi,yi);

       if (rotMaxBmTh > rotationForest.get(xi,yi)) //TEST AT
       {
//        Rotation = rotationForest.get(xi,yi) + 10;
        Rotation = rotationForest.get(xi,yi) + 5;        
        if (Rotation > rotMaxBmTh) {Rotation = rotMaxBmTh;}
        rotationForest.set(xi,yi,Rotation);		   
        cohortTmp.setRotPeriod(Rotation);
        cohort_all[asID]->setRotPeriod(Rotation);       

           if ((newForAge > biomassRotTh) && (rotMaxBmTh > biomassRotTh)) {
//             Rotation = rotationForestNew.get(xi,yi) + 10;
             Rotation = rotationForestNew.get(xi,yi) + 5;             
             if (Rotation > rotMaxBmTh) {Rotation = rotMaxBmTh;}
             cohortTmpNew.setRotPeriod(Rotation);
             newCohort_all[asID]->setRotPeriod(Rotation);    
             rotationForestNew.set(xi,yi,Rotation);
             }
        g4m::ageStruct::v resTmp = cohortTmp.aging();
        sawnW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
        restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
        sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
        restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
        g4m::ageStruct::v resTmpNew = cohortTmpNew.aging();
        sawnWnew = resTmpNew.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
        restWnew = resTmpNew.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
        sawnThWnew = resTmpNew.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
        restThWnew = resTmpNew.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
        newHarvestTmp = ((sawnW + restW + sawnThW + restThW + resUse*resTmp.hv) * (dat_all[asID].OforestShare-dat_all[asID].deforestShare)  +
                         (sawnWnew + restWnew + sawnThWnew + restThWnew + resUse*resTmpNew.hv) * dat_all[asID].AforestShare) * 
                          dat_all[asID].LandAreaHa * iter->FTIMBER.v() + dat_all[asID].deforWoodTotM3; // Total current harvested wood in the cell, m3
                countryHarvestTmp += (newHarvestTmp-harvestTmp);
                
                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[countryreg] - wprod[countryprice].v(year)) 
                                   ){
                    woodHarvest[countryreg] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,newHarvestTmp);
                }else{ // return old values
                rotationForest.set(xi,yi,rotationForestTmp);	
                cohort_all[asID]->setRotPeriod(rotationForestTmp);
                rotationForestNew.set(xi,yi,rotationForestNewTmp);	
                newCohort_all[asID]->setRotPeriod(rotationForestNewTmp);
                }        
        }
       }  // end for if (managedForest(xi,yi)<=2)  
      }   //end for esle if (woodHarvest[region] >= 1.1 * wprod[region].v(year))
//if (country==47){cout<<"thinningForest(FM100E)\t"<<thinningForest.get(xi,yi)<<"\tmanagedForest\t"<<int(managedForest.get(xi,yi))<<endl;}
  } // End protect
 } // End some countries
 } // Test only some regions
  iter++;
} // End While
// 
//************************End of Fifth Pass************************************
//cout << "End of Fifth pass"<<endl;

if (harvControl){
      for (int i=1; i<=NumberOfCountryregmix; i++){
        char countrych[3];
        int ii = i;
        int2str(ii,countrych);
        string countryprice = "re"+string(countrych)+"price0";
        if (wprod[countryprice].v(year) > 0){
        
//             if ((regions.find(coutryRegion[i]) != regions.end()) 
//                      && (countriesList.find(i) != countriesList.end())  // Test only some countries         
//                         && i!=114  // Luxenbourg is too small (1 cell) to be considered correctly
//                         )
//              {                                  
                harvDiff[i] = abs(woodHarvest[i] - wprod[countryprice].v(year))/wprod[countryprice].v(year);                 
    //cout<<"harvDiff["<<i<<"]"<<"\t"<<harvDiff[i]<<endl;
cout<<"harvDiff["<<i<<"]="<<"\t"<<harvDiff[i]<<"\t woodHarvest["<<i<<"]="<<"\t"<<woodHarvest[i]<<"\t wprod["<<countryprice<<"]"<<year<<"=\t"<<wprod[countryprice].v(year)<<endl;
//              }else{
//               harvDiff[i] = 0;
//              }
        }else{
        harvDiff[i] = 0;
        }
    //cout<<"harvDiff["<<i<<"]"<<"\t"<<harvDiff[i]<<endl;
      }
}







//*****************************************************************************
// Adjusting unmanaged forest rotation length to match reported FM sink
//*****************************************************************************

if (adjustFMsink){
  if (year == 2000){
         iter = data_all.begin();
         while (iter != data_all.end()) {
           if (regions.find(iter->POLESREG[2000]) != regions.end()) { // Test only some regions
            short int country = iter->COUNTRY[2000];
            if (countriesList.find(country) != countriesList.end()) { // Test only some countries      
             if ((iter->PROTECT[2000]==0)&&((iter->POTVEG[2000]<10 && iter->FOREST[2000]>0 && iter->MAIE[2000]>0)||(iter->POTVEG[2000]<10 && (iter->NPP[2000]>0)||iter->MAIN[2000]>0))){
              int asID = iter->asID;
              FMs[country-1]+=(cohort_all[asID]->getBm() - dat_all[asID].Obiomass0) *
                     (iter->FOREST[2000])*(dat_all[asID].LandAreaHa)*3.6666666667/10000.;  // FM sink averaged over 5 years, GgCO2/yr
             }
            } 
           }
           iter++;
        }

         iter = data_all.begin();
         while (iter != data_all.end()) {
         if (regions.find(iter->POLESREG[2000]) != regions.end()) { // Test only some regions
          short int country = iter->COUNTRY[2000];
          if (countriesList.find(country) != countriesList.end()) { // Test only some countries      
          if ((iter->PROTECT[2000]==0)&&((iter->POTVEG[2000]<10 && iter->FOREST[2000]>0 && iter->MAIE[2000]>0)||(iter->POTVEG[2000]<10 && (iter->NPP[2000]>0)||iter->MAIN[2000]>0))){
            int asID = iter->asID;
            int xi = (iter->x);
            int yi = (iter->y);
//cout<<"asID= \t"<<asID<<"\t country= \t"<<country<<"\t FMs= \t"<<FMs[country-1]<<"\t Fmstat= \t"<<FM_sink_stat[country-1]<<"\t unmanaged= \t"<<int(unmanaged.get(xi,yi))<<endl;
            if (unmanaged.get(xi,yi)>0){
                int biomassRot = 0;  
                int rotMaxBm = 0;  
                int Rotation = 0;
                if (FMs[country-1] < 0.85*FM_sink_stat[country-1]){
                 if (iter->FOREST[1990] >0 && iter->CABOVEHA[1990] > 0 && maiForest.get(xi,yi) > 0)  
                  {  
//                     biomassRot = fi.gU(iter->CABOVEHA[1990], maiForest.get(xi,yi), 1);     // rotation time to get current biomass (without thinning)  
//                     biomassRot = fi.gU(dat_all[asID].Obiomass0, maiForest.get(xi,yi), 1);     // rotation time to get current biomass (without thinning)  
                     rotMaxBm = fi.gTopt(maiForest.get(xi,yi), 2);         

                    }     
//cout<<"rotationForest= \t"<<rotationForest.get(xi,yi)<<"\t rotMaxBm= \t"<< rotMaxBm<<endl;
                  if (rotationForest.get(xi,yi) < rotMaxBm){
                     double BmTmp = 0.;
        
                     g4m::ageStruct cohortTmp = *cohort_all[asID];
                     BmTmp = cohortTmp.getBm();
                     cohort_all[asID]->setRotPeriod(rotMaxBm);
                     rotationForest.set(xi,yi,rotMaxBm);
        
                     cohortTmp.setRotPeriod(rotMaxBm);
                     cohortTmp.aging();
                     cohortTmp.aging();
                     cohortTmp.aging();
                     cohortTmp.aging();                                       
                     cohortTmp.aging();   

                     cohortTmp.aging();
                     cohortTmp.aging();
                     cohortTmp.aging();
                     cohortTmp.aging();                                       
                     cohortTmp.aging();                                 
                     FMs[country-1]+=((cohortTmp.getBm()-BmTmp) - (BmTmp-dat_all[asID].Obiomass0)) *
                          (iter->FOREST[2000]) * (dat_all[asID].LandAreaHa) * 3.6666666667/10000.;
                 }
              }
            }
           } // Protect
          } // Test some countries
         } // Test some regions
           iter++;
       }// While
 } // if year ==1995
} //if (adjustFMsink)         


}// Else

if (harvControl){

  double woodHarvest[NumberOfCountryregmix];
  woodHarvest[0]=0.;
  for (int i=1; i<NumberOfCountryregmix; i++){woodHarvest[i]= 0.;}

iter = data_all.begin();
while (iter != data_all.end()) {
  short int region = iter->POLESREG[2000];
  if (regions.find(region) != regions.end()) { // Test only some regions         
     short int country = iter->COUNTRY[2000];
     if (countriesList.find(country) != countriesList.end()) { // Test only some countries       
       int asID = iter->asID;
       if ((iter->PROTECT[2000]==0)&&((iter->POTVEG[2000]<10 && iter->FOREST[2000]>0 && iter->MAIE[2000]>0)||(iter->POTVEG[2000]<10 && (iter->NPP[2000]>0)||iter->MAIN[2000]>0))){
          short int countryreg = iter->COUNTRYREGMIX[2000];
          int xi = (iter->x);
          int yi = (iter->y);
          double sawnW = 0.;
          double restW = 0.;
          double sawnThW = 0.;
          double restThW = 0.;
          double sawnWnew = 0.;
          double restWnew = 0.;
          double sawnThWnew = 0.;
          double restThWnew = 0.;
          double resLoss = 0.;
          double resLossNew = 0.;        
          double harvestTmp = 0;
          double newHarvestTmp = 0;

          
//          if (dat_all[asID].ObiomassPrev >0 && iter->CABOVEHA.v() > 0 && maiForest.get(xi,yi) > 0 && thinningForest.get(xi,yi) >0)  
          if (thinningForest.get(xi,yi) >0)  
          {
//                double biomassRotTh = fi.gUt(dat_all[asID].ObiomassPrev, maiForest.get(xi,yi), thinningForest.get(xi,yi));     // rotation time to get current biomass (without thinning)  
                g4m::ageStruct cohortTmp = *cohort_all[asID];
                g4m::ageStruct cohortTmpNew = *newCohort_all[asID];
            	harvestTmp = harvestGrid.get(xi,yi);
                g4m::ageStruct::v resTmp = cohortTmp.aging();
                sawnW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
                restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
                sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
                restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
                resLoss = resTmp.hv;          
                
                g4m::ageStruct::v resTmpNew = cohortTmpNew.aging();
                sawnWnew = resTmpNew.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
                restWnew = resTmpNew.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
                sawnThWnew = resTmpNew.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
                restThWnew = resTmpNew.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.  
                resLossNew = resTmpNew.hv;          

                newHarvestTmp = ((sawnW + restW + sawnThW + restThW + resUse*resLoss) * (dat_all[asID].OforestShare-dat_all[asID].deforestShare)  +
                             (sawnWnew + restWnew + sawnThWnew + restThWnew +resUse*resLossNew) * dat_all[asID].AforestShare) * 
                              dat_all[asID].LandAreaHa * iter->FTIMBER.v() + dat_all[asID].deforWoodTotM3; // Expected total current harvested wood in the cell, m3

                 woodHarvest[countryreg] += (newHarvestTmp);
       
           }else{
                 woodHarvest[countryreg] += dat_all[asID].deforWoodTotM3;
           }                   
//if (country==47){cout<<"thinningForest(FM0E)\t"<<thinningForest.get(xi,yi)<<"\tmanagedForest\t"<<int(managedForest.get(xi,yi))<<endl;}
//if (country==47){ cout<<"countryFM = \t"<<country<<"\t deforWoodTotM3 = \t"<<dat_all[asID].deforWoodTotM3<<"\tdeforest=\t"<<dat_all[asID].deforestShare<<"\toforest=\t"<<dat_all[asID].OforestShare<<endl;       }
//if (country==47){ cout<<"countryFMout = \t"<<country<<"\t deforWoodTotM3 = \t"<<dat_all[asID].deforWoodTotM3;
//cout<<"\t harvest=\t"<<(sawnW + restW + sawnThW + restThW + resUse*resLoss)*(dat_all[asID].OforestShare)*iter->FTIMBER.v()*dat_all[asID].LandAreaHa;
//cout<<"\tnewHarvest=\t"<<(sawnWnew + restWnew + sawnThWnew + restThWnew +resUse*resLossNew) * dat_all[asID].AforestShare*iter->FTIMBER.v()*dat_all[asID].LandAreaHa;
//cout<<"\tnewHarvestGrid=\t"<<harvestTmp<<"\tharvTot=\t"<<newHarvestTmp;
//cout<<"\t thinning=\t"<<thinningForest.get(xi,yi)<<"\trot=\t"<<rotationForest.get(xi,yi)<<endl;}

         }
      }
   }
  iter++;
}
//cout<<"year=\t"<<year<<"\t outputHarvest(11)=\t"<<woodHarvest[1]<<endl;
//cout<<"year=\t"<<year<<"\t outputHarvest(11)=\t"<<woodHarvest[11]<<endl;
//cout<<"year=\t"<<year<<"\t outputHarvest(47)=\t"<<woodHarvest[47]<<endl;
//cout<<"year=\t"<<year<<"\t outputHarvest(47)=\t"<<woodHarvest[49]<<endl;
} // End if harvControl
}
