#ifndef fm_cpol_cpp
#define fm_cpol_cpp

 void fm_cpol(dataDetStruct &data_all, g4m::incrementTab &fi, ageStructVector &cohort_all, 
              ageStructVector &newCohort_all, datGlobal &dat_all, griddata &maiForest, 
              griddata &thinningForest, griddata &rotationForest, griddata2<char> &managedForest,
              griddata &rotationForestNew, griddata &thinningForestNew, griddata2<char> &manageChForest,
              griddata2<char> &rotationType, griddata &harvestGrid, int year, griddata2<char> &unmanaged, 
              double fm_hurdle, double &maxDiff, double priceC,
              set<int> &useChange, griddata &thinningForestO,griddata2<int> &rotationForestO,griddata2<int> &rotationForestNewO) 
 
 
 {
  
  double woodHarvest[NumberOfCountryregmix];
  woodHarvest[0]=0.;
  for (int i=1; i<NumberOfCountryregmix; i++){
    woodHarvest[i]= 0.;
//    woodHarvest[i]=CountriesWoodHarvestPlusM3Year.get(i, year-1); 
  }

 dataDetStruct::iterator iter = data_all.begin();

while (iter != data_all.end()) {
  short int region = iter->POLESREG[2000];
  if (regions.find(region) != regions.end()) { // Test only some regions         
     short int country = iter->COUNTRY[2000];
     if (toAdjust.find(country) != toAdjust.end()) { // Test only some countries         
       int asID = iter->asID;
       if (iter->PROTECT[2000]==0) {
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
//if (country==47){cout<<"thinningForest(FM100B)\t"<<thinningForest.get(xi,yi)<<"\tmanagedForest\t"<<int(managedForest.get(xi,yi))<<endl; }         
//          if (dat_all[asID].ObiomassPrev >0 && iter->CABOVEHA.v() > 0 && maiForest.get(xi,yi) > 0 && thinningForest.get(xi,yi) >0)  
//          if (maiForest.get(xi,yi) > 0 && thinningForest.get(xi,yi) >0)  
          if (thinningForest.get(xi,yi) >0)  
          {
//                double biomassRotTh = fi.gUt(dat_all[asID].ObiomassPrev, maiForest.get(xi,yi), thinningForest.get(xi,yi));     // rotation time to get current biomass (without thinning)  
                g4m::ageStruct cohortTmp = *cohort_all[asID];
                g4m::ageStruct cohortTmpNew = *newCohort_all[asID];
//            	harvestTmp = harvestGrid.get(xi,yi);
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

                newHarvestTmp = ((sawnW + restW + sawnThW + restThW + resUse*resTmp.hv) * (dat_all[asID].OforestShare-dat_all[asID].deforestShare) +
                             (sawnWnew + restWnew + sawnThWnew + restThWnew +resUse*resTmpNew.hv) * dat_all[asID].AforestShare) * 
                              dat_all[asID].LandAreaHa * iter->FTIMBER.v() + dat_all[asID].deforWoodTotM3; // Total current harvested wood in the cell, m3
                 woodHarvest[country] += (newHarvestTmp);
                 harvestGrid.set(xi,yi,newHarvestTmp);                       
//cout<<"countryFM = \t"<<country<<"\t deforWoodTotM3 = \t"<<dat_all[asID].deforWoodTotM3<<endl;       
           }else{
                 woodHarvest[country] += dat_all[asID].deforWoodTotM3;
                 harvestGrid.set(xi,yi,dat_all[asID].deforWoodTotM3); 
           } 
         }
      }
   }
  iter++;
}
//if (toAdjust.size()>1){ 
//cout<<"year=\t"<<year<<"\t inputHarvest(11)=\t"<<woodHarvest[11]<<endl;
//cout<<"year=\t"<<year<<"\t inputHarvest(47)=\t"<<woodHarvest[47]<<endl;
//}

//if (year!=2034){
if ((fm_hurdle == 1)&&(toAdjust.size()>1)){
//if (fm_hurdle == 1){
//if (toAdjust.size()>1 && year == refYear+1){              
//if (toAdjust.size()>1){                                    
 iter = data_all.begin();
 while (iter != data_all.end())
 {
  short int region = iter->POLESREG[2000];
  if (regions.find(region) != regions.end()) { // Test only some regions         
     short int country = iter->COUNTRY[2000];
     if (toAdjust.find(country) != toAdjust.end()) {         
      if (iter->PROTECT[2000] == 0)
      {
        int asID = iter->asID;
        char regionch[2];
        int2str(region,regionch);
        string regprice = "re"+string(regionch)+"price0";
        string regprice0 = "re"+string(regionch)+"price0";
        int xi = (iter->x);
        int yi = (iter->y);                    

        if (managedForest.get(xi,yi)>0 && maiForest.get(xi,yi)>0 && dat_all[asID].OforestShare > 0 )
        {
          double rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), 3);
//          double rotMaxBm = fi.gTopt(maiForest.get(xi,yi), 2);
          double maiV = maiForest.get(xi,yi)*iter->FTIMBER.v();
          double harvestTmp = harvestGrid.get(xi,yi);  

          double forestShare = dat_all[asID].OforestShare;          
          double NPVcw = 0.;   	
//          double NPVc = 0.;   	

          coeff.PriceC.clear();
          coeff.PriceC.insert(0, priceC * iter->CORRUPTION.v());

          {
          g4m::ageStruct cohortTmp = *cohort_all[asID];
          cohortTmp.setRotPeriod(rotMaxBmTh);
          NPVcw = npv_calc(*iter, cohortTmp, maiV,year,rotMaxBmTh,regprice,regprice0,forestShare,1)*forestShare*dat_all[asID].LandAreaHa;
          }                         

//          {
//          short int used = 0;                                    
//          g4m::ageStruct cohortTmp = *cohort_all[asID];
//          cohortTmp.setStockingdegree(-1.);
//          cohortTmp.setRotPeriod(rotMaxBm);
//          NPVc = npv_calc(*iter, cohortTmp, maiV,year,rotMaxBm,regprice,regprice0,forestShare,0)*forestShare*dat_all[asID].LandAreaHa;
//          }

//cout<< "asID ="<<"\t"<<asID<<"\t"<< "country ="<<"\t"<<country<<"\t"<<"year =\t"<<year<< "\tNPVcw = "<<"\t"<<NPVcw<<"\t"<< "NPVc = "<<"\t"<<NPVc<<"\t"<<"NPVbau =" << "\t" << NPVbau[year-refYear-1][asID] <<endl;
 
            if (NPVcw > NPVbau[year-refYear-1][asID])                             
             {
                   if (rotationForest.get(xi,yi) < rotMaxBmTh && NPVcw >= 0)
//                     if (rotationForest.get(xi,yi) < rotMaxBmTh)
                     {
//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t < rotMaxBmTh before"<<endl;}           	                   	                                                                    
                           double rotationTmpD = 1;
                           int rotationTmp = 1;
if (rotationForest.get(xi,yi)<0 || rotationTmpD<0){
cout<<"before\t"<<"ID=\t"<<asID<<"\tyear=\t"<<year<<"\t RL=\t"<<rotationForest.get(xi,yi)<<endl;}
                           
                           if (NPVbau[year-refYear-1][asID] > 0.){

                                  rotationTmpD = rotationForest.get(xi,yi) + 10 * (1+((NPVcw-NPVbau[year-refYear-1][asID])/(NPVbau[year-refYear-1][asID])));

if (rotationTmpD<0 || rotationForest.get(xi,yi)<0){
cout<<"after1\t"<<"ID=\t"<<asID<<"\tyear=\t"<<year<<"\t RL=\t"<<rotationForest.get(xi,yi)<<endl;
cout<<"rotMaxBmTh=\t"<<rotMaxBmTh<<"\trotationTmp=\t"<<rotationTmpD<<"\tNPVbau=\t"<<NPVbau[year-refYear-1][asID]<<endl;
cout<<"NPVcw=\t"<<NPVcw<<"\t10*(1+abs(..=\t"<<10 * (1+abs((NPVcw-NPVbau[year-refYear-1][asID])/NPVbau[year-refYear-1][asID]))<<endl;

cout<<"---"<<endl;
}
                           }else if (NPVbau[year-refYear-1][asID] < 0.){
                                  rotationTmpD = rotationForest.get(xi,yi) + 10 * (1+((NPVcw-NPVbau[year-refYear-1][asID])/(NPVbau[year-refYear-1][asID])));
   
                           }else{rotationTmpD = rotationForest.get(xi,yi) + 15;}

if (rotationTmp<0 || rotationForest.get(xi,yi)<0){
cout<<"after2\t"<<"ID=\t"<<asID<<"\tyear=\t"<<year<<"\t RL=\t"<<rotationForest.get(xi,yi)<<endl;
cout<<"rotMaxBmTh=\t"<<rotMaxBmTh<<"\trotationTmp=\t"<<rotationTmp<<"\tNPVbau=\t"<<NPVbau[year-refYear-1][asID]<<endl;
cout<<"NPVcw=\t"<<NPVcw<<"\t10*(1+abs(..=\t"<<10 * (1+abs((NPVcw-NPVbau[year-refYear-1][asID])/NPVbau[year-refYear-1][asID]))<<endl;
}   



//                      int rotationTmp = rotationForest.get(xi,yi) * 1.2;
                      if (rotationTmpD>rotMaxBmTh) {rotationTmp = rotMaxBmTh;}else{rotationTmp=int(rotationTmpD);}
                      rotationForest.set(xi,yi,rotationTmp);
//                      rotationForest.set(xi,yi,rotMaxBmTh);
                      cohort_all[asID]->setRotPeriod(rotationTmp);
                      g4m::ageStruct cohortTmp = *cohort_all[asID];
                      cohortTmp.setRotPeriod(rotationTmp);
                     
                      g4m::ageStruct cohortTmpNew = *newCohort_all[asID];                 
                      int newForAge = newCohort_all[asID]->getActiveAge();
                      int rotationNewTmp = rotationForestNew.get(xi,yi);
                      if (newForAge > rotationNewTmp){
//                         rotationNewTmp+=20;
//                         if (rotationNewTmp>rotMaxBmTh) {rotationNewTmp = rotMaxBmTh;}
                         newCohort_all[asID]->setRotPeriod(rotationTmp);
                         cohortTmpNew.setRotPeriod(rotationTmp);
                         rotationForestNew.set(xi,yi,rotationTmp);
                         }
                         
                      g4m::ageStruct::v resTmp = cohortTmp.aging();
                      double sawnW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
                      double restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
                      double sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
                      double restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
    
                      g4m::ageStruct::v resTmpNew = cohortTmpNew.aging();
                      double sawnWnew = resTmpNew.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
                      double restWnew = resTmpNew.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
                      double sawnThWnew = resTmpNew.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
                      double restThWnew = resTmpNew.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
                      double newHarvestTmp_cw = ((sawnW + restW + sawnThW + restThW + resUse*resTmp.hv) * (forestShare-dat_all[asID].deforestShare)  +
                                 (sawnWnew + restWnew + sawnThWnew + restThWnew +resUse*resTmpNew.hv) * dat_all[asID].AforestShare) * 
                                  dat_all[asID].LandAreaHa * iter->FTIMBER.v() + dat_all[asID].deforWoodTotM3; // Total current harvested wood in the cell, m3
                      woodHarvest[country] += (newHarvestTmp_cw - harvestTmp);
                      harvestGrid.set(xi,yi,newHarvestTmp_cw);                                                                                        
//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<newHarvestTmp_cw<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t < rotMaxBmTh after"<<endl;}           	                   	                                                                    
//                      double NPV50 = npv_calc(*iter, cohortTmp, maiV,year,rotationTmp,regprice,regprice0,forestShare,0)*forestShare*dat_all[asID].LandAreaHa;
                      NPVcGrid.set(xi,yi,NPVcw);
                      } else 
                      {     //g4m::ageStruct cohortTmp = *cohort_all[asID];
//                            double NPV50 = npv_calc(*iter, cohortTmp, maiV,year,rotationForest.get(xi,yi),regprice,regprice0,forestShare,0)*forestShare*dat_all[asID].LandAreaHa;
                            NPVcGrid.set(xi,yi,NPVcw);     
                      }
                             
              }else if (rotationForest.get(xi,yi) < rotMaxBmTh)
            {    
//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\tdisturb +5 before"<<endl;}           	                   	                                                             
              int rotationTmp = rotationForest.get(xi,yi) + 5;
              if (rotationTmp>rotMaxBmTh) {rotationTmp = rotMaxBmTh;}
//              if (rotationTmp < rotMaxBmTh)
//                 {          g4m::ageStruct cohortTmp = *cohort_all[asID];
//                             cohortTmp.setRotPeriod(rotationTmp);
//                             NPVcw = npv_calc(*iter, cohortTmp, maiV,year,rotMaxBmTh,regprice,regprice0,forestShare,1)*forestShare*dat_all[asID].LandAreaHa;
//                  }          
//               if  (NPVcw >=0)   
                {   
                 rotationForest.set(xi,yi,rotationTmp);
                 cohort_all[asID]->setRotPeriod(rotationTmp);
                 g4m::ageStruct cohortTmp = *cohort_all[asID];
                 cohortTmp.setRotPeriod(rotationTmp);
                 
                 g4m::ageStruct cohortTmpNew = *newCohort_all[asID];                 
                 int newForAge = newCohort_all[asID]->getActiveAge();
                 int rotationNewTmp = rotationForestNew.get(xi,yi);
                 if (newForAge > rotationNewTmp){
                     rotationNewTmp+=5;
                     if (rotationNewTmp>rotMaxBmTh) {rotationNewTmp = rotMaxBmTh;}
                     newCohort_all[asID]->setRotPeriod(rotationNewTmp);
                     cohortTmpNew.setRotPeriod(rotationNewTmp);
                     rotationForestNew.set(xi,yi,rotationNewTmp);
                     }
                     
                 g4m::ageStruct::v resTmp = cohortTmp.aging();
                 double sawnW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
                 double restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
                 double sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
                 double restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.

                 g4m::ageStruct::v resTmpNew = cohortTmpNew.aging();
                 double sawnWnew = resTmpNew.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
                 double restWnew = resTmpNew.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
                 double sawnThWnew = resTmpNew.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
                 double restThWnew = resTmpNew.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
                 double newHarvestTmp_cw = ((sawnW + restW + sawnThW + restThW + resUse*resTmp.hv) * (forestShare-dat_all[asID].deforestShare)  +
                             (sawnWnew + restWnew + sawnThWnew + restThWnew +resUse*resTmpNew.hv) * dat_all[asID].AforestShare) * 
                              dat_all[asID].LandAreaHa * iter->FTIMBER.v() + dat_all[asID].deforWoodTotM3; // Total current harvested wood in the cell, m3

                 woodHarvest[country] += (newHarvestTmp_cw - harvestTmp);
                 harvestGrid.set(xi,yi,newHarvestTmp_cw);                                                                                                                                            
//{cout<<"asID=\t"<<asID<<"\harvestTmp=\t"<<newHarvestTmp_cw<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\tdisturb +5 after"<<endl;}           	                   	                                            
                 double NPV50 = npv_calc(*iter, cohortTmp, maiV,year,rotationTmp,regprice,regprice0,forestShare,0)*forestShare*dat_all[asID].LandAreaHa;
                 NPVcGrid.set(xi,yi,NPV50);
                }                                                                                                                                                                                                                                                                       
         } 
         else 
          {
//           g4m::ageStruct cohortTmp = *cohort_all[asID];
//           cohortTmp.setRotPeriod(rotMaxBmTh);
//           double NPV50 = npv_calc(*iter, cohortTmp, maiV,year,rotationForest.get(xi,yi),regprice,regprice0,forestShare,0)*forestShare*dat_all[asID].LandAreaHa;
           NPVcGrid.set(xi,yi,NPVcw);     
          }
      } // if managed
       else if (managedForest.get(xi,yi)<=0 && maiForest.get(xi,yi)>0 && dat_all[asID].OforestShare > 0 )
      {
//          double rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), 3);
          double rotMaxBm = fi.gTopt(maiForest.get(xi,yi), 2);
          double maiV = maiForest.get(xi,yi)*iter->FTIMBER.v();
//          double harvestTmp = harvestGrid.get(xi,yi);  

          double forestShare = dat_all[asID].OforestShare;          
//          double NPVcw = 0.;   	
          double NPVc50 = 0.;   	

          coeff.PriceC.clear();
          coeff.PriceC.insert(0, priceC * iter->CORRUPTION.v());
          
          short int used = 0;                                    
          g4m::ageStruct cohortTmp = *cohort_all[asID];
//          cohortTmp.setStockingdegree(-1.);
//          cohortTmp.setRotPeriod(rotMaxBm);
          NPVc50 = npv_calc(*iter, cohortTmp, maiV,year,rotMaxBm,regprice,regprice0,forestShare,0)*forestShare*dat_all[asID].LandAreaHa;
          NPVcGrid.set(xi,yi,NPVc50);
       }// else managedForest.get(xi,yi)<=0
      
//if (country==47){cout<<"thinningForest(FM100_2)\t"<<thinningForest.get(xi,yi)<<"\tmanagedForest\t"<<int(managedForest.get(xi,yi))<<endl; }         

    }   // if protected
   }    // if toAdjust
  }     // if regions 



  iter++;
 }       // while
}        // if refYear +1 
//}// if year!=2034
//cout<<"year=\t"<<year<<"\t inputHarvestDisturb(11)=\t"<<woodHarvest[11]<<endl;
 
//------------------------------------------------------------------------------
// 
// -------Zero Adjust thinning if population density changed --------------------
//
//------------------------------------------------------------------------------


if ((year > 2000) && ((year) % 10 == 0) && (fm_hurdle == 1))
{
//cout<<"Start Zero Adjust thinning if population density changed"<<endl;
  iter = data_all.begin();

 while (iter != data_all.end())
 {
  short int region = iter->POLESREG[2000];
  if (regions.find(region) != regions.end()) { // Test only some regions         
     short int country = iter->COUNTRY[2000];
     if (toAdjust.find(country) != toAdjust.end()) { // do only for some countries         
     if (iter->PROTECT[2000] == 0)
	{
    int asID = iter->asID;
    char regionch[2];
    int2str(region,regionch);
    string regprice = "re"+string(regionch)+"price0";
    string regprice0 = "re"+string(regionch)+"price0";
    char countrych[3];
    int2str(country,countrych);
    string countryprice = "re"+string(countrych)+"price0";
    country = iter->COUNTRY[2000]; 
    int xi = (iter->x);
    int yi = (iter->y);
    
//  if (woodHarvest[country] < 0.9 * wprod[countryprice].v(year))
//  if (woodHarvest[country] < 0.85 * wprod[countryprice].v(year))  
  if (woodHarvest[country] < 0.88 * wprod[countryprice].v(year))   
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

      double countryHarvestTmp = woodHarvest[country];
      
      double NPVtmp = 0.;
      double maiV = maiForest.get(xi,yi)*iter->FTIMBER.v();

    g4m::ageStruct cohortTmpNew = *newCohort_all[asID]; 
    g4m::ageStruct cohortTmp = *cohort_all[asID];
   	double harvestTmp = harvestGrid.get(xi,yi);              
//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t0<0.88 before"<<endl;}           	                   	                                            
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

             if (useChange.find(asID)!=useChange.end()){
                 managedForest.set(xi,yi,managedForestTmp+3);
                 Rotation = rotationForestO.get(xi,yi);
                 SD = thinningForestO.get(xi,yi);
                                      
              }else if (managedForest.get(xi,yi) == 0)
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

                   if (useChange.find(asID)!=useChange.end()){
                      managedForest.set(xi,yi,managedForestTmp+3);
                      Rotation = rotationForestO.get(xi,yi);
                      SD = thinningForestO.get(xi,yi);
                      rotationForestNew.set(xi,yi,rotationForestNewO.get(xi,yi));	
                      thinningForestNew.set(xi,yi,SD);	
                      newCohort_all[asID]->setRotPeriod(rotationForestNewO.get(xi,yi));
                      newCohort_all[asID]->setStockingdegree(SD);  

                   }else if (managedForest.get(xi,yi) == 0)
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
//                woodHarvest[country] += (newHarvestTmp-harvestTmp);
                countryHarvestTmp += (newHarvestTmp-harvestTmp);
                
                coeff.PriceC.clear();
                coeff.PriceC.insert(0, priceC * iter->CORRUPTION.v());
                short int used = 0;
                if (thinningForest.get(xi,yi)>0){used = 1;}
                NPVtmp = npv_calc(*iter, cohortTmp, maiV,year,Rotation,regprice,regprice0,dat_all[asID].OforestShare,used)*dat_all[asID].LandAreaHa * dat_all[asID].OforestShare;
            
    
                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[country] - wprod[countryprice].v(year)) 
//                                   && NPVtmp >= NPVbau[year-refYear-1][asID]
//                                   && NPVtmp >= 0
                                   && NPVtmp >= NPVcGrid.get(xi,yi)
                                   ){
                    woodHarvest[country] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,newHarvestTmp);
                    if (useChange.find(asID) == useChange.end()){manageChForest.set(xi,yi,1);}
//{cout<<"asID=\t"<<asID<<"\tnewHarvestTmp=\t"<<newHarvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t0<0.88"<<endl;}           	                   	        
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

//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t0<0.88 after"<<endl;}           	                   	                                            
        }  //End  if ((data["POPDENS"].v(1990) >0) && (data["GDP"].v(1990) > 0))

      } // End  if (managedForest(xi,yi)<=0)

// }else if (woodHarvest[country] > 1.1 * wprod[countryprice].v(year)) 
// }else if (woodHarvest[country] > 1.15 * wprod[countryprice].v(year))  
 }else if (woodHarvest[country] > 1.12 * wprod[countryprice].v(year))  
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

      double countryHarvestTmp = woodHarvest[country];
      
      double NPVtmp = 0.;
      double maiV = maiForest.get(xi,yi)*iter->FTIMBER.v();

    g4m::ageStruct cohortTmp = *cohort_all[asID];
   	double harvestTmp = harvestGrid.get(xi,yi);              
//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t0>1.12 before"<<endl;}           	                   	                                            
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
                cohortTmp.setRotPeriod(Rotation);
                cohortTmp.setStockingdegree(-1.);
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
//                woodHarvest[country] += (newHarvestTmp-harvestTmp);
//                countryHarvestTmp += (newHarvestTmp-harvestTmp);
                countryHarvestTmp = countryHarvestTmp - harvestTmp+dat_all[asID].deforWoodTotM3;             
                
                coeff.PriceC.clear();
                coeff.PriceC.insert(0, priceC * iter->CORRUPTION.v());                   
                short int used = 0;
                if (thinningForest.get(xi,yi)>0){used = 1;}

                NPVtmp = npv_calc(*iter, cohortTmp, maiV,year,Rotation,regprice,regprice0,dat_all[asID].OforestShare,used)*dat_all[asID].LandAreaHa * dat_all[asID].OforestShare;
                

                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[country] - wprod[countryprice].v(year)) 
//                                   && NPVtmp >= NPVbau[year-refYear-1][asID]
//                                   && NPVtmp >= 0
                                   && NPVtmp >= NPVcGrid.get(xi,yi)
                                   ){
                    woodHarvest[country] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,dat_all[asID].deforWoodTotM3);
           	        manageChForest.set(xi,yi,1);
//{cout<<"asID=\t"<<asID<<"\tnewHarvestTmp=\t"<<dat_all[asID].deforWoodTotM3<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t0>1.12"<<endl;}           	                   	                   	        
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

//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t0>1.12 after"<<endl;}           	                   	                                            
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
//cout << "Zero pass is finished"<< endl; 
//if (iter->COUNTRY[2000] == 197){cout<<"woodHarvest_f0= "<<woodHarvest[region-1]<<"\t woodHarvestStat= "<<wprod[countryprice].v(year) <<endl;}
} // End if year


 

//cout<<"Start First pass = adjust rotation time "<<endl;  
//----First pass = adjust rotation time -------  
//dataDetStruct::iterator iter = data_all.begin();
iter = data_all.begin();
while (iter != data_all.end()) {
  short int region = iter->POLESREG[2000];
  if (regions.find(region) != regions.end()) { // Test only some regions         
    short int country = iter->COUNTRY[2000];
    if (toAdjust.find(country) != toAdjust.end()) {  
     int asID = iter->asID;
     if ((iter->PROTECT[2000]==0)&&((iter->POTVEG[2000]<10 && iter->FOREST[2000]>0 && iter->MAIE[2000]>0)||(iter->POTVEG[2000]<10 && (iter->NPP[2000]>0)||iter->MAIN[2000]>0))){
    char regionch[2];
    int2str(region,regionch);
    string regprice = "re"+string(regionch)+"price0";
    string regprice0 = "re"+string(regionch)+"price0";
    char countrych[3];
    int2str(country,countrych);
    string countryprice = "re"+string(countrych)+"price0";
    country = iter->COUNTRY[2000];     
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
  double countryHarvestTmp = woodHarvest[country];  
  
  double NPVtmp = 0.;
  double maiV = maiForest.get(xi,yi)*iter->FTIMBER.v();

//  if (woodHarvest[country] < 0.97 * wprod[countryprice].v(year))
  if (woodHarvest[country] < 0.99 * wprod[countryprice].v(year))  
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
//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t1<0.99 before"<<endl;}           	                   	                                            
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
                
                coeff.PriceC.clear();
                coeff.PriceC.insert(0, priceC * iter->CORRUPTION.v());       
                short int used = 0;
                if (thinningForest.get(xi,yi)>0){used = 1;}
                         
                NPVtmp = npv_calc(*iter, cohortTmp, maiV,year,Rotation,regprice,regprice0,dat_all[asID].OforestShare,used)*dat_all[asID].LandAreaHa * dat_all[asID].OforestShare;

//                if (NPVtmp == 0){NPVtmp +=0.01;}
//cout<<"country(<0.97)= "<<country<<"\t fm_hurdle= "<<fm_hurdle<<"\t NPVtmp= \t"<<NPVtmp<<"\t NPVbau*fm_hurdle= \t"<<fm_hurdle*NPVbau[year-refYear-1][asID]<<endl;                
                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[country] - wprod[countryprice].v(year)) 
//                                   && fm_hurdle*NPVtmp >= NPVbau[year-refYear-1][asID]
//                                   && fm_hurdle*NPVtmp >= 0
                                   && fm_hurdle*NPVtmp >= NPVcGrid.get(xi,yi)
                                   ){
//cout<<"woodHarvest=\t"<<woodHarvest[country]<<"\tcountryHarvestTmp=\t"<<countryHarvestTmp<<endl;
                    woodHarvest[country] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,newHarvestTmp);
//{cout<<"asID=\t"<<asID<<"\tnewHarvestTmp=\t"<<newHarvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t1<0.99"<<endl;}           	                   	        
                }else{ // return old values
                rotationForest.set(xi,yi,rotationForestTmp);	
                cohort_all[asID]->setRotPeriod(rotationForestTmp);
                rotationForestNew.set(xi,yi,rotationForestNewTmp);	
                newCohort_all[asID]->setRotPeriod(rotationForestNewTmp);
                }  
//        }
//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t1<0.99 after"<<endl;}           	                   	                                            
       }  // end for if (managedForest(xi,yi)>=2)          
    
    } //end for if (woodHarvest[region] <= 0.9 * wprod[region].v(year))
//    else if (woodHarvest[country] > 1.03 * wprod[countryprice].v(year)) 
    else if (woodHarvest[country] > 1.01 * wprod[countryprice].v(year))     
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
//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t3>1.01 before"<<endl;}           	                   	                                            

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
                
                coeff.PriceC.clear();
                coeff.PriceC.insert(0, priceC * iter->CORRUPTION.v());         
                short int used = 0;
                if (thinningForest.get(xi,yi)>0){used = 1;}
                       
                NPVtmp = npv_calc(*iter, cohortTmp, maiV,year,Rotation,regprice,regprice0,dat_all[asID].OforestShare,used)*dat_all[asID].LandAreaHa * dat_all[asID].OforestShare;

//                if (NPVtmp == 0){NPVtmp +=0.01;}
                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[country] - wprod[countryprice].v(year)) 
//                                   && fm_hurdle*NPVtmp >= NPVbau[year-refYear-1][asID]
//                                   && fm_hurdle*NPVtmp >= 0
                                   && fm_hurdle*NPVtmp >= NPVcGrid.get(xi,yi)
                                   ){
                    woodHarvest[country] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,newHarvestTmp);
//{cout<<"asID=\t"<<asID<<"\tnewHarvestTmp=\t"<<newHarvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t1>1.01"<<endl;}           	                   	        
                }else{ // return old values
                rotationForest.set(xi,yi,rotationForestTmp);	
                cohort_all[asID]->setRotPeriod(rotationForestTmp);
                rotationForestNew.set(xi,yi,rotationForestNewTmp);	
                newCohort_all[asID]->setRotPeriod(rotationForestNewTmp);
                }  
//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t1>1.01 after"<<endl;}           	                   	                                            
        }
       

       }  // end for if (managedForest(xi,yi)<=2)       
      }   //end for esle if (woodHarvest[region] >= 1.1 * wprod[region].v(year))
  } //End Protect
 }  // End some countries
}  // Test only some regions 
  iter++;
} // End While
//if (iter->COUNTRY[2000] == 197){cout<<"woodHarvest_f1= "<<woodHarvest[region-1]<<"\t woodHarvestStat= "<<wprod[countryprice].v(year) <<endl;}   
//cout<<" Finished First pass"<<endl;
// ----- End of First pass


//----Second pass = adjust rotation time -------  
//dataDetStruct::iterator iter = data_all.begin();
iter = data_all.begin();
while (iter != data_all.end()) {
  short int region = iter->POLESREG[2000];
  if (regions.find(region) != regions.end()) { // Test only some regions         
     short int country = iter->COUNTRY[2000];
     if (toAdjust.find(country) != toAdjust.end()) {  
       if ((iter->PROTECT[2000]==0)&&((iter->POTVEG[2000]<10 && iter->FOREST[2000]>0 && iter->MAIE[2000]>0)||(iter->POTVEG[2000]<10 && (iter->NPP[2000]>0)||iter->MAIN[2000]>0))){
        int asID = iter->asID;
        char regionch[2];
    int2str(region,regionch);
    string regprice = "re"+string(regionch)+"price0";
    string regprice0 = "re"+string(regionch)+"price0";
    char countrych[3];
    int2str(country,countrych);
    string countryprice = "re"+string(countrych)+"price0";
    country = iter->COUNTRY[2000];     
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
  double countryHarvestTmp = woodHarvest[country];  
  
  double NPVtmp = 0.;
  double maiV = maiForest.get(xi,yi)*iter->FTIMBER.v();

//  if (woodHarvest[country] < 0.9 * wprod[countryprice].v(year))
//  if (woodHarvest[country] < 0.95 * wprod[countryprice].v(year))
  if (woodHarvest[country] < 0.98 * wprod[countryprice].v(year))  
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
//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t2<0.98 before"<<endl;}           	                   	                                            
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
                
                coeff.PriceC.clear();
                coeff.PriceC.insert(0, priceC * iter->CORRUPTION.v());       
                short int used = 0;
                if (thinningForest.get(xi,yi)>0){used = 1;}
                         
                NPVtmp = npv_calc(*iter, cohortTmp, maiV,year,Rotation,regprice,regprice0,dat_all[asID].OforestShare,used)*dat_all[asID].LandAreaHa * dat_all[asID].OforestShare;

//                if (NPVtmp == 0){NPVtmp +=0.01;}               
                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[country] - wprod[countryprice].v(year)) 
//                                   && fm_hurdle*NPVtmp >= NPVbau[year-refYear-1][asID]
//                                   && fm_hurdle*NPVtmp >= 0
                                   && fm_hurdle*NPVtmp >= NPVcGrid.get(xi,yi)
                                   ){
                    woodHarvest[country] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,newHarvestTmp);
//{cout<<"asID=\t"<<asID<<"\tnewHarvestTmp=\t"<<newHarvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t2<0.98"<<endl;}           	                   	        
                }else{ // return old values
                rotationForest.set(xi,yi,rotationForestTmp);	
                cohort_all[asID]->setRotPeriod(rotationForestTmp);
                rotationForestNew.set(xi,yi,rotationForestNewTmp);	
                newCohort_all[asID]->setRotPeriod(rotationForestNewTmp);
                }  
//        }

//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t2<0.98 after"<<endl;}           	                   	                                            
       }  // end for if (managedForest(xi,yi)>=2)          
    
    } //end for if (woodHarvest[region] <= 0.9 * wprod[region].v(year))
//    else if (woodHarvest[country] > 1.1 * wprod[countryprice].v(year)) 
//    else if (woodHarvest[country] > 1.05 * wprod[countryprice].v(year)) 
//    else if (woodHarvest[country] > 1.03 * wprod[countryprice].v(year)) 
    else if (woodHarvest[country] > 1.02 * wprod[countryprice].v(year)) 
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

//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t2>1.02 before"<<endl;}           	                   	                                            
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
                
                coeff.PriceC.clear();
                coeff.PriceC.insert(0, priceC * iter->CORRUPTION.v());      
                short int used = 0;
                if (thinningForest.get(xi,yi)>0){used = 1;}
                          
                NPVtmp = npv_calc(*iter, cohortTmp, maiV,year,Rotation,regprice,regprice0,dat_all[asID].OforestShare,used)*dat_all[asID].LandAreaHa * dat_all[asID].OforestShare;

//                if (NPVtmp == 0){NPVtmp +=0.01;}                
                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[country] - wprod[countryprice].v(year)) 
//                                   && fm_hurdle*NPVtmp >= NPVbau[year-refYear-1][asID]
//                                   && fm_hurdle*NPVtmp >= 0
                                   && fm_hurdle*NPVtmp >= NPVcGrid.get(xi,yi)
                                   ){
                    woodHarvest[country] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,newHarvestTmp);
//{cout<<"asID=\t"<<asID<<"\tnewHarvestTmp=\t"<<newHarvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t2>1.02"<<endl;}           	                   	        
                }else{ // return old values
                rotationForest.set(xi,yi,rotationForestTmp);	
                cohort_all[asID]->setRotPeriod(rotationForestTmp);
                rotationForestNew.set(xi,yi,rotationForestNewTmp);	
                newCohort_all[asID]->setRotPeriod(rotationForestNewTmp);
                }  
        }
        
//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t2>1.02 after"<<endl;}           	                   	                                            

       }  // end for if (managedForest(xi,yi)<=2)       
      }   //end for esle if (woodHarvest[region] >= 1.1 * wprod[region].v(year))
  } //End Protect
 }  // End some countries
}  // Test only some regions 
  iter++;
} // End While
//if (iter->COUNTRY[2000] == 197){cout<<"woodHarvest_f1= "<<woodHarvest[region-1]<<"\t woodHarvestStat= "<<wprod[countryprice].v(year) <<endl;}   
//cout<<" Finished First pass"<<endl;
// ----- End of Second pass






////------------------------------------------------------------------------------
//// 
//// -------Third pass: Adjust thinning -----------------------------------------------
////
////------------------------------------------------------------------------------
//cout<<"Start Second pass = Adjust thinning"<< endl;
 iter = data_all.begin();
 while (iter != data_all.end())
 {
  short int region = iter->POLESREG[2000];
  if (regions.find(region) != regions.end()) { // Test only some regions          
     short int country = iter->COUNTRY[2000];
     if (toAdjust.find(country) != toAdjust.end()) {  
	if (iter->PROTECT[2000] == 0)
	{
    int xi = (iter->x);
    int yi = (iter->y);
   if (maiForest.get(xi,yi)>0)
   { 
    int asID = iter->asID;
    char regionch[2];
    int2str(region,regionch);
    string regprice = "re"+string(regionch)+"price0";
    string regprice0 = "re"+string(regionch)+"price0";
    char countrych[3];
    int2str(country,countrych);
    string countryprice = "re"+string(countrych)+"price0";
    country = iter->COUNTRY[2000];     
//  if (floor(woodHarvest[country]) < 0.95 * wprod[countryprice].v(year))
//  if (floor(woodHarvest[country]) < 0.94 * wprod[countryprice].v(year))  
  if (floor(woodHarvest[country]) < 0.97 * wprod[countryprice].v(year))  
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

      double countryHarvestTmp = woodHarvest[country]; 
      
      double NPVtmp = 0.;
      double maiV = maiForest.get(xi,yi)*iter->FTIMBER.v();
      double stockingDegree = 1.5;     
      g4m::ageStruct cohortTmpNew = *newCohort_all[asID]; 
      g4m::ageStruct cohortTmp = *cohort_all[asID];
   	  double harvestTmp = harvestGrid.get(xi,yi);              
//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t3<0.97 before"<<endl;}           	                   	                                            

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
             if (useChange.find(asID)!=useChange.end()){
                 managedForest.set(xi,yi,managedForestTmp+3);
                 Rotation = rotationForestO.get(xi,yi);
                 stockingDegree = thinningForestO.get(xi,yi);
             }else if (managedForest.get(xi,yi) == 0)
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
                  if (useChange.find(asID)!=useChange.end()){
                      managedForest.set(xi,yi,managedForestTmp+3);
                      Rotation = rotationForestO.get(xi,yi);
                      stockingDegree = thinningForestO.get(xi,yi);
                      rotationForestNew.set(xi,yi,rotationForestNewO.get(xi,yi));	
                      thinningForestNew.set(xi,yi,stockingDegree);	
                      newCohort_all[asID]->setRotPeriod(rotationForestNewO.get(xi,yi));
                      newCohort_all[asID]->setStockingdegree(stockingDegree);  
                  }else if (managedForest.get(xi,yi) == 0)
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
                
                coeff.PriceC.clear();
                coeff.PriceC.insert(0, priceC * iter->CORRUPTION.v());                
                short int used = 0;
                if (thinningForest.get(xi,yi)>0){used = 1;}
                
                NPVtmp = npv_calc(*iter, cohortTmp, maiV,year,Rotation,regprice,regprice0,dat_all[asID].OforestShare,used)*dat_all[asID].LandAreaHa * dat_all[asID].OforestShare;

//                if (NPVtmp == 0){NPVtmp +=0.01;}                
                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[country] - wprod[countryprice].v(year)) 
//                                   && fm_hurdle*NPVtmp >= NPVbau[year-refYear-1][asID]
//                                   && fm_hurdle*NPVtmp >= 0
                                   && fm_hurdle*NPVtmp >= NPVcGrid.get(xi,yi)
                                   ){
                    woodHarvest[country] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,newHarvestTmp);
         	        if (useChange.find(asID)== useChange.end()){manageChForest.set(xi,yi,1);}
//{cout<<"asID=\t"<<asID<<"\tnewHarvestTmp=\t"<<newHarvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t3<0.97"<<endl;}           	                   	        
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
//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t3<0.97 after"<<endl;}           	                   	                                            

        } // End  if ((managedForest(xi,yi)<=0) & (managedForest(xi,yi)>-2)
   
// }else if (floor(woodHarvest[country]) > 1.05 * wprod[countryprice].v(year)) 
// }else if (floor(woodHarvest[country]) > 1.2 * wprod[countryprice].v(year))  
// }else if (floor(woodHarvest[country]) > 1.1 * wprod[countryprice].v(year))  
// }else if (floor(woodHarvest[country]) > 1.06 * wprod[countryprice].v(year))  
// }else if (floor(woodHarvest[country]) > 1.04 * wprod[countryprice].v(year))   
 }else if (floor(woodHarvest[country]) > 1.03 * wprod[countryprice].v(year))   
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

      double countryHarvestTmp = woodHarvest[country];
      
      double NPVtmp = 0.;
      double maiV = maiForest.get(xi,yi)*iter->FTIMBER.v();
      g4m::ageStruct cohortTmp = *cohort_all[asID];
   	  double harvestTmp = harvestGrid.get(xi,yi);              

//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t3>1.03 before"<<endl;}           	                   	                                             
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
                cohortTmp.setRotPeriod(Rotation);
                cohortTmp.setStockingdegree(-1.);
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
                
                coeff.PriceC.clear();
                coeff.PriceC.insert(0, priceC * iter->CORRUPTION.v());     
                short int used = 0;
                if (thinningForest.get(xi,yi)>0){used = 1;}
                           
                NPVtmp = npv_calc(*iter, cohortTmp, maiV,year,Rotation,regprice,regprice0,dat_all[asID].OforestShare,used)*dat_all[asID].LandAreaHa * dat_all[asID].OforestShare;

//                if (NPVtmp == 0){NPVtmp +=0.01;}                
                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[country] - wprod[countryprice].v(year)) 
//                                   && fm_hurdle*NPVtmp >= NPVbau[year-refYear-1][asID]
//                                   && fm_hurdle*NPVtmp >= 0
                                   && fm_hurdle*NPVtmp >= NPVcGrid.get(xi,yi)
                                   ){
                    woodHarvest[country] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,dat_all[asID].deforWoodTotM3);
           	        manageChForest.set(xi,yi,1);
//{cout<<"asID=\t"<<asID<<"\tnewHarvestTmp=\t"<<dat_all[asID].deforWoodTotM3<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t3>1.03"<<endl;}           	                   	        
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

//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t3>1.03 after"<<endl;}           	                   	                                            
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
     short int country = iter->COUNTRY[2000];
     if (toAdjust.find(country) != toAdjust.end()) {  
	if (iter->PROTECT[2000] == 0)
	{
    int asID = iter->asID;
    char regionch[2];
    int2str(region,regionch);
    string regprice = "re"+string(regionch)+"price0";
    string regprice0 = "re"+string(regionch)+"price0";
    char countrych[3];
    int2str(country,countrych);
    string countryprice = "re"+string(countrych)+"price0";
    country = iter->COUNTRY[2000];     
    int xi = (iter->x);
    int yi = (iter->y);

//if (floor(woodHarvest[country]) > 1.35 * wprod[countryprice].v(year))  
//if (floor(woodHarvest[country]) > 1.15 * wprod[countryprice].v(year))
//if (floor(woodHarvest[country]) > 1.1 * wprod[countryprice].v(year))  
//if (floor(woodHarvest[country]) > 1.08 * wprod[countryprice].v(year))  
if (floor(woodHarvest[country]) > 1.05 * wprod[countryprice].v(year))  
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

      double countryHarvestTmp = woodHarvest[country];
      
      double NPVtmp = 0.;
      double maiV = maiForest.get(xi,yi)*iter->FTIMBER.v();
      g4m::ageStruct cohortTmp = *cohort_all[asID];
   	  double harvestTmp = harvestGrid.get(xi,yi);              
//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t3>1.05 before"<<endl;}           	                   	                                            
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
                cohortTmp.setRotPeriod(Rotation);
                cohortTmp.setStockingdegree(-1.);                
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
                
                coeff.PriceC.clear();
                coeff.PriceC.insert(0, priceC * iter->CORRUPTION.v());   
                short int used = 0;
                if (thinningForest.get(xi,yi)>0){used = 1;}
                             
                NPVtmp = npv_calc(*iter, cohortTmp, maiV,year,Rotation,regprice,regprice0,dat_all[asID].OforestShare,used)*dat_all[asID].LandAreaHa * dat_all[asID].OforestShare;

//                if (NPVtmp == 0){NPVtmp +=0.01;}                
                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[country] - wprod[countryprice].v(year)) 
//                                   && fm_hurdle*NPVtmp >= NPVbau[year-refYear-1][asID]
//                                   && fm_hurdle*NPVtmp >= 0
                                   && fm_hurdle*NPVtmp >= NPVcGrid.get(xi,yi)
                                   ){
                    woodHarvest[country] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,dat_all[asID].deforWoodTotM3);
           	        manageChForest.set(xi,yi,1);
//{cout<<"asID=\t"<<asID<<"\tnewHarvestTmp=\t"<<dat_all[asID].deforWoodTotM3<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t3>1.05"<<endl;}           	                   	        
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
//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t3>1.05 after"<<endl;}           	                   	                                            
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
  short int country = iter->COUNTRY[2000];
  if (toAdjust.find(country) != toAdjust.end()) {  
   if ((iter->PROTECT[2000]==0)&&((iter->POTVEG[2000]<10 && iter->FOREST[2000]>0 && iter->MAIE[2000]>0)||(iter->POTVEG[2000]<10 && (iter->NPP[2000]>0)||iter->MAIN[2000]>0))){
    int asID = iter->asID;
    char regionch[2];
    int2str(region,regionch);
    string regprice = "re"+string(regionch)+"price0";
    string regprice0 = "re"+string(regionch)+"price0";
    char countrych[3];
    int2str(country,countrych);
    string countryprice = "re"+string(countrych)+"price0";
    country = iter->COUNTRY[2000];     
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
      double countryHarvestTmp = woodHarvest[country];
      
      double NPVtmp = 0.;
      double maiV = maiForest.get(xi,yi)*iter->FTIMBER.v();

//  if (floor(woodHarvest[country]) < 0.97 * wprod[countryprice].v(year))
  if (floor(woodHarvest[country]) < 0.99 * wprod[countryprice].v(year))
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
//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t4<0.99 before"<<endl;}           	                   	                                            
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
                
                coeff.PriceC.clear();
                coeff.PriceC.insert(0, priceC * iter->CORRUPTION.v());                
                short int used = 0;
                if (thinningForest.get(xi,yi)>0){used = 1;}
                
                NPVtmp = npv_calc(*iter, cohortTmp, maiV,year,Rotation,regprice,regprice0,dat_all[asID].OforestShare,used)*dat_all[asID].LandAreaHa * dat_all[asID].OforestShare;

//                if (NPVtmp == 0){NPVtmp +=0.01;}                
                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[country] - wprod[countryprice].v(year)) 
//                                   && fm_hurdle*NPVtmp >= NPVbau[year-refYear-1][asID]
//                                   && fm_hurdle*NPVtmp >= 0
                                   && fm_hurdle*NPVtmp >= NPVcGrid.get(xi,yi)
                                   ){
                    woodHarvest[country] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,newHarvestTmp);
//{cout<<"asID=\t"<<asID<<"\tnewHarvestTmp=\t"<<newHarvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t4<0.99"<<endl;}           	                   	        
                }else{ // return old values
                rotationForest.set(xi,yi,rotationForestTmp);	
                cohort_all[asID]->setRotPeriod(rotationForestTmp);
                rotationForestNew.set(xi,yi,rotationForestNewTmp);	
                newCohort_all[asID]->setRotPeriod(rotationForestNewTmp);
                }        
//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t4<0.99 after"<<endl;}           	                   	                                            
//        }
//        } //End if ((cohort_all[asID]->getArea(0) == 0.)||(cohort_all[asID]->getArea(0) >= 1/400.)){
       }  // end for if (managedForest(xi,yi)>=2)          
    
    } //end for if (woodHarvest[region] <= 0.9 * wprod[region].v(year))
//    else if (floor(woodHarvest[country]) > 1.03 * wprod[countryprice].v(year)) 
    else if (floor(woodHarvest[country]) > 1.01 * wprod[countryprice].v(year))     
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
//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t4>1.01 before"<<endl;}           	                   	                                            
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
                
                coeff.PriceC.clear();
                coeff.PriceC.insert(0, priceC * iter->CORRUPTION.v());      
                short int used = 0;
                if (thinningForest.get(xi,yi)>0){used = 1;}

                NPVtmp = npv_calc(*iter, cohortTmp, maiV,year,Rotation,regprice,regprice0,dat_all[asID].OforestShare,used)*dat_all[asID].LandAreaHa * dat_all[asID].OforestShare;

//                if (NPVtmp == 0){NPVtmp +=0.01;}                
                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[country] - wprod[countryprice].v(year)) 
//                                   && fm_hurdle*NPVtmp >= NPVbau[year-refYear-1][asID]
//                                   && fm_hurdle*NPVtmp >= 0
                                   && fm_hurdle*NPVtmp >= NPVcGrid.get(xi,yi)
                                   ){
                    woodHarvest[country] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,newHarvestTmp);
//{cout<<"asID=\t"<<asID<<"\tnewHarvestTmp=\t"<<newHarvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t4>1.01"<<endl;}           	                   	        
                }else{ // return old values
                rotationForest.set(xi,yi,rotationForestTmp);	
                cohort_all[asID]->setRotPeriod(rotationForestTmp);
                rotationForestNew.set(xi,yi,rotationForestNewTmp);	
                newCohort_all[asID]->setRotPeriod(rotationForestNewTmp);
                }        
        }
//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t4>1.01 after"<<endl;}           	                   	                                            
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
  short int country = iter->COUNTRY[2000];
  if (toAdjust.find(country) != toAdjust.end()) {  
   if ((iter->PROTECT[2000]==0)&&((iter->POTVEG[2000]<10 && iter->FOREST[2000]>0 && iter->MAIE[2000]>0)||(iter->POTVEG[2000]<10 && (iter->NPP[2000]>0)||iter->MAIN[2000]>0))){
    int asID = iter->asID;
    char regionch[2];
    int2str(region,regionch);
    string regprice = "re"+string(regionch)+"price0";
    string regprice0 = "re"+string(regionch)+"price0";
    char countrych[3];
    int2str(country,countrych);
    string countryprice = "re"+string(countrych)+"price0";
    country = iter->COUNTRY[2000]; 
    
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
  double countryHarvestTmp = woodHarvest[country];
  
  double NPVtmp = 0.;
  double maiV = maiForest.get(xi,yi)*iter->FTIMBER.v();

//  if (floor(woodHarvest[country]) < 0.9 * wprod[countryprice].v(year))
//  if (floor(woodHarvest[country]) < 0.95 * wprod[countryprice].v(year))
  if (floor(woodHarvest[country]) < 0.98 * wprod[countryprice].v(year))
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
//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t5<0.98 before"<<endl;}           	                   	                                            
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
                
                coeff.PriceC.clear();
                coeff.PriceC.insert(0, priceC * iter->CORRUPTION.v());                
                short int used = 0;
                if (thinningForest.get(xi,yi)>0){used = 1;}
                
                NPVtmp = npv_calc(*iter, cohortTmp, maiV,year,Rotation,regprice,regprice0,dat_all[asID].OforestShare,used)*dat_all[asID].LandAreaHa * dat_all[asID].OforestShare;

//                if (NPVtmp == 0){NPVtmp +=0.01;}                
                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[country] - wprod[countryprice].v(year)) 
//                                   && fm_hurdle*NPVtmp >= NPVbau[year-refYear-1][asID]
//                                   && fm_hurdle*NPVtmp >= 0
                                   && fm_hurdle*NPVtmp >= NPVcGrid.get(xi,yi)
                                   ){
                    woodHarvest[country] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,newHarvestTmp);
//{cout<<"asID=\t"<<asID<<"\tnewHarvestTmp=\t"<<newHarvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t5<0.98"<<endl;}           	                   	        
                }else{ // return old values
                rotationForest.set(xi,yi,rotationForestTmp);	
                cohort_all[asID]->setRotPeriod(rotationForestTmp);
                rotationForestNew.set(xi,yi,rotationForestNewTmp);	
                newCohort_all[asID]->setRotPeriod(rotationForestNewTmp);
                }        
//        }
//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t5<0.98 after"<<endl;}           	                   	                                            
       }  // end for if (managedForest(xi,yi)>=2)          
    
    } //end for if (woodHarvest[region] <= 0.9 * wprod[region].v(year))
//    else if (floor(woodHarvest[country]) > 1.1 * wprod[countryprice].v(year)) 
//    else if (floor(woodHarvest[country]) > 1.05 * wprod[countryprice].v(year)) 
//    else if (floor(woodHarvest[country]) > 1.03 * wprod[countryprice].v(year))     
    else if (floor(woodHarvest[country]) > 1.02 * wprod[countryprice].v(year))     
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
//{cout<<"asID=\t"<<asID<<"\HarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t5>1.02 before"<<endl;}           	                   	                                            
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
                
                coeff.PriceC.clear();
                coeff.PriceC.insert(0, priceC * iter->CORRUPTION.v());                
                short int used = 0;
                if (thinningForest.get(xi,yi)>0){used = 1;}
                
                NPVtmp = npv_calc(*iter, cohortTmp, maiV,year,Rotation,regprice,regprice0,dat_all[asID].OforestShare,used)*dat_all[asID].LandAreaHa * dat_all[asID].OforestShare;

//                if (NPVtmp == 0){NPVtmp +=0.01;}                
                if (abs(countryHarvestTmp - wprod[countryprice].v(year))<(1+tolerance)*abs(woodHarvest[country] - wprod[countryprice].v(year)) 
//                                   && fm_hurdle*NPVtmp >= NPVbau[year-refYear-1][asID]
//                                   && fm_hurdle*NPVtmp >= 0
                                   && fm_hurdle*NPVtmp >= NPVcGrid.get(xi,yi)
                                   ){
                    woodHarvest[country] = countryHarvestTmp;                                      
           	        harvestGrid.set(xi,yi,newHarvestTmp);
//{cout<<"asID=\t"<<asID<<"\tnewHarvestTmp=\t"<<newHarvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t5>1.02"<<endl;}           	        
                }else{ // return old values
                rotationForest.set(xi,yi,rotationForestTmp);	
                cohort_all[asID]->setRotPeriod(rotationForestTmp);
                rotationForestNew.set(xi,yi,rotationForestNewTmp);	
                newCohort_all[asID]->setRotPeriod(rotationForestNewTmp);
                }        
        }
//{cout<<"asID=\t"<<asID<<"\tHarvestTmp=\t"<<harvestTmp<<"\tthinning=\t"<<thinningForest.get(xi,yi)<<"\tSD=\t"<<cohort_all[asID]->getStockingdegree()<<"\tRotation=\t"<<rotationForest.get(xi,yi)<<"\tRotPeriod=\t"<<cohort_all[asID]->getRotPeriod()<<"\t5>1.02 before"<<endl;}           	                   	                                            
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



    if (toAdjust.size()>1){
      for (int i=1; i<=209; i++){
        char countrych[3];
        int ii = i;
        int2str(ii,countrych);
        string countryprice = "re"+string(countrych)+"price0";
        if (wprod[countryprice].v(year) > 0){
        
             if ((regions.find(coutryRegion[i]) != regions.end()) 
                      && (toAdjust.find(i) != toAdjust.end())  // Test only some countries         
                         && i!=114  // Luxenbourg is too small (1 cell) to be considered correctly
                         )
                 {                                  
                 harvDiff[i] = abs(woodHarvest[i] - wprod[countryprice].v(year))/wprod[countryprice].v(year);                 
    //cout<<"harvDiff["<<i<<"]"<<"\t"<<harvDiff[i]<<endl;

cout<<"harvDiff["<<i<<"]="<<"\t"<<harvDiff[i]<<"\t woodHarvest["<<i<<"]="<<"\t"<<woodHarvest[i]<<"\t wprod["<<countryprice<<"]"<<year<<"=\t"<<wprod[countryprice].v(year)<<endl;
             }else{
               harvDiff[i] = 0;}
        }else{
        harvDiff[i] = 0;
        }
    //cout<<"harvDiff["<<i<<"]"<<"\t"<<harvDiff[i]<<endl;
      }
      maxDiff = *max_element(harvDiff+1,harvDiff+210);
      for (int j=0;j<=209;j++){if (harvDiff[j] == maxDiff){maxDiffCountry = j; 
                                                                           break;}}
    }else if (toAdjust.size()==1) {
          set<int>::iterator itt = toAdjust.begin();
          int ii = *itt;
cout<<"ii=\t"<<ii<<"\t woodHarvest["<<ii<<"]="<<"\t"<<woodHarvest[ii]<<endl;          
          char countrych[3];
          int2str(ii,countrych);
          string countryprice = "re"+string(countrych)+"price0";          
          harvDiff[ii] = abs(woodHarvest[ii] - wprod[countryprice].v(year))/wprod[countryprice].v(year); 
          maxDiff = harvDiff[ii];
          }
          
      
}

#endif
