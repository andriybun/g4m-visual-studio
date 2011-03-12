#include "forest.h"
#include "dima.h"

extern g4m::coeffStruct coeff;
extern settingsT settings;

extern map<string, g4m::ipol<double,double> > wprice;
extern map<string, g4m::ipol<double,double> > wprod;

extern g4m::ipol<double,double> sws;
extern g4m::ipol<double,double> hlv;
extern g4m::ipol<double,double> hle;
extern g4m::ipol<double,double> dbv;
extern g4m::ipol<double,double> dbe;

extern set<int> regions;

extern double MAI_CountryUprotect[NumberOfCountries];
extern double MAI_CountryAll[NumberOfCountries];
extern double MAI_countryregmix_up_avg[NumberOfCountryregmix];

extern double Hurdle_opt[NumberOfCountries];
extern double countryLosses[NumberOfCountries];

extern int PriceCiS[11];

extern double resUse;

extern int numAgeStruct;

void initManagedForest(dataDetStruct &data_all, g4m::incrementTab &fi, datGlobal &dat_all,
                       ageStructVector &cohort_all, ageStructVector &newCohort_all,
                       griddata2<double> &maiForest, griddata2<double> &thinningForest,
                       griddataLite<char> &rotationType, griddataLite<char> &managedForest,
                       griddata2<double> &rotationForest, griddata2<double> &harvestGrid, griddata2<double> &OforestShGrid) 

 {
  double woodHarvest[NumberOfCountryregmix];
  double woodLost[NumberOfCountryregmix];
//  double woodHarvestStat[NumberOfCountries];
  int managedCount[NumberOfCountryregmix];

  for (int i=0; i<NumberOfCountryregmix; i++){
    woodHarvest[i]=0.; 
    woodLost[i]=0.;
//    woodHarvestStat[i]=0.;
    managedCount[i]=0;
  }

//  int year =1990;
  int year =2000;

  double sawnW = 0.;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
  double restW = 0.;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
  double sawnThW = 0.;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
  double restThW = 0.;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
//  double defIncome = 0.;

          int optimUnmanaged = 0;
          int optimMAI = 1;
          int optimMaxBm = 2;
          int optimMaxBmTh = 3;
          int optimHarvFin = 4;
          int optimHarvAve = 5;
//          int rotUnmanaged = 0;
//          int rotMAI = 0;
//          int rotMaxBm = 0;
//          int rotMaxBmTh = 0;
//          int rotHarvFin = 0;
//          int rotHarvAve = 0;

//   Start Zero initialisation
//------------------------------------------------------------------------------
  dataDetStruct::iterator iter = data_all.begin();
  while (iter != data_all.end()) {
   if (regions.find(iter->POLESREG[2000]) != regions.end()) { // Test only some regions
    if (iter->PROTECT[2000] == 0) {
    	int xi = (iter->x);
    	int yi = (iter->y);
        int countryreg = (int)iter->COUNTRYREGMIX.v();
        int Country0 = (int)iter->COUNTRY.v();
        double LandAreaHa = iter->LANDAREA.v()*100;
        double forestShare0 = 0;
        if (iter->FOREST.v()+(iter->CROP.v())+(iter->BUILTUP.v())>1)
           {forestShare0 = (1-(iter->CROP.v()+iter->BUILTUP.v()));}
           else {forestShare0 = iter->FOREST.v();}
		OforestShGrid.set(xi,yi,forestShare0);
        double forestArea0 = LandAreaHa * forestShare0;
        int biomasRot=0;  // MG: rotation time fitted to get certain biomass under certain MAI (w/o thinning)
        int biomasRotTh=0;  // MG: rotation time fitted to get certain biomass under certain MAI (with thinning)    
        double harvWood=0.; //MG: harvestable wood, m3
        double abBiomassO = 0.;
        double MAI = 0.;
        double defIncome = 0.;

        int rotUnmanaged = 0;
        int rotMAI = 0;
        int rotMaxBm = 0;
        int rotMaxBmTh = 0;
        int rotHarvFin = 0;
        int rotHarvAve = 0;

        if (forestShare0 > 0){
           MAI = iter->MAIE.v(); // Max mean annual increment (tC/ha) of Existing forest (with uniform age structure and managed with rotation length maximazing MAI)
        }else{
           MAI = iter->MAIN.v(); // Max mean annual increment of New forest (with uniform age structure and managed with rotation length maximazing MAI)
        }

        if (MAI < 0.){MAI = 0.;}     
        maiForest.set(xi,yi,MAI);
        
        double harvMAI = MAI*iter->FTIMBER.v()*(1-coeff.HarvLoos.v());


 //       double forFlag = 0.;          //MG: a forest area for fitting existing forest in the grid: 0-no forest; 1 - 1 ha of normal forest

        if (iter->CABOVEHA.v() > 0 && maiForest.get(xi,yi)> 0) {
          biomasRot = fi.gU(iter->CABOVEHA.v(), MAI, 1.);        // rotation time to get current biomass (without thinning)
          biomasRotTh = fi.gUt(iter->CABOVEHA.v(), MAI, 1.);     // rotation time to get current biomass (with thinning)     

        }
        if (maiForest.get(xi,yi)> 0){
          rotMAI = fi.gTopt(MAI, optimMAI);
//          rotMaxBm = fi.gTopt(MAI, optimMaxBm);                        
//          rotMaxBmTh = fi.gTopt(MAI, optimMaxBmTh);
        }

          dima decision(2000
		       , iter->NPP
		       , iter->SPOPDENS
		       , iter->SAGRSUIT
		       , iter->PRICEINDEX
		       , coeff.PriceIndexE
		       , iter->R
		       , coeff.PriceC
		       , coeff.PlantingCostsR
		       , coeff.PriceLandMinR
		       , coeff.PriceLandMaxR
		       , coeff.MaxRotInter
		       , coeff.MinRotInter
		       , coeff.decRateL
		       , coeff.decRateS
		       , iter->FRACLONGPROD
		       , coeff.baseline
		       , iter->FTIMBER 
		       , coeff.PriceTimberMaxR
		       , coeff.PriceTimberMinR
		       , coeff.FCuptake
		       , iter->GDP
		       , coeff.HarvLoos
		       , forestShare0
		       , wprice["regprice"]
		       , wprice["regprice0"].v(2000)
               , rotMAI
               , harvMAI); 
//               , Rotation
//               , harvWood );


    int Rotation = 0;

    if (iter->MANAGEDFLAG.v(2000) >0) {
      thinningForest.set(xi,yi,1.);
      Rotation = biomasRotTh+1;
if (Rotation < rotMAI) Rotation = rotMAI;
      rotationType.set(xi,yi,11);


	      double pDefIncome = iter->CABOVEHA.v() * 
                   (decision.priceTimber() * iter->FTIMBER.v() * (1. -coeff.HarvLoos.v()));
	      //Immediate Pay if deforested (Slash and Burn)
	      double sDefIncome = iter->CABOVEHA.v() *
		           (decision.priceTimber() * iter->FTIMBER.v()
		         * (1. -coeff.HarvLoos.v()));
	      defIncome = pDefIncome * (1. - iter->SLASHBURN.v())
		            + sDefIncome * iter->SLASHBURN.v();


          if (MAI>MAI_countryregmix_up_avg[countryreg]) {
                managedForest.set(xi,yi,3.);
             } else {
               if ((decision.forValNC() * Hurdle_opt[Country0-1]) > (decision.agrVal() + defIncome)) {
                 managedForest.set(xi,yi,2.);
                } else {
                managedForest.set(xi,yi,1.);
                }
              }

    } else {
          thinningForest.set(xi,yi,-1.);
          Rotation = biomasRot+1;
          if (Rotation < rotMAI) Rotation = rotMAI;
          rotationType.set(xi,yi,10);

	      double pDefIncome = iter->CABOVEHA.v() * 
                   (decision.priceTimber() * iter->FTIMBER.v() * (1. -coeff.HarvLoos.v()));
	      //Immediate Pay if deforested (Slash and Burn)
	      double sDefIncome = iter->CABOVEHA.v() *
		           (decision.priceTimber() * iter->FTIMBER.v()
		         * (1. -coeff.HarvLoos.v()));
	      defIncome = pDefIncome * (1. - iter->SLASHBURN.v())
		            + sDefIncome * iter->SLASHBURN.v();

          if (MAI > MAI_CountryUprotect[Country0-1]) {
                managedForest.set(xi,yi,0.);
             } else {
               if ((decision.forValNC() * Hurdle_opt[Country0-1]) > (decision.agrVal() + defIncome)) {
                 managedForest.set(xi,yi,-1.);
                } else {
                 managedForest.set(xi,yi,-2.);
                }
             }
    }
    rotationForest.set(xi,yi,Rotation);	

    }          // End if (iter->PROTECT[2000] == 0)
  }// Test only some regions
  iter++;
}            // End while(iter != data_all.end())

//    initLoop(0, data_all, fi, cohort_all, newCohort_all, dat_all, maiForest, thinningForest, rotationForest);
//    initCohorts(data_all, fi, cohort_all,maiForest, thinningForest, rotationForest); 
//    initCohorts(data_all, fi, cohort_all,newCohort_all,maiForest, thinningForest, rotationForest); 

cout<<"before initloop"<<endl;
    initLoop(0, data_all, fi, cohort_all, newCohort_all, dat_all, maiForest, thinningForest, rotationForest);
cout << "end of first pass" << endl;



//******************************************************************************
//**************************First Pass********************
//           Init havWood
//******************************************************************************

  iter = data_all.begin();
  while (iter != data_all.end()) {
   if (regions.find(iter->POLESREG[2000]) != regions.end()) { // Test only some regions        
    if ((iter->PROTECT[2000]==0)&&((iter->POTVEG[2000]<10 && iter->FOREST[2000]>0 && iter->MAIE[2000]>0)||(iter->POTVEG[2000]<10 && (iter->NPP[2000]>0)||iter->MAIN[2000]>0))){   // We consider only unprotected land

          double abBiomassO = 0.;  
    
            int asID = iter->asID;
            short int countryreg = (int)iter->COUNTRYREGMIX.v();
            short int region = iter->POLESREG[2000];
            string regprice = "re"+IntToStr(region)+"price0";
            
            string countryregprice = "re"+IntToStr(countryreg)+"price0";
            countryreg = (int)iter->COUNTRYREGMIX.v();
                        
            short int Country0 = iter->COUNTRY[2000]; 
            int xi = (iter->x);
            int yi = (iter->y);
          int rotUnmanaged = 0;
          int rotMAI = 0;
          int rotMaxBm = 0;
          int rotMaxBmTh = 0;
          int rotHarvFin = 0;
          int rotHarvAve = 0;
          int biomasRot=0;  // MG: rotation time fitted to get certain biomass under certain MAI (w/o thinning)
          int biomasRotTh=0;  // MG: rotation time fitted to get certain biomass under certain MAI (with thinning)

          double LandAreaHa = iter->LANDAREA.v()*100;
          double forestShare0 = 0;
          if (iter->FOREST.v()+(iter->CROP.v())+(iter->BUILTUP.v())>1)
           {forestShare0 = (1-(iter->CROP.v()+iter->BUILTUP.v()));}
           else {forestShare0 = iter->FOREST.v();}
     
           double forestArea0 = LandAreaHa * forestShare0;
          double harvMAI = maiForest.get(xi,yi)*iter->FTIMBER.v()*(1-coeff.HarvLoos.v());
          double harvWood=0; //MG: harvestable wood, m3
          double defIncome = 0.;

          g4m::ageStruct cohortTmp = *cohort_all[asID];
          cohortTmp.setRotPeriod(rotationForest.get(xi,yi));
          g4m::ageStruct::v res;  // MG: results vector for the set (old) forest 
          int Rotation = 0;
//          double forFlag = 0.;    //MG: a forest area for fitting existing forest in the grid: 0-no forest; 1 - 1 ha of normal forest
          
        if (iter->CABOVEHA.v() > 0 && maiForest.get(xi,yi)> 0) {
          biomasRot = fi.gU(iter->CABOVEHA.v(), maiForest.get(xi,yi), 1);        // rotation time to get current biomass (without thinning)
          biomasRotTh = fi.gUt(iter->CABOVEHA.v(), maiForest.get(xi,yi), thinningForest.get(xi,yi));     // rotation time to get current biomass (with thinning)     
            if (thinningForest.get(xi,yi) > 0) {
              Rotation = biomasRotTh+1; 
            }  else {
              Rotation = biomasRot+1;
            }
//          if (iter->FOREST"].v(1990) >0) forFlag = 1.0;
        }
        if (maiForest.get(xi,yi)> 0){
          rotMAI = fi.gTopt(maiForest.get(xi,yi), optimMAI);
//          rotMaxBm = fi.gTopt(maiForest.get(xi,yi), optimMaxBm);                        
          rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), optimMaxBmTh);
        }             

          res = cohortTmp.aging();
          sawnW = res.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
          restW = res.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
          sawnThW = res.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
          restThW = res.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
          harvWood = (sawnW + restW + sawnThW + restThW + resUse*res.hv) * iter->FTIMBER.v() ;
          if ((biomasRot ==0)||(harvWood < 0)) harvWood = 0.;
//          coeff.PriceC.insert(0,0.);
          dima decision(2000
		       , iter->NPP
		       , iter->SPOPDENS
		       , iter->SAGRSUIT
		       , iter->PRICEINDEX
		       , coeff.PriceIndexE
		       , iter->R
		       , coeff.PriceC
		       , coeff.PlantingCostsR
		       , coeff.PriceLandMinR
		       , coeff.PriceLandMaxR
		       , coeff.MaxRotInter
		       , coeff.MinRotInter
		       , coeff.decRateL
		       , coeff.decRateS
		       , iter->FRACLONGPROD
		       , coeff.baseline
		       , iter->FTIMBER 
		       , coeff.PriceTimberMaxR
		       , coeff.PriceTimberMinR
		       , coeff.FCuptake
		       , iter->GDP
		       , coeff.HarvLoos
		       , forestShare0
		       , wprice[regprice]
		       , wprice[regprice].v(2000)
               , rotMAI
               , harvMAI); 
//               , Rotation
//               , harvWood );
          abBiomassO = cohortTmp.getBm();
	      double pDefIncome = abBiomassO * 
                   (decision.priceTimber() * iter->FTIMBER.v() * (1. -coeff.HarvLoos.v()));
	      //Immediate Pay if deforested (Slash and Burn)
	      double sDefIncome = abBiomassO *
		           (decision.priceTimber() * iter->FTIMBER.v() 
		         * (1. -coeff.HarvLoos.v()));
	      defIncome = pDefIncome * (1. - iter->SLASHBURN.v())
		            + sDefIncome * iter->SLASHBURN.v();
		            
          harvestGrid.set(xi,yi,harvWood * forestArea0);   
          if (managedForest.get(xi,yi) > 0) {
//if (Country0 == 61){cout<<"woodHarvest= "<<woodHarvest[Country0-1]<<"\t 0.9 * woodHarvestStat= "<<0.9 * woodHarvestStat[Country0-1] <<endl;}
            if (woodHarvest[countryreg] < 0.95 * wprod[countryregprice].v(year)) {
              if (managedForest.get(xi,yi) >= 2) {
//	    		HarvestTmp = harvestGrid.get(xi,yi);
                rotationForest.set(xi,yi,rotMAI);		   
                cohortTmp.setRotPeriod(rotMAI);
                res = cohortTmp.aging();
                sawnW = res.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
                restW = res.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
                sawnThW = res.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
                restThW = res.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
                harvWood = (sawnW + restW + sawnThW + restThW + resUse*res.hv) * iter->FTIMBER.v() ;
                if ((biomasRot ==0)||(harvWood < 0)) harvWood = 0.;
			    harvestGrid.set(xi,yi,harvWood * forestArea0);
                woodHarvest[Country0-1] += harvWood * forestArea0;
              }            
            } else if (woodHarvest[Country0-1] > 1.05 * wprod[countryregprice].v(year)) {
//if (Country0 == 61){cout<<"woodHarvest= "<<woodHarvest[countryreg]<<"\t 1.1 * woodHarvestStat= "<<1.1 * woodHarvestStat[Country0-1] <<endl;}                   

                if (rotationForest.get(xi,yi) < rotMaxBmTh) {
//                HarvestTmp = harvestGrid.get(xi,yi);
			    rotationForest.set(xi,yi, rotMaxBmTh);
                cohortTmp.setRotPeriod(rotMaxBmTh);
                res = cohortTmp.aging();
                sawnW = res.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
                restW = res.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
                sawnThW = res.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
                restThW = res.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
                harvWood = (sawnW + restW + sawnThW + restThW  + resUse*res.hv) * iter->FTIMBER.v() ;
                if ((biomasRot ==0)||(harvWood < 0)) harvWood = 0.;
			    harvestGrid.set(xi,yi,harvWood * forestArea0);
                woodHarvest[countryreg] += harvWood * forestArea0;
              }
            } else { // keep biomass rotation
                res = cohortTmp.aging();
                sawnW = res.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
                restW = res.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
                sawnThW = res.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
                restThW = res.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
                harvWood = (sawnW + restW + sawnThW + restThW  + resUse*res.hv) * iter->FTIMBER.v() ;
                if ((biomasRot ==0)||(harvWood < 0)) harvWood = 0.;
			    harvestGrid.set(xi,yi,harvWood * forestArea0);
                woodHarvest[countryreg] += harvWood * forestArea0;
            }     
            managedCount[countryreg] +=1;                  
          }else { // unmanaged forests
                res = cohortTmp.aging();
                sawnW = res.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
                restW = res.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
                sawnThW = res.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
                restThW = res.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
                harvWood = (sawnW + restW + sawnThW + restThW  + resUse*res.hv) * iter->FTIMBER.v() ;
                if ((biomasRot ==0)||(harvWood < 0)) harvWood = 0.;

                woodLost[countryreg] += harvWood * forestArea0;
          }          
          
    } //else {iter++;}  // end for IF unprotected
   }   // Test only some regions
    iter++;
  } //end for WHILE
//************************End of First Pass************************************









//******************************************************************************
//**************************Second Pass********************
//******************************************************************************



  iter = data_all.begin();
  while (iter != data_all.end()) {
   if (regions.find(iter->POLESREG[2000]) != regions.end()) { // Test only some regions        
    if ((iter->PROTECT[2000]==0)&&((iter->POTVEG[2000]<10 && iter->FOREST[2000]>0 && iter->MAIE[2000]>0)||(iter->POTVEG[2000]<10 && (iter->NPP[2000]>0)||iter->MAIN[2000]>0))){   // We consider only unprotected land

          double HarvestTmp = 0;
          double newHarvestTmp = 0;
          double abBiomassO = 0.;  
            int asID = iter->asID;
            short int countryreg = (int)iter->COUNTRYREGMIX.v();
            short int region = iter->POLESREG[2000];
            string regprice = "re"+IntToStr(region)+"price0";
            string countryregprice = "re"+IntToStr(countryreg)+"price0";
            countryreg = (int)iter->COUNTRYREGMIX.v();
            
            short int Country0 = iter->COUNTRY[2000]; 
            int xi = (iter->x);
            int yi = (iter->y);
    
          int rotUnmanaged = 0;
          int rotMAI = 0;
          int rotMaxBm = 0;
          int rotMaxBmTh = 0;
          int rotHarvFin = 0;
          int rotHarvAve = 0;
          int biomasRot=0;  // MG: rotation time fitted to get certain biomass under certain MAI (w/o thinning)
          int biomasRotTh=0;  // MG: rotation time fitted to get certain biomass under certain MAI (with thinning)

          double LandAreaHa = iter->LANDAREA.v()*100;
          double forestShare0 = 0;
           if (iter->FOREST.v()+(iter->CROP.v())+(iter->BUILTUP.v())>1)
           {forestShare0 = (1-(iter->CROP.v()+iter->BUILTUP.v()));}
           else {forestShare0 = iter->FOREST.v();}
          double forestArea0 = LandAreaHa * forestShare0;
          double harvMAI = maiForest.get(xi,yi)*iter->FTIMBER.v()*(1-coeff.HarvLoos.v());
          double harvWood=0; //MG: harvestable wood, m3
          double defIncome = 0.;


          g4m::ageStruct cohortTmp = *cohort_all[asID];
          cohortTmp.setRotPeriod(rotationForest.get(xi,yi));

          g4m::ageStruct::v res;  // MG: results vector for the set (old) forest 
          int Rotation = 0;
//          double forFlag = 0.;    //MG: a forest area for fitting existing forest in the grid: 0-no forest; 1 - 1 ha of normal forest
          
        if (iter->CABOVEHA.v() > 0 && maiForest.get(xi,yi)> 0) {
          biomasRot = fi.gU(iter->CABOVEHA.v(), maiForest.get(xi,yi), 1);        // rotation time to get current biomass (without thinning)
          biomasRotTh = fi.gUt(iter->CABOVEHA.v(), maiForest.get(xi,yi), thinningForest.get(xi,yi));     // rotation time to get current biomass (with thinning)     
            if (thinningForest.get(xi,yi) > 0) {
              Rotation = biomasRotTh+1; 
            }  else {
              Rotation = biomasRot+1;
            }
//          if (iter->FOREST"].v(1990) >0) forFlag = 1.0;
        }
        if (maiForest.get(xi,yi)> 0){
          rotMAI = fi.gTopt(maiForest.get(xi,yi), optimMAI);
//          rotMaxBm = fi.gTopt(maiForest.get(xi,yi), optimMaxBm);                        
          rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), optimMaxBmTh);
        }  
        
        if (Rotation < rotMAI) Rotation = rotMAI;           
          
          res = cohortTmp.aging();
          sawnW = res.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
          restW = res.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
          sawnThW = res.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
          restThW = res.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
          harvWood = (sawnW + restW + sawnThW + restThW + resUse*res.hv) * iter->FTIMBER.v() ;
          if ((biomasRot ==0)||(harvWood < 0)) harvWood = 0.;
//          coeff.PriceC.insert(0,0.);
          dima decision(2000
		       , iter->NPP
		       , iter->SPOPDENS
		       , iter->SAGRSUIT
		       , iter->PRICEINDEX
		       , coeff.PriceIndexE
		       , iter->R
		       , coeff.PriceC
		       , coeff.PlantingCostsR
		       , coeff.PriceLandMinR
		       , coeff.PriceLandMaxR
		       , coeff.MaxRotInter
		       , coeff.MinRotInter
		       , coeff.decRateL
		       , coeff.decRateS
		       , iter->FRACLONGPROD
		       , coeff.baseline
		       , iter->FTIMBER
		       , coeff.PriceTimberMaxR
		       , coeff.PriceTimberMinR
		       , coeff.FCuptake
		       , iter->GDP
		       , coeff.HarvLoos
		       , forestShare0
		       , wprice[regprice]
		       , wprice[regprice].v(2000)
               , rotMAI
               , harvMAI); 
//               , Rotation
//               , harvWood );
          abBiomassO = cohortTmp.getBm();
	      double pDefIncome = abBiomassO * 
                   (decision.priceTimber() * iter->FTIMBER.v() * (1. -coeff.HarvLoos.v()));
	      //Immediate Pay if deforested (Slash and Burn)
	      double sDefIncome = abBiomassO *
		           (decision.priceTimber() * iter->FTIMBER.v() 
		         * (1. -coeff.HarvLoos.v()));
	      defIncome = pDefIncome * (1. - iter->SLASHBURN.v())
		            + sDefIncome * iter->SLASHBURN.v();
		            
//          harvestGrid.set(xi,yi,harvWood * forestArea0);   
          if (managedForest.get(xi,yi) > 0) {
//if (Country0 == 61){cout<<"woodHarvest= "<<woodHarvest[countryreg]<<"\t 0.9 * woodHarvestStat= "<<0.9 * woodHarvestStat[Country0-1] <<endl;}
            if (woodHarvest[countryreg] < 0.95 * wprod[countryregprice].v(year)) {
              if ((Rotation > rotMAI) && (rotationForest.get(xi,yi) > Rotation)) { 
	    		HarvestTmp = harvestGrid.get(xi,yi);
                rotationForest.set(xi,yi,Rotation);		   
                cohortTmp.setRotPeriod(Rotation);
                res = cohortTmp.aging();
                sawnW = res.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
                restW = res.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
                sawnThW = res.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
                restThW = res.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
                harvWood = (sawnW + restW + sawnThW + restThW + resUse*res.hv) * iter->FTIMBER.v() ;
                if ((biomasRot ==0)||(harvWood < 0)) harvWood = 0.;
         		newHarvestTmp = harvWood * forestArea0;
		     	harvestGrid.set(xi,yi,harvWood * forestArea0);
                woodHarvest[countryreg] += (newHarvestTmp-HarvestTmp);
              } else {//if (rotationForest.get(xi,yi) > rotMAI) { 
	    		HarvestTmp = harvestGrid.get(xi,yi);
                rotationForest.set(xi,yi,rotMAI);		   
                cohortTmp.setRotPeriod(rotMAI);
                res = cohortTmp.aging();
                sawnW = res.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
                restW = res.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
                sawnThW = res.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
                restThW = res.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
                harvWood = (sawnW + restW + sawnThW + restThW + resUse*res.hv) * iter->FTIMBER.v() ;
                if ((biomasRot ==0)||(harvWood < 0)) harvWood = 0.;
         		newHarvestTmp = harvWood * forestArea0;
		     	harvestGrid.set(xi,yi,newHarvestTmp);
                woodHarvest[countryreg] += (newHarvestTmp-HarvestTmp);
              }            
            } else if (woodHarvest[countryreg] > 1.05 * wprod[countryregprice].v(year)) {
//if (Country0 == 61){cout<<"woodHarvest= "<<woodHarvest[countryreg]<<"\t 1.1 * woodHarvestStat= "<<1.1 * woodHarvestStat[Country0-1] <<endl;}                   
              if (rotationForest.get(xi,yi) < Rotation) { 
	    		HarvestTmp = harvestGrid.get(xi,yi);
                rotationForest.set(xi,yi,Rotation);		   
                cohortTmp.setRotPeriod(Rotation);
                res = cohortTmp.aging();
                sawnW = res.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
                restW = res.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
                sawnThW = res.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
                restThW = res.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
                harvWood = (sawnW + restW + sawnThW + restThW + resUse*res.hv) * iter->FTIMBER.v() ;
                if ((biomasRot ==0)||(harvWood < 0)) harvWood = 0.;

         		newHarvestTmp = harvWood * forestArea0;
		     	harvestGrid.set(xi,yi,newHarvestTmp);
                woodHarvest[countryreg] += (newHarvestTmp-HarvestTmp);
              } else if (rotationForest.get(xi,yi) < rotMaxBmTh) {
                HarvestTmp = harvestGrid.get(xi,yi);
			    rotationForest.set(xi,yi, rotMaxBmTh);
                cohortTmp.setRotPeriod(rotMaxBmTh);
                res = cohortTmp.aging();
                sawnW = res.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
                restW = res.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
                sawnThW = res.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
                restThW = res.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
                harvWood = (sawnW + restW + sawnThW + restThW  + resUse*res.hv) * iter->FTIMBER.v() ;
                if ((biomasRot ==0)||(harvWood < 0)) harvWood = 0.;

  			    newHarvestTmp = harvWood * forestArea0;
			    harvestGrid.set(xi,yi,newHarvestTmp);
                woodHarvest[countryreg] += (newHarvestTmp-HarvestTmp);
              }
            }
          }
//          iter++;
//        } while (((iter-1)->COUNTRY[2000]) == ((iter)->COUNTRY[2000]));   // Check are we in the same country  // end for Within current country
//if (Country0 == 61){cout<<"woodHarvest= "<<woodHarvest[countryreg]<<"\twoodHarvestStat= "<<woodHarvestStat[Country0-1] <<endl;}
    } //else {iter++;}  // end for IF unprotected
   }   // Test only some regions
    iter++;
  } //end for WHILE
//************************End of Second Pass************************************

//cout << "end of second pass" << endl;

////
/////////////////////////////////////////////////////
//////
//////                   Third Pass
//////
/////////////////////////////////////////////////////
////
//  iter = data_all.begin();
//
////cout << "Putting data for current cell into conteiner... "<< endl;
//   while (iter != data_all.end())
//   {if (regions.find(iter->POLESREG[2000]) != regions.end()) { // Test only some regions
//	if (iter->PROTECT[2000] == 0)
//	 {
//        double HarvestTmp = 0;
//        double newHarvestTmp = 0;
//        double abBiomassO = 0.;  
//
//          int xi = (iter->x);
//          int yi = (iter->y);
//          int asID = iter->asID;
//          int Country0 = (int)iter->COUNTRY"].v();
//          int rotUnmanaged = 0;
//          int rotMAI = 0;
//          int rotMaxBm = 0;
//          int rotMaxBmTh = 0;
//          int rotHarvFin = 0;
//          int rotHarvAve = 0;
//          int biomasRot=0;  // MG: rotation time fitted to get certain biomass under certain MAI (w/o thinning)
//          int biomasRotTh=0;  // MG: rotation time fitted to get certain biomass under certain MAI (with thinning)
//          int biomasRotTh2=0;  // MG: rotation time fitted to get certain biomass under certain MAI (with thinning=2)
//
//          double LandAreaHa = iter->LANDAREA"].v()*100;
//          double forestArea0 = LandAreaHa * iter->FOREST"].v();
//          double harvMAI = maiForest.get(xi,yi)*iter->FTIMBER"].v()*(1-coeff.HarvLoos.v());
//          double harvWood=0; //MG: harvestable wood, m3
//          double defIncome = 0.;
//
//          
//          
//          
//          g4m::ageStruct cohortTmp = *cohort_all[asID];
//          cohortTmp.setRotPeriod(rotationForest.get(xi,yi));
//
// 
//        g4m::ageStruct::v res; // MG: results vector for the set (old) forest    
//// End of FM initialisation
//
//        double MAI = maiForest.get(xi,yi); //MG: mean annual increment in tC/ha/year
//
//        if (iter->CABOVEHA"].v() > 0 && maiForest.get(xi,yi)> 0) {
//          biomasRot = fi.gU(iter->CABOVEHA"].v(), MAI, 1);        // rotation time to get current biomass (without thinning)
//          biomasRotTh = fi.gUt(iter->CABOVEHA"].v(), MAI, thinningForest.get(xi,yi));     // rotation time to get current biomass (with thinning)     
//          biomasRotTh2 = fi.gUt(iter->CABOVEHA"].v(), MAI, 2);     // rotation time to get current biomass (with thinning)     
////          if (iter->FOREST"].v(1990) >0) forFlag = 1.0;
//        }
//        if (maiForest.get(xi,yi)> 0){
//          rotMAI = fi.gTopt(MAI, optimMAI);
//          rotMaxBm = fi.gTopt(MAI, optimMaxBm);                        
//          rotMaxBmTh = fi.gTopt(MAI, optimMaxBmTh);
//        }             
//
//        iter->PRICEC"].insert(0,0.);
//        dima decision((int)iter->BYEAR"].v()
//		       , iter->NPP"]
//		       , iter->SPOPDENS"]
//		       , iter->SAGRSUIT"]
//		       , iter->PRICEINDEX"]
//		       , coeff.PriceIndexE
//		       , iter->R"]
//		       , coeff.PriceC
//		       , coeff.PlantingCostsR
//		       , coeff.PriceLandMinR
//		       , coeff.PriceLandMaxR
//		       , coeff.MaxRotInter
//		       , coeff.MinRotInter
//		       , coeff.decRateL
//		       , coeff.decRateS
//		       , iter->FRACLONGPROD"]
//		       , coeff.baseline
//		       , iter->FTIMBER"] 
//		       , coeff.PriceTimberMaxR
//		       , coeff.PriceTimberMinR
//		       , coeff.FCuptake
//		       , iter->GDP"]
//		       , coeff.HarvLoos
//		       , iter->FOREST"].v((int)iter->BYEAR"].v())
//		       , wprice["regprice"]
//		       , wprice["regprice0"].v()
//               , rotMAI
//               , harvMAI);
//
////  g4m::ageStruct::v res; // MG: results vector for the set (old) forest    
// double Thinning = -1.;
// double ThinningInit = -1.;
// int Rotation = 0;
// int RotationInit = 0;
//
////if (Country0 == 61){cout<<"woodHarvest= "<<woodHarvest[countryreg]<<"\t 0.9 * woodHarvestStat= "<<0.9 * woodHarvestStat[Country0-1] <<endl;}
//
//if (woodHarvest[countryreg] < (0.95 * woodHarvestStat[Country0-1])) 
// {
////if (Country0 == 61){cout<<"Inside IF!!???<"<<endl;};
////  if ((managedForest.get(xi,yi)) <= 0 && (managedForest.get(xi,yi) > -2))
//  if ((managedForest.get(xi,yi)) <= 0)
//   {
//                HarvestTmp = harvestGrid.get(xi,yi);
//                ThinningInit = 1.;
//                RotationInit = biomasRotTh2+1;
//                rotationType.set(xi,yi,11);
//                
//                cohortTmp.setStockingdegree(2);
//                thinningForest.set(xi,yi,1.);
//                  
//     if (MAI > MAI_countryregmix_up_avg[countryreg])
//     {   
//         if ((decision.forValNC() * Hurdle_opt[Country0-1]) > (decision.agrVal() + defIncome))
//                  {
//                          managedForest.set(xi,yi,3.);
////                          Rotation = rotMAI;
//                          Rotation = RotationInit;                          
//                          rotationType.set(xi,yi,1);
//                    }else 
//                    {     managedForest.set(xi,yi,2.);
////                          Rotation = rotMaxBmTh;
//                          Rotation = RotationInit;
//                          rotationType.set(xi,yi,2);
//                     }
//       }else
//       {                  managedForest.set(xi,yi,2.);
////                          Rotation = rotMaxBmTh;
//                          Rotation = RotationInit;                          
//                          rotationType.set(xi,yi,3);
//        }
//       rotationForest.set(xi,yi,Rotation);	
//       cohortTmp.setRotPeriod(Rotation);
//          res = cohortTmp.aging();
//          sawnW = res.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
//          restW = res.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
//          sawnThW = res.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
//          restThW = res.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
//          harvWood = (sawnW + restW + sawnThW + restThW + resUse*res.hv) * iter->FTIMBER"].v();
//          if ((biomasRot ==0)||(harvWood < 0)) harvWood = 0.;
//
//   		newHarvestTmp = harvWood * forestArea0;        
//
//        harvestGrid.set(xi,yi,newHarvestTmp);   
//        woodHarvest[countryreg] += newHarvestTmp;
//        woodLost[Country0-1] -= HarvestTmp; 
//        }           
// }else if (woodHarvest[countryreg] > (1.05 * woodHarvestStat[Country0-1])) 
// {
////if (Country0 == 61){cout<<"woodHarvest= "<<woodHarvest[countryreg]<<"\t 1.1 * woodHarvestStat= "<<1.1 * woodHarvestStat[Country0-1] <<endl;}
////if (Country0 == 61){cout<<"Inside IF!!??? >"<<endl;};
//  if (thinningForest.get(xi,yi) > 0)
//   {
//	    		HarvestTmp = harvestGrid.get(xi,yi);
//	    		
//                ThinningInit = -1.;
//                thinningForest.set(xi,yi,-1.);
//                RotationInit = biomasRot+1;
//                rotationType.set(xi,yi,10);
//
//                cohortTmp.setStockingdegree(-1.);                 
//
//      if (MAI > MAI_countryregmix_up_avg[countryreg])
//       {   if ((decision.forValNC() * Hurdle_opt[Country0-1]) > (decision.agrVal() + defIncome))
//           {   
//                managedForest.set(xi,yi,0.);
////                 thinningForest.set(xi,yi,-1.);
//                 Rotation = biomasRot+1;
//                 rotationType.set(xi,yi,1);
//             
//            }else
//            {     
//                 managedForest.set(xi,yi,-1.);
//                  Rotation = biomasRot+1;
//                  rotationType.set(xi,yi,10);
//              
//            }
//       }else 
//       {  if ((decision.forValNC() * Hurdle_opt[Country0-1]) > (decision.agrVal() + defIncome))
//            {          managedForest.set(xi,yi,-1.);
////                   thinningForest.set(xi,yi,-1.);
//                   Rotation = biomasRot+1;
//                   rotationType.set(xi,yi,10);
//               
//             }else
//             {      
//                    managedForest.set(xi,yi,-2.);
////                    Rotation = biomasRot+1;
//                    Rotation = rotMaxBm;
//                    rotationType.set(xi,yi,10);
//               
//              }
//        }
//       rotationForest.set(xi,yi,Rotation);	
//       cohortTmp.setRotPeriod(Rotation);
////       cohort.setStockingdegree(thinningForest.get(xi,yi));
//          res = cohortTmp.aging();
//          sawnW = res.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
//          restW = res.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
//          sawnThW = res.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
//          restThW = res.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
//          harvWood = (sawnW + restW + sawnThW + restThW + resUse*res.hv) * iter->FTIMBER"].v();
//          if ((biomasRot ==0)||(harvWood < 0)) harvWood = 0.;
//   		newHarvestTmp = harvWood * forestArea0;        
//       
//        woodHarvest[countryreg] -= (HarvestTmp);
//        harvestGrid.set(xi,yi,0);  
//        woodLost[Country0-1] += newHarvestTmp; 
//      
// }
//}
////cout<< managedForest.get(xi,yi)<<endl;
//
//
///*  
//cout <<"country=\t"<<Country0<<"\t woodHarvest=\t"<< woodHarvest[countryreg];
//cout <<"\t rotation=\t"<< Rotation<<"\t thinning= \t"<<thinningForest.get(xi,yi);
//cout <<"\t sawnThW=\t"<<sawnThW*forestArea0*4<<"\t restThW=\t"<<restThW*forestArea0*4<<"\t";
//cout <<"\t sawnW= \t"<<sawnW*forestArea0*4<<"\t restW= \t"<<restW*forestArea0*4;
//cout << "\t sawnThWha=\t" << sawnThW << "\t restThWha=\t"<<restThW;
//cout <<"\t sawnWha= \t"<<sawnW<<"\t restWha= \t"<<restW<<"\t abbiomassO= \t"<< abBiomassO<<"\t abbiomass= \t"<<iter->BIOMASS"].v()<<"\t";
//cout <<endl;
//*/
////cout<< "managedForestSeting....\n";
//  
//   } //End for PROTECT == 0
//  } // Test only some regions 
//iter++;
// } // End for WHILE (cell loop) 
//
////    initLoop(0, data_all, fi, cohort_all, newCohort_all, dat_all, maiForest, thinningForest, rotationForest);
//    initCohorts(data_all, fi, cohort_all,maiForest, thinningForest, rotationForest); 
//cout << "Third pass is finished"<< endl;




//******************************************************************************
//**************************Forth Pass********************
//******************************************************************************
//cout << "Start forth pass" << endl;

   iter = data_all.begin();

 while (iter != data_all.end())
 {if (regions.find(iter->POLESREG[2000]) != regions.end()) { // Test only some regions
    if ((iter->PROTECT[2000]==0)&&((iter->POTVEG[2000]<10 && iter->FOREST[2000]>0 && iter->MAIE[2000]>0)||(iter->POTVEG[2000]<10 && (iter->NPP[2000]>0)||iter->MAIN[2000]>0))){   // We consider only unprotected land
	  double HarvestTmp = 0;
      double newHarvestTmp = 0;
      double abBiomassO = 0.;  	 
            int asID = iter->asID;
            short int countryreg = (int)iter->COUNTRYREGMIX.v();
            short int region = countryreg;
            string regprice = "re"+IntToStr(region)+"price0";
            countryreg = (int)iter->COUNTRYREGMIX.v();
            short int Country0 = iter->COUNTRY[2000]; 
            int xi = (iter->x);
            int yi = (iter->y);
      
       int biomasRot=0;  // MG: rotation time fitted to get certain biomass under certain MAI (w/o thinning)
       int biomasRotTh=0;  // MG: rotation time fitted to get certain biomass under certain MAI (with thinning)
       double harvWood=0; //MG: harvestable wood, m3
  
 
if (managedForest.get(xi,yi) > 0)
{ 
                       
        double LandAreaHa = iter->LANDAREA.v()*100;
        double forestShare0 = 0;
          if (iter->FOREST.v()+(iter->CROP.v())+(iter->BUILTUP.v())>1)
           {forestShare0 = (1-(iter->CROP.v()+iter->BUILTUP.v()));}
           else {forestShare0 = iter->FOREST.v();}
     
        double forestArea0 = LandAreaHa * forestShare0;

        int rotMAI = 0;
        int rotMaxBm = 0;
        int rotMaxBmTh = 0;
        int rotHarvFin = 0;
        int rotHarvAve = 0;

          g4m::ageStruct cohortTmp = *cohort_all[asID];
          cohortTmp.setRotPeriod(rotationForest.get(xi,yi));
//          cohortTmp.setStockingdegree(thinningForest.get(xi,yi));
 
        g4m::ageStruct::v res; // MG: results vector for the set (old) forest    
// End of FM initialisation



// double Thinning = 1.;
 int Rotation = 0;
 
        if (iter->CABOVEHA.v() > 0 && maiForest.get(xi,yi)> 0) {
          biomasRot = fi.gU(iter->CABOVEHA.v(), maiForest.get(xi,yi), 1);        // rotation time to get current biomass (without thinning)
          biomasRotTh = fi.gUt(iter->CABOVEHA.v(), maiForest.get(xi,yi), thinningForest.get(xi,yi));     // rotation time to get current biomass (with thinning)     
          if (thinningForest.get(xi,yi) > 0)
          {Rotation = biomasRotTh+1; 
          }else
          {Rotation = biomasRot+1;}
//          if (iter->FOREST"].v(1990) >0) forFlag = 1.0;
        }
        if (maiForest.get(xi,yi)> 0){
          rotMAI = fi.gTopt(maiForest.get(xi,yi), optimMAI);
//          rotMaxBm = fi.gTopt(maiForest.get(xi,yi), optimMaxBm);                        
          rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), optimMaxBmTh);
        }          
        if (Rotation < rotMAI) Rotation = rotMAI;   

          res = cohortTmp.aging();
          sawnW = res.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
          restW = res.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
          sawnThW = res.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
          restThW = res.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
          harvWood = (sawnW + restW + sawnThW + restThW + resUse*res.hv) * iter->FTIMBER.v();
          if ((biomasRot ==0)||(harvWood < 0)) harvWood = 0.;


//cout<<"x \t"<<xi<<"\t y \t"<<yi<<"\t thinning\t"<<thinningForest.get(xi,yi)<<"\t";
//cout<<"countryB=\t"<<Country0<<"\t harvest=\t"<<woodHarvest[countryreg];
//cout<<"\t 0.8harvestStat\t"<< 0.8 * woodHarvestStat[Country0-1]<<"\t 1.2harvestStat\t"<< 1.2 * woodHarvestStat[Country0-1];
//cout<<"\t biomassInit\t"<< iter->BIOMASS"].v()<<"\t biomassO\t"<< abBiomassO<<"\t rotationInit \t"<<Rotation<<endl;      

//if (Country0 == 61){cout<<"woodHarvest= "<<woodHarvest[countryreg]<<"\t 0.9 * woodHarvestStat= "<<0.9 * woodHarvestStat[Country0-1] <<endl;}
 if (woodHarvest[countryreg] < 0.95 * wprod[regprice].v(year)) 
{
///if (Country0 == 61){cout<<"Inside IF (4th path) < !!???"<<endl;};
//
//		   if ((Rotation > rotMAI) && (rotationForest.get(xi,yi) > Rotation))
//		    { 
//	    		HarvestTmp = harvestGrid.get(xi,yi);
//                rotationForest.set(xi,yi,Rotation);		   
//                cohort.setRotPeriod(Rotation);
//#include "growForest.cpp"
//         		newHarvestTmp = harvWood * forestArea0;
//		     	harvestGrid.set(xi,yi,harvWood * forestArea0);
////cout<<"x \t"<<xi<<"\t y \t"<<yi<<"\t thinning\t"<<thinningForest.get(xi,yi)<<"\t";
////cout<<"country=\t"<<Country0<<"\t harvest=\t"<<woodHarvest[countryreg]<<"\t 0.8harvestStat\t"<< 0.8 * woodHarvestStat[Country0-1]<<"\t \t \t biomassInit\t"<< iter->BIOMASS"].v()<<"\t biomassO\t"<< abBiomassO<<"\t rotation \t"<<Rotation<<endl;               
//                woodHarvest[countryreg] += (newHarvestTmp-HarvestTmp);
//
//            } else if (rotationForest.get(xi,yi) > rotMAI)
		   if (rotationForest.get(xi,yi) != rotMAI)
            { 
	    		HarvestTmp = harvestGrid.get(xi,yi);
                rotationForest.set(xi,yi,rotMAI);		   
                cohortTmp.setRotPeriod(rotMAI);
//---grow forest for one year
//#include "growForest.cpp"
          res = cohortTmp.aging();
          sawnW = res.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
          restW = res.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
          sawnThW = res.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
          restThW = res.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
          harvWood = (sawnW + restW + sawnThW + restThW + resUse*res.hv) * iter->FTIMBER.v();
          if ((biomasRot ==0)||(harvWood < 0)) harvWood = 0.;

         		newHarvestTmp = harvWood * forestArea0;
		     	harvestGrid.set(xi,yi,newHarvestTmp);
//cout<<"x \t"<<xi<<"\t y \t"<<yi<<"\t thinning\t"<<thinningForest.get(xi,yi)<<"\t";
//cout<<"country=\t"<<Country0<<"\t harvest=\t"<<woodHarvest[countryreg]<<"\t 0.8harvestStat\t"<< 0.8 * woodHarvestStat[Country0-1]<<"\t \t \t biomassInit\t"<< iter->BIOMASS"].v()<<"\t biomassO\t"<< abBiomassO<<"\t rotation \t"<<Rotation<<endl;               
                woodHarvest[countryreg] += (newHarvestTmp-HarvestTmp);

              }            
  

		  
 } else if (woodHarvest[countryreg] > 1.05 * wprod[regprice].v(year)) 
 {
//if (Country0 == 61){cout<<"woodHarvest= "<<woodHarvest[countryreg]<<"\t 1.1 * woodHarvestStat= "<<1.1 * woodHarvestStat[Country0-1] <<endl;}
//if (Country0 == 61){cout<<"Inside IF (4th path) >!!???"<<endl;};        
//		   if (rotationForest.get(xi,yi) < Rotation)
//		    { 
//	    		HarvestTmp = harvestGrid.get(xi,yi);
//                rotationForest.set(xi,yi,Rotation);		   
//                cohort.setRotPeriod(Rotation);
//#include "growForest.cpp"
//         		newHarvestTmp = harvWood * forestArea0;
//		     	harvestGrid.set(xi,yi,harvWood * forestArea0);
////cout<<"x \t"<<xi<<"\t y \t"<<yi<<"\t thinning\t"<<thinningForest.get(xi,yi)<<"\t";		     	
////cout<<"country=\t"<<Country0<<"\t harvest=\t"<<woodHarvest[countryreg]<<"\t 1.1harvestStat=\t"<< 1.1 * woodHarvestStat[Country0-1]<<"\t \t \t biomassInit\t"<< iter->BIOMASS"].v()<<"\t biomassO\t"<< abBiomassO<<"\t rotation \t"<<Rotation<<endl;               
//                woodHarvest[countryreg] += (newHarvestTmp-HarvestTmp);
//             } else if (rotationForest.get(xi,yi) < rotMaxBmTh)
           if (rotationForest.get(xi,yi) < rotMaxBmTh)
			 {    HarvestTmp = harvestGrid.get(xi,yi);
			      rotationForest.set(xi,yi, rotMaxBmTh);
                  cohortTmp.setRotPeriod(rotMaxBmTh);
//---grow forest for one year
//#include "growForest.cpp"
          res = cohortTmp.aging();
          sawnW = res.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
          restW = res.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
          sawnThW = res.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
          restThW = res.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
          harvWood = (sawnW + restW + sawnThW + restThW + resUse*res.hv) * iter->FTIMBER.v();
          if ((biomasRot ==0)||(harvWood < 0)) harvWood = 0.;

  			     newHarvestTmp = harvWood * forestArea0;
			     harvestGrid.set(xi,yi,newHarvestTmp);
//cout<<"x \t"<<xi<<"\t y \t"<<yi<<"\t thinning\t"<<thinningForest.get(xi,yi)<<"\t";			     
//cout<<"country=\t"<<Country0<<"\t harvest=\t"<<woodHarvest[countryreg]<<"\t 1.1harvestStat=\t"<< 1.1 * woodHarvestStat[Country0-1]<<"\t \t \t biomassInit\t"<< iter->BIOMASS"].v()<<"\t biomassO\t"<< abBiomassO<<"\t rotation \t"<<rotationForest.get(xi,yi)<<endl;               
                woodHarvest[countryreg] += (newHarvestTmp-HarvestTmp);
              }
 }
}

//cout<<"x \t"<<xi<<"\t y \t"<<yi<<"\t thinning\t"<<thinningForest.get(xi,yi)<<"\t";
//cout<<"countryE=\t"<<Country0<<"\t harvest=\t"<<woodHarvest[countryreg]<<"\t harvestStat=\t"<< woodHarvestStat[Country0-1]<<"\t \t \t biomassInit\t"<< iter->BIOMASS"].v()<<"\t biomassO\t"<< abBiomassO<<"\t rotationE \t"<<rotationForest.get(xi,yi);               
//cout <<"\t"<<sawnThW*forestArea0*4<<"\t"<<restThW*forestArea0*4<<"\t";
//cout <<sawnW*forestArea0*4<<"\t"<<restW*forestArea0*4;
//cout<<endl;

//cout <<rotationForest.get(xi,yi)<<"\t";
//cout <<sawnThW*forestArea0*4<<"\t"<<restThW*forestArea0*4<<"\t";
//cout <<sawnW*forestArea0*4<<"\t"<<restW*forestArea0*4;// <<"\t"<<abBiomassO*forestArea0<<"\t";
//cout << endl;

//iter++;
//cout<<"iterC="<<((iter-1)->COUNTRY[2000])<<"\t iterC+1="<<((iter)->COUNTRY[2000]) <<endl;
//} while (((iter-1)->COUNTRY[2000]) == ((iter)->COUNTRY[2000]));   // Check are we in the same country  // end for Within current country
//cout<<"iterS="<<((iter-1)->COUNTRY[2000])<<"\t iterCS+1="<<((iter)->COUNTRY[2000]) <<endl;

//cout<<"x \t"<<xi<<"\t y \t"<<yi<<"\t thinning\t"<<thinningForest.get(xi,yi)<<"\t";
//cout<<"countryE=\t"<<Country0<<"\t harvest=\t"<<woodHarvest[countryreg]<<"\t harvestStat=\t"<< woodHarvestStat[Country0-1]<<"\t \t \t biomassInit\t"<< iter->BIOMASS"].v()<<"\t biomassO\t"<< abBiomassO<<"\t rotationE \t"<<rotationForest.get(xi,yi);               
//cout<<endl;

//cout <<X<<"\t"<<Y<<"\t"<<Country0<<"\t"<<year<<"\t"
//if (Country0 == 61) 
//{cout<<"Country= "<<Country0<<"\t woodHarvestFinal= "<<woodHarvest[countryreg]<<"\t woodHarvestStat= "<<woodHarvestStat[Country0-1] <<endl;}

}// else{iter++;}  // end for IF unprotected
} // Test only some regions   
iter++;
} //end for WHILE

	
//************************End of Forth Pass************************************
//cout << "End of 4th pass"<<endl;



////************* TEST of INIT FM
//for (int i=1;i<=209;i++)
//{
//cout<<"Country= "<<i<<"\t managedCountInitFM = "<< managedCount[i-1]<<"\t woodHarvest = "<< woodHarvest[i-1]<<endl;
//}
//   iter = data_all.begin();
//
//   while (iter != data_all.end())
//   {
//
//  int Country0 = 1;
//  double HarvestTmp = 0;
//  double newHarvestTmp = 0;
////  double forestArea0 = 0.;
//  double abBiomassO = 0.;  	 
//  int xi = 0;
//  int yi = 0;
//  double X = 0.;
//  double Y = 0.;
//
//	   if (iter->PROTECT[2000]==0)    // We consider only unprotected land
//	   {
//
////cout<<Country0<<endl;       
////#include "countrySelect.cpp"
//      if (   (iter->COUNTRY[2000] == 11)
//          || (iter->COUNTRY[2000] == 25)
//          || (iter->COUNTRY[2000] == 33)        
//          || (iter->COUNTRY[2000] == 33)  
//          || (iter->COUNTRY[2000] == 61)
//          || (iter->COUNTRY[2000] == 62)
//          || (iter->COUNTRY[2000] == 69)             
//          || (iter->COUNTRY[2000] == 156)             
//          || (iter->COUNTRY[2000] == 165)             
//          || (iter->COUNTRY[2000] == 179)                     
//          || (iter->COUNTRY[2000] == 197) ) 
//        { // Test only some countries
//
//       int biomasRot=0;  // MG: rotation time fitted to get certain biomass under certain MAI (w/o thinning)
//       int biomasRotTh=0;  // MG: rotation time fitted to get certain biomass under certain MAI (with thinning)
//       double harvWood=0; //MG: harvestable wood, m3
//
//
//    	            xi = (iter->x);
//    	            yi = (iter->y);
//    	            X = (iter->x)*0.5+0.25-180;
//    	            Y = (iter->y)*0.5+0.25-90;
//    	          
//    	   //cout << "Xi = "<< xi <<"   Yi = "<< yi << endl;       
//if (managedForest.get(xi,yi) > 0)
//{ 
//                       
//       Country0 = (int)iter->COUNTRY"].v();
//       double LandAreaHa = iter->LANDAREA"].v()*100;
//       double forestArea0 = LandAreaHa * iter->FOREST"].v();
//
//
//        g4m::ipol<double,double> sws;  //Schnittholzanteil an Vfm // share of harvestable sawnwood per m3 (diameter, share)
//        g4m::ipol<double,double> hlv;  //1-Ernteverluste Vornutzung // loosses after first prefinal cut (diameter, share of harvesting loses) ?
//        g4m::ipol<double,double> hle;  //1-Ernteverluste Endnutzung // losses at final cut (diameter, share of harvesting loses)?
//        g4m::ipol<double,double> dbv;  //Dekungsbeitrag vornutzung   // income per m3 for thinning (diameter,income)
//        g4m::ipol<double,double> dbe;  //Dekungsbeitrag endnutzung   //  income per m3 for final harvest (diameter,income)
//
//        sws.insert(10, .0);
//        sws.insert(30, .6);
//        hlv.insert(0, .0);
//        hlv.insert(25, .7);
//        hle.insert(0, .0);
//        hle.insert(25, .7);
//        dbv.insert(0, 2);
//        dbe.insert(0, 3);
// 
//        g4m::ageStruct::v res; // MG: results vector for the set (old) forest    
//// End of FM initialisation
//
//
//
//// double Thinning = 1.;
// int Rotation = 0;
// 
//  double forFlag = 0.; //MG: a forest area for fitting existing forest in the grid: 0-no forest; 1 - 1 ha of normal forest
////  if (iter->FOREST"].v(1990) >0 && iter->BIOMASS"].v() > 0 && maiForest.get(xi,yi) > 0)
//  if (iter->FOREST"].v(1990) >0 && iter->CABOVEHA"].v() > 0 && maiForest.get(xi,yi) > 0)  
//  
//  {
//          biomasRot = fi.gU(iter->CABOVEHA"].v(), maiForest.get(xi,yi), 1);       // rotation time to get current biomass (without thinning)
//          biomasRotTh = fi.gUt(iter->CABOVEHA"].v(), maiForest.get(xi,yi), 1);     // rotation time to get current biomass (with thinning)  
//
//          if (thinningForest.get(xi,yi) == 1)
//          {Rotation = biomasRotTh+1; 
//          }else
//          {Rotation = biomasRot+1;
//          }
//
//          forFlag = 1.0;
//  }     
//
////cout << "\t Rotation1 = \t"<<Rotation<<endl;
//
//			g4m::ageStruct cohort(&fi, sws, hlv, hle, dbv, dbe, 0, 0, maiForest.get(xi,yi), 
//			                            Rotation, thinningForest.get(xi,yi),forFlag, 0.75); 
//
//cohort.aging();
//            cohort.setRotPeriod(rotationForest.get(xi,yi));
//
////---grow forest for one year
////#include "growForest.cpp"
//          res = cohort.aging();
//          sawnW = res.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
//          restW = res.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
//          sawnThW = res.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
//          restThW = res.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
//          harvWood = (sawnW + restW + sawnThW + restThW) * iter->FTIMBER"].v();
//          if ((biomasRot ==0)||(harvWood < 0)) harvWood = 0.;
//     CountriesWoodHarvestM3Year.inc(Country0,iter->BYEAR"].v(),harvWood*forestArea0);
////     CountriesWoodHarvestM3Year.inc(Country0,iter->BYEAR"].v(),harvWood); 
//}// end for if managed
//iter++;
//} else{iter++;}  // end for Country select
//
//} else{iter++;}  // end for IF unprotected
//	   
////int Nc=0;
//
//
//} //end for WHILE
///////////////////******   END of Test of init FM ************************

 } //END of void
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
 
void initLoop(int i, dataDetStruct &data_all, g4m::incrementTab &fi, ageStructVector &cohort_all, 
              ageStructVector &newCohort_all, datGlobal &dat_all, griddata2<double> &maiForest, 
              griddata2<double> &thinningForest, griddata2<double> &rotationForest) 

//void initLoop(int i, dataDetStruct &data_all, g4m::incrementTab &fi,  
//              datGlobal &dat_all, griddata2<double> &maiForest, 
//              griddata2<double> &thinningForest, griddata2<double> &rotationForest) 



 {
//cout << "Begin initLoop"<<endl;

//// Deleating cohorts
//    for (int k = 0; k < cohort_all.size();k++) {
//      delete cohort_all[k];
//    }
//    cohort_all.clear();
//
//    for (int k = 0; k < newCohort_all.size();k++) {
//      delete newCohort_all[k];
//    }
//    newCohort_all.clear();
    

//  double woodHarvest[209];
//  double woodLost[209];
//  double woodPot[209];
//
//  for (int i=0; i<=208; i++){
//    woodHarvest[i]=0.; 
//    woodLost[i]=0.;
//    woodPot[i]=0.;
//  }
// 
  int asID = 0;  // index in the ageStruct vector
  int kk=0;
  numAgeStruct = 0;

  map<int, vector<double> > ageStructData;
  {

//cout<<"before reading file..."<<endl;
//    ifstream fin("ageStructData.txt");
    string agestructFile = settings.inputPath + "\\" + "ageStructDataJRC_nc.txt";
    
    ifstream fin(agestructFile.c_str());
  if (!fin.is_open()) {
    cout << "Cannot read  " << agestructFile << endl;
    system("pause");
    exit(0);
  }
//cout<<"first line reading..."<<endl;    
    {string tmp; getline(fin, tmp);} //Skip first line
    int country;
    vector<double> ageShares;
//    ageShares.resize(9);
    ageShares.resize(16);
    while(fin.good() && !fin.eof()) {
      fin >> country;
      double sum=0.;
//      for(int i=0; i<9; ++i) {fin >> ageShares[i]; sum+=ageShares[i];}
      for(int i=0; i<16; ++i) {fin >> ageShares[i]; sum+=ageShares[i];}
      double add=0.;
//      if(sum <=0.) {sum=0.; add=1./9.;} else {sum = 1./sum;}
      if(sum <=0.) {sum=0.; add=1./16.;} else {sum = 1./sum;}
//      for(int i=0; i<9; ++i) {ageShares[i] = add + ageShares[i]*sum;}
      for(int i=0; i<16; ++i) {ageShares[i] = add + ageShares[i]*sum;}      
      ageStructData[country] = ageShares;
    }
  }



//cout<<"start cell loop"<<endl;
  dataDetStruct::iterator iter = data_all.begin();
  while (iter != data_all.end()) {
   if (regions.find(iter->POLESREG[2000]) != regions.end()) { // Test only some regions
    if ((iter->PROTECT[2000]==0)&&((iter->POTVEG[2000]<10 && iter->FOREST[2000]>0 && iter->MAIE[2000]>0)||(iter->POTVEG[2000]<10 && (iter->NPP[2000]>0)||iter->MAIN[2000]>0))){
        int xi = (iter->x);
        int yi = (iter->y);
        double X = (iter->x)*GridStepLon+GridStepLon/2-180;
        double Y = (iter->y)*GridStepLat+GridStepLat/2-90;
        int Country = (int)iter->COUNTRY.v();
        double LandAreaHa = iter->LANDAREA.v()*100;
//        int aveRot = 0;                  // Average rotation time from statistics
        int PriceCi = PriceCiS[i];
        int iprice=i+1;
        coeff.PriceC.clear();
        coeff.PriceC.insert(0, PriceCi * iter->CORRUPTION.v());
        double harvWood=0.; //MG: harvestable wood, m3
        double harvWoodLost=0.; // wood lost in unmanaged forests, tC/ha
        double harvWoodNew=0.; //MG: harvestable wood, m3 (new forest)
        double harvWoodLostNew=0.; // wood lost in unmanaged forests, tC/ha (new forest)

        int beyear = (int)coeff.bYear;
//        double maxForInit = 1-iter->BUILTUP.v(beyear)-iter->CROP.v(beyear);
//        if (maxForInit < 0) maxForInit = 0;
        double forestShare = 0;        //Actual forest share
          if (iter->FOREST.v()+(iter->CROP.v())+(iter->BUILTUP.v())>1)
           {forestShare = (1-(iter->CROP.v()+iter->BUILTUP.v()));}
           else {forestShare = iter->FOREST.v();}
        double forestArea0 = LandAreaHa * forestShare;
        double OforestShare = forestShare;
        double AforestShare = 0.;              //MG: Actual forest share
        double refForShare = forestShare;      //forest share of ref. year	
        double OfsNoPay = forestShare;         //MG: Forest share (deforested) without payment
        double AfsNoPay = 0.;                  //MG: Forest share (afforested) without payment
        double fsNoPay = OfsNoPay + AfsNoPay;  //MG: Forest share without payment
//        g4m::ipol<double,double> sws;  //Schnittholzanteil an Vfm // share of harvestable sawnwood per m3 (diameter, share)
//        g4m::ipol<double,double> hlv;   //1-Ernteverluste Vornutzung // loosses after first prefinal cut (diameter, share of harvesting loses) ?
//        g4m::ipol<double,double> hle;   //1-Ernteverluste Endnutzung // losses at final cut (diameter, share of harvesting loses)?
//        g4m::ipol<double,double> dbv;  //Dekungsbeitrag vornutzung   // income per m3 for thinning (diameter,income)
//        g4m::ipol<double,double> dbe;  //Dekungsbeitrag endnutzung   //  income per m3 for final harvest (diameter,income)
//        sws.insert(10, .0);
//        sws.insert(30, .6);
//        hlv.insert(0, .0);
//        hlv.insert(25, .7);
//        hle.insert(0, .0);
//        hle.insert(25, .7);
//        dbv.insert(0, 2);
//        dbe.insert(0, 3);

//        hlv.insert(7, 0.8*(1-countryLosses[Country-1]));
        hlv.insert(25, 0.8*(1-countryLosses[Country-1]));
        hlv.insert(50, 0.8*(1-0.7*countryLosses[Country-1]));        
//        hle.insert(7, (1-countryLosses[Country-1]));
        hle.insert(25, (1-countryLosses[Country-1]));        
        hle.insert(50, (1-0.7*countryLosses[Country-1]));        

        double forFlag = 0.; //MG: a forest area for fitting existing forest in the grid: 0-no forest; 1 - 1 ha of normal forest
        double sawnWpot = 0.;
        double restWpot = 0.;
        double sawnThWpot = 0.;
        double restThWpot = 0.;
        int optimumType = 3;
        int MAIRot = 0;  //MG: optimal rotation time (see above)
//        int optRotUnmanaged = 0;
        int rotationTimeCurr = 0; 
        int Rotation = 0;
        int rotMaxBm = 0;
        int rotMaxBmTh = 0;
        int biomasRot=0;  // MG: rotation time fitted to get certain biomass under certain MAI (w/o thinning)
        int biomasRotTh=0;  // MG: rotation time fitted to get certain biomass under certain MAI (with thinning)
//        if (refForShare >0 && iter->CABOVEHA"].v() > 0 && maiForest.get(xi,yi)> 0) {
        if (iter->CABOVEHA.v() > 0 && maiForest.get(xi,yi)> 0) {
          biomasRot = fi.gU(iter->CABOVEHA.v(), maiForest.get(xi,yi), 1);
//          biomasRotTh = fi.gUt(iter->CABOVEHA"].v(), maiForest.get(xi,yi), 1); // with thinning
          biomasRotTh = fi.gUt(iter->CABOVEHA.v(), maiForest.get(xi,yi), thinningForest.get(xi,yi)); // with thinning
          if (iter->FOREST.v()>0) forFlag = 1.0;
        }
        if (maiForest.get(xi,yi)> 0){
        MAIRot = fi.gTopt(maiForest.get(xi,yi), 1);
        rotMaxBm = fi.gTopt(maiForest.get(xi,yi), 2);                        
        rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), 3);
        }
        rotationTimeCurr = rotationForest.get(xi,yi);
        if (biomasRot < 0) biomasRot = 0;
        if (MAIRot < 0) MAIRot = 0;
//        if (optRotUnmanaged < 0) optRotUnmanaged = 0;
 //       if (aveRot < 0) aveRot = 0;
        if (thinningForest.get(xi,yi) >0) {
          Rotation = biomasRotTh+1; 
        } else {
          Rotation = biomasRot+1;
        }

// Existing (old forest)
//        g4m::ageStruct *cohort = new g4m::ageStruct(&fi, sws, hlv, hle, dbv, dbe, 0, 0, maiForest.get(xi,yi), Rotation, thinningForest.get(xi,yi),forFlag, 0.75);

     // Initialize age structure
//    g4m::ageStruct *cohort = new g4m::ageStruct(&fi, sws, hlv, hle, dbv, dbe, 0, 0, 0, 1, thinningForest.get(xi,yi),0, 0.75);
//    g4m::ageStruct *cohort = new g4m::ageStruct(&fi, sws, hlv, hle, dbv, dbe, 0, 0, 0, 1, thinningForest.get(xi,yi),0, 0.5);    
//    cohort->setMai(maiForest.get(xi,yi));
 bool realForest = false;
 if (iter->POTVEG[2000]<10 && iter->FOREST[2000]>0 && iter->MAIE[2000]>0){realForest = true;}    
 g4m::ageStruct *cohort = new g4m::ageStruct(&fi, sws, hlv, hle, dbv, dbe, 0, 0,maiForest.get(xi,yi), 1, thinningForest.get(xi,yi),0, 0.5,realForest);   
//cout<<"ageClasse0=\t"<<cohort->getAgeClasses()<<endl; 
 double abBiomass0 = 0.;
 if (iter->POTVEG[2000]<10 && iter->FOREST[2000]>0 && iter->MAIE[2000]>0){
  map<int, vector<double> >::iterator PageStructData = ageStructData.find(Country);
  if((PageStructData != ageStructData.end()) && thinningForest.get(xi,yi)>0 && forFlag > 0) {
//  if((PageStructData != ageStructData.end()) && thinningForest.get(xi,yi)>0 && forFlag > 1000) { // Don't use Age Struct Data!!!
//    cohort->createNormalForest(161, 0., 1.);
    cohort->createNormalForest(321, 0., 1.);
//    double ageBreaks[] = {10.,20.,40.,60.,80.,100.,120.,140.,999.};
//    double ageSize[] = {11.,10.,20.,20.,20.,20.,20.,20.,20.};
//    int ageGroups = 9;
    double ageBreaks[] = {10.,20.,30.,40.,50.,60.,70.,80.,90.,100.,110.,120.,130.,140.,150.,999.};
    double ageSize[] = {11.,10.,10.,10.,10.,10.,10.,10.,10.,10.,10.,10.,10.,10.,10.,10.};
    int ageGroups = 16;

//    int oldest = 0;
    int oldestAgeGroup = 0;
    int   oag = 0;    
    int ageGroup = 0;
    for(int i=0; i<161; ++i) {
      while(i>ageBreaks[ageGroup] && ageGroup+1 < ageGroups) {++ageGroup;}
      cohort->setArea(i,PageStructData->second[ageGroup]/ageSize[ageGroup]);
      if (PageStructData->second[ageGroup]>0) {oldestAgeGroup = ageGroup;}
    }
    double area = cohort->getArea();
    double biomass = cohort->getBm();
    if(area > 0. && biomass > 0.) {

//Tune age structure for current cell
    
//    if (biomass < iter->CABOVEHA"].v()){
    if (biomass < 0.95*iter->CABOVEHA.v()){                
       int young = 0;
       oag = oldestAgeGroup; // current oldest age group
//       while (biomass < iter->CABOVEHA"].v() && oag < 30){
       while (biomass < 0.95*iter->CABOVEHA.v() && oag < 30){                          
          if (ageSize[young]>0 && oag>young){   
             for (int i=0;i<10;++i) {
//               cohort->setArea(i+young*10,(PageStructData->second[young])/(ageSize[young]*2));
               double areaTmp = cohort->getArea(i+young*10+1);  
               cohort->setArea(i+young*10+1,areaTmp/2);
//               cohort->setArea(i+oag*10+1,(PageStructData->second[young])/(ageSize[young]*2));
               cohort->setArea(i+(oag+1)*10+1,areaTmp/2);
//               cohort->setBm(i+oldestAgeGroup+1, stockingDegree*cohort->getBm(i+oldestAgeGroup+1));
               biomass = cohort->getBm();
               }
           }   
         young +=1;
         oag+=1;
       }    

//     } else if (biomass > 3 * iter->CABOVEHA"].v()){/
//     } else if (biomass > 2.5 * iter->CABOVEHA"].v()){            
     } else if (biomass > 2 * iter->CABOVEHA.v()){        //v_24_11                
        int young = 0;
        oag = oldestAgeGroup;          
//        while (biomass > 3 * iter->CABOVEHA"].v() && oag >2){
        while (biomass > 2 * iter->CABOVEHA.v() && oag >2){              
           if (oag > young){
             if (ageSize[oag]>0 && ageSize[young]>0){      
              for (int i=0;i<ageSize[oag];++i) {
               double areaTmp_oag = cohort->getArea(i+oag*10+1);                    
               double areaTmp_young = cohort->getArea(i+young*10+1);
               cohort->setArea(i+oag*10+1,0.);
               cohort->setArea(i+young*10+1,(areaTmp_oag+areaTmp_young));
//               cohort->setBm(i+oldestAgeGroup+1, stockingDegree*cohort->getBm(i+oldestAgeGroup+1));
               biomass = cohort->getBm();
               }
             }
            } else {
              if (ageSize[oag]>0 && ageSize[oag-1]>0){      
               for (int i=0;i<ageSize[oag];++i) {
               double areaTmp_oag = cohort->getArea(i+oag*10+1);                    
               double areaTmp_young = cohort->getArea(i+(oag-1)*10+1);                   
               cohort->setArea(i+oag*10+1,0.);
               cohort->setArea(i+(oag-1)*10+1,(areaTmp_oag+areaTmp_young));
//               cohort->setBm(i+oldestAgeGroup+1, stockingDegree*cohort->getBm(i+oldestAgeGroup+1));
               biomass = cohort->getBm();
               }
              }       
            }    
          young +=1;
          oag-=1;
         }    
//      double stockingDegree = iter->CABOVEHA"].v()/(cohort->getBm()/cohort->getArea());
//cout<< "countryIn = "<<Country<<"\tarea = "<<cohort->getArea()<<"\tbm = "<<cohort->getBm()<<"\tabC= "<<iter->CABOVEHA"].v()<<"\t stockingDegree = "<<stockingDegree<<"\t Th= "<<thinningForest.get(xi,yi)<<"\t oldestAgeGroup= "<<oldestAgeGroup<<"\t oag= "<<oag<<endl;
      }  

      double stockingDegree = iter->CABOVEHA.v()/(cohort->getBm()/cohort->getArea());
      if(stockingDegree < 0.) {stockingDegree = 0.;}

                 
//         cohort->setArea(i,PageStructData->second[ageGroup]/ageSize[ageGroup]);
//cout<< "countryIn = "<<Country<<"\tarea = "<<cohort->getArea()<<"\tbm = "<<cohort->getBm()<<"\tabC= "<<iter->CABOVEHA"].v()<<"\t Th= "<<thinningForest.get(xi,yi)<<endl;
//cout<< "countryIn = "<<Country<<"\tarea = "<<cohort->getArea()<<"\tbm = "<<cohort->getBm()<<"\tabC= "<<iter->CABOVEHA"].v()<<"\t stockingDegree = "<<stockingDegree<<"\t Th= "<<thinningForest.get(xi,yi)<<"\t oldestAgeGroup= "<<oldestAgeGroup<<"\t oag= "<<oag<<endl;
//      cohort->setStockingdegree(thinningForest.get(xi,yi));
      cohort->setStockingdegree(stockingDegree);
      thinningForest.set(xi,yi, stockingDegree);
      for(int i=0; i<321; ++i) {
      cohort->setBm(i, stockingDegree*cohort->getBm(i));
      }
//cout<< "countryIn = "<<Country<<"\tarea = "<<cohort->getArea()<<"\tbm = "<<cohort->getBm()<<"\tabC= "<<iter->CABOVEHA"].v()<<"\t stockingDegree = "<<stockingDegree<<"\t Th= "<<thinningForest.get(xi,yi)<<"\t oldestAgeGroup= "<<oldestAgeGroup<<"\t oag= "<<oag<<endl;
      cohort->setRotPeriod(321);
      cohort->aging();
//cout<< "countryIn = "<<Country<<"\tarea = "<<cohort->getArea()<<"\tbm = "<<cohort->getBm()<<"\tabC= "<<iter->CABOVEHA"].v()<<"\t stockingDegree = "<<stockingDegree<<"\t Th= "<<thinningForest.get(xi,yi)<<"\t oldestAgeGroup= "<<oldestAgeGroup<<"\t oag= "<<oag<<endl;
    }
         
    
//    cohort->setRotPeriod(fi.gUt(cohort->getBm()/cohort->getArea(), maiForest.get(xi,yi), 1));
    Rotation = fi.gUt(cohort->getBm()/cohort->getArea(), maiForest.get(xi,yi), thinningForest.get(xi,yi))+1;
    if (Rotation < MAIRot) Rotation = MAIRot;
    cohort->setRotPeriod(Rotation);  
//    cohort->setRotPeriod(rotationForest.get(xi,yi));
//      rotationForest.set(xi,yi,Rotation);
  } else {
    cohort->createNormalForest(Rotation, forFlag, thinningForest.get(xi,yi));
    cohort->setStockingdegree(thinningForest.get(xi,yi));         
    cohort->setRotPeriod(Rotation);
    cohort->aging();    
    if (Rotation < MAIRot) Rotation = MAIRot;
    cohort->setRotPeriod(Rotation);    
  }            
        
  rotationForest.set(xi,yi,Rotation);      
        
        
        
        g4m::ageStruct::v res;    // MG: results vector for the set (old) forest
        res = cohort->aging();
//        res = cohort.aging();        
        sawnWpot = res.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
        restWpot = res.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
        sawnThWpot = res.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
        restThWpot = res.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
        double potHarvest = (sawnWpot + restWpot + sawnThWpot + restThWpot + resUse*res.hv) * iter->FTIMBER.v(); // Potential sustainable harvest, m3/year

        abBiomass0 = cohort->getBm(); // modelled biomass at time 0, tC/ha
//        cohort->setRotPeriod(rotationTimeCurr);
 } //End for FOREST>0
////********Testing harvest
//        res = cohort->aging();
//  double sawnW = 0.;
//  double restW = 0.;
//  double sawnThW = 0.;
//  double restThW = 0.;
//  double sawnWlost = 0.;
//  double restWlost = 0.;
//  double sawnThWlost = 0.;
//  double restThWlost = 0.;
//
//    if (thinningForest.get(xi,yi)>0) {  
//    sawnW = res.enSw ;           // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
//    restW = res.enRw ;           // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
//    sawnThW = res.vnSw ;         // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
//    restThW = res.vnRw ;         // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
//    woodPot[Country-1]+=potHarvest * forestArea0;
//  } else {
//    sawnWlost = res.enSw ;           // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
//    restWlost = res.enRw ;           // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
//    sawnThWlost = res.vnSw ;         // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
//    restThWlost = res.vnRw ;         // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
//  }
//  harvWood = (sawnW + restW + sawnThW + restThW) * iter->FTIMBER"].v(); // Total current harvested wood in the cell, m3
//  harvWoodLost = (sawnWlost + restWlost + sawnThWlost + restThWlost); // Total current "lost" wood in the cell, tC (in remote forests)
//  woodHarvest[Country-1]+=harvWood * forestArea0;
//  woodLost[Country-1]+=harvWoodLost * forestArea0;
//  woodPot[Country-1]+=potHarvest * forestArea0;
////********************************************************


// New forest (planted/afforested)
//        g4m::ageStruct *newCohort = new g4m::ageStruct(&fi, sws, hlv, hle, dbv, dbe, 0, 0, maiForest.get(xi,yi), Rotation, thinningForest.get(xi,yi),0, 0.75); 
//        g4m::ageStruct::v newRes; // MG: results vector for the new (planted/afforested) forest
        bool realForest_new = false;
        if (iter->POTVEG[2000]<9 && iter->POTVEG[2000]>0 && iter->NPP[2000]>0){realForest_new = true;;}
        g4m::ageStruct *newCohort = new g4m::ageStruct(&fi, sws, hlv, hle, dbv, dbe, 0, 0, 10, 1, 0,0, 0.75,realForest_new); //MG: age of new forest is limited to 120 years!!

//cout<<"ageClassesNew0=\t"<<newCohort->getAgeClasses()<<endl;
        if (iter->POTVEG[2000]<9 && iter->POTVEG[2000]>0 && iter->NPP[2000]>0){
            newCohort->setMai(maiForest.get(xi,yi));                     
            newCohort->createNormalForest(Rotation, 0, thinningForest.get(xi,yi));
            newCohort->setStockingdegree(thinningForest.get(xi,yi));         
            newCohort->setRotPeriod(Rotation);
//            newCohort->aging();                                     
//cout<<"ageClassesNew=\t"<<newCohort->getAgeClasses()<<endl;
        }        

//cout<<"start singleCell"<<endl;
        dat singleCell;
//************************************
//**** Element for preparing output for GUI 
//************************************
		singleCell.x = X;
		singleCell.y = Y;
//cout<<"X=\t"<<X<<"\t Y=\t"<<Y<<"\t simUnit=\t"<<sMap.getSIMU(X,Y)<<endl;
//        singleCell.Rotation = Rotation;
        singleCell.Rotation = rotationForest.get(xi,yi);
        singleCell.LandAreaHa = LandAreaHa;
//        singleCell.potHarvest = potHarvest;
        singleCell.potHarvest = 0;
        singleCell.forestShare = forestShare;
        singleCell.OforestShare = OforestShare;
        singleCell.AforestShare = AforestShare;
        singleCell.prevOForShare = refForShare; //MG: Old Forest share in the previous reporting year
        singleCell.prevOForShareRP = OforestShare; //MG: Old Forest share in the previous reporting year
        singleCell.prevAForShareRP = 0.; //MG: New (afforested) Forest share in the previous reporting year
        singleCell.AforestSharePrev = 0.;
	    singleCell.savedCarbonPrev = 0.;
        singleCell.gainedCarbonPrev = 0.;
        singleCell.EmissionsTotPrev = 0.;
        singleCell.EmissionsAfforPrev = 0.;
        singleCell.prevPlantPhytHaBmGr = 0.;
        singleCell.prevPlantPhytHaBlGr = 0.;
        singleCell.deforestHaTot=0.;
        singleCell.afforestHaTot=0.;
        singleCell.EmissionsProduct= 0.;  
        singleCell.EmissionsLitter = 0.;  
        singleCell.EmissionsSOC = 0.;      
        singleCell.EmissionsSlashBurn = 0.;
        singleCell.EmissionsDeadBurn = 0.;
        singleCell.EmissionsCRootBurn = 0.;    
        singleCell.EmissionsTot = 0.;     
        singleCell.EmLitterAffor =0.;
        singleCell.EmSOCAffor = 0.; 
        singleCell.EmissionsAffor = 0.;
//        singleCell.ObiomassPrev = iter->CABOVEHA"].v();
        singleCell.ObiomassPrev = abBiomass0;
        singleCell.Obiomass0 = abBiomass0;                   // Modelled biomass at time 0, tC/ha
//        singleCell.rotBiomass = rotationForest.get(xi,yi);
        singleCell.rotBiomass = Rotation;
        singleCell.SD = thinningForest.get(xi,yi);
        for (int i=0; i<110; ++i) {
          singleCell.forestAgeShare[i] = 0.;
          singleCell.BDeadA[i]=0.;
          singleCell.LitterA[i]=0.;
          singleCell.SOCA[i]=0;
          singleCell.ProdLongA[i]=0.;
          singleCell.ProdShortA[i]=0.;
          singleCell.deforestA[i]=0;
          singleCell.FineRootA[i]=0;
          singleCell.LitterAffor[i]=0.;
          singleCell.SOCaffor[i]=0.;
        }
        singleCell.deforWoodTotM3 = 0.;// stem wood obtained from deforestation averaged for last 5 years        
        singleCell.deforestShare = 0.;//                 
        singleCell.afforestShare = 0.;//  
        singleCell.afforestHaYear = 0.;
        singleCell.deforestHaYear = 0.;
        singleCell.harvestTot = 0.;
        singleCell.harvestFcM3Ha = 0.;
        singleCell.harvestThM3Ha = 0.;
                               
        for (int i=0;i<5;i++) {singleCell.deforWoodArray[i] = 0.;
                              singleCell.deforAreaArray[i] = 0.;}
        singleCell.prevReportYear = coeff.bYear;
        singleCell.ireportYear=0;
// saving results to initial vectors
        cohort_all.push_back(cohort);
        newCohort_all.push_back(newCohort);
//cout<<"push_back singleCell"<<"\t asID"<<asID<<endl;
        dat_all.push_back(singleCell);
        iter->asID = asID;
        asID++;
        numAgeStruct++;
//if (thinningForest.get(xi,yi)>0){
//     CountriesManagedForHa.inc(Country,iter->BYEAR"].v(),OforestShare*singleCell.LandAreaHa);
//     CountriesManagedCount.inc(Country,iter->BYEAR"].v(),1);
//     }
//          
////     CountriesWoodHarvestM3Year.inc(Country,iter->BYEAR"].v(),(harvWood*OforestShare+harvWoodNew*singleCell.AforestSharePrev)*singleCell.LandAreaHa);
//     CountriesWoodLoosCYear.inc(Country,iter->BYEAR"].v(),(harvWoodLost*OforestShare+harvWoodLostNew*singleCell.AforestSharePrev)*singleCell.LandAreaHa);

//if (asID % 1000 == 0) cout << "asID = " << asID << endl;
//if (asID >=32765) cout << "asID = " << asID << "\txi= "<<xi<<"\t yi= "<<yi<< "\tx= "<<X<<"\t y= "<<Y<< endl;
    }     // End for IF unprotected 
  }      // Test only some regions 
    iter++;
kk++;
//if (kk % 100 == 0)
//    cout << "kk = " << kk << endl;
  }       // End of WHILE cells loop
//for (int i=1;i<=209;i++){
//cout<<"Country= "<<i<<"\t harvWoodM3 = "<< woodHarvest[i-1]<<"\t woodPotM3 = "<< woodPot[i-1]<<"\t harvLostC = "<< woodLost[i-1]<<endl;
//}
//cout<< "End initLoop"<<endl;
 }
