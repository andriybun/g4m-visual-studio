#include "forest.h"

extern g4m::coeffStruct coeff;

extern set<int> regions;

extern g4m::ipol<double,double> sws;
extern g4m::ipol<double,double> hlv;
extern g4m::ipol<double,double> hle;
extern g4m::ipol<double,double> dbv;
extern g4m::ipol<double,double> dbe;

extern double countryLosses[NumberOfCountries];

void initCohorts(dataDetStruct &data_all, g4m::incrementTab &fi, ageStructVector &cohort_all, ageStructVector &newCohort_all,
                 griddata2<double> &maiForest,griddata2<double> &thinningForest, griddata2<double> &rotationForest) 
 {


//cout<<" Deleating cohorts"<<endl;
    for (int i = 0; i < cohort_all.size();i++) {
      delete cohort_all[i];
    }
    cohort_all.clear();

    for (int k = 0; k < newCohort_all.size();k++) {
      delete newCohort_all[k];
    }
    newCohort_all.clear();
//cout<<" End of Deleating cohorts"<<endl;
 
  int asID = 0;  // index in the ageStruct vector
  int kk=0;


  map<int, vector<double> > ageStructData;
  {

//cout<<"before reading file..."<<endl;
//    ifstream fin("ageStructData.txt");
    ifstream fin("ageStructDataJRC.txt");
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




  dataDetStruct::iterator iter = data_all.begin();
  while (iter != data_all.end()) {
   if (regions.find(iter->POLESREG[2000]) != regions.end()) { // Test only some regions
    if (iter->PROTECT[2000]==0) {

        int xi = (iter->x);
        int yi = (iter->y);
//        double X = (iter->x)*0.5+0.25-180;
//        double Y = (iter->y)*0.5+0.25-90;
        int Country = (int)iter->COUNTRY.v();

        int beyear = (int)coeff.bYear;

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
//        int optimumType = 3;
//        int MAIRot = 0;  //MG: optimal rotation time (see above)
//        int optRotUnmanaged = 0;
//        int rotationTimeCurr = 0; 
        int Rotation = 0;
//        int rotMaxBm = 0;
//        int rotMaxBmTh = 0;
        int biomasRot=0;  // MG: rotation time fitted to get certain biomass under certain MAI (w/o thinning)
        int biomasRotTh=0;  // MG: rotation time fitted to get certain biomass under certain MAI (with thinning)
//        if (refForShare >0 && iter->CABOVEHA"].v() > 0 && maiForest.get(xi,yi)> 0) {
        if (iter->CABOVEHA.v() > 0 && maiForest.get(xi,yi)> 0) {
          biomasRot = fi.gU(iter->CABOVEHA.v(), maiForest.get(xi,yi), 1);
//          biomasRotTh = fi.gUt(iter->CABOVEHA"].v(), maiForest.get(xi,yi), 1); // with thinning
          biomasRotTh = fi.gUt(iter->CABOVEHA.v(), maiForest.get(xi,yi), thinningForest.get(xi,yi)); // with thinning
          if (iter->FOREST.v()>0) forFlag = 1.0;
        }
//        if (maiForest.get(xi,yi)> 0){
//        MAIRot = fi.gTopt(maiForest.get(xi,yi), 1);
//        rotMaxBm = fi.gTopt(maiForest.get(xi,yi), 2);                        
//        rotMaxBmTh = fi.gTopt(maiForest.get(xi,yi), 3);
//        }
//        rotationTimeCurr = rotationForest.get(xi,yi);
        if (biomasRot < 0) biomasRot = 0;
//        if (MAIRot < 0) MAIRot = 0;
//        if (optRotUnmanaged < 0) optRotUnmanaged = 0;
//        if (aveRot < 0) aveRot = 0;
        if (thinningForest.get(xi,yi) >0) {
          Rotation = biomasRotTh+1; 
        } else {
          Rotation = biomasRot+1;
        }

// Existing (old forest)
//        g4m::ageStruct *cohort = new g4m::ageStruct(&fi, sws, hlv, hle, dbv, dbe, 0, 0, maiForest.get(xi,yi), Rotation, thinningForest.get(xi,yi),forFlag, 0.75);

     // Initialize age structure
//    g4m::ageStruct *cohort = new g4m::ageStruct(&fi, sws, hlv, hle, dbv, dbe, 0, 0, 0, 1, thinningForest.get(xi,yi),0, 0.75);
    g4m::ageStruct *cohort = new g4m::ageStruct(&fi, sws, hlv, hle, dbv, dbe, 0, 0, 0, 1, thinningForest.get(xi,yi),0, 0.5);
    cohort->setMai(maiForest.get(xi,yi));

  map<int, vector<double> >::iterator PageStructData = ageStructData.find(Country);
  if((PageStructData != ageStructData.end()) && thinningForest.get(xi,yi)>0 && forFlag > 0) {
//  if((PageStructData != ageStructData.end()) && thinningForest.get(xi,yi)>0 && forFlag > 1000) { // Don't use Age Struct Data!!!
    cohort->createNormalForest(161, 0., 1.);
//    double ageBreaks[] = {10.,20.,40.,60.,80.,100.,120.,140.,999.};
//    double ageSize[] = {11.,10.,20.,20.,20.,20.,20.,20.,20.};
//    int ageGroups = 9;
    double ageBreaks[] = {10.,20.,30.,40.,50.,60.,70.,80.,90.,100.,110.,120.,130.,140.,150.,999.};
    double ageSize[] = {11.,10.,10.,10.,10.,10.,10.,10.,10.,10.,10.,10.,10.,10.,10.,10.};
    int ageGroups = 16;

    int ageGroup = 0;
    for(int i=0; i<161; ++i) {
      while(i>ageBreaks[ageGroup] && ageGroup+1 < ageGroups) {++ageGroup;}
      cohort->setArea(i,PageStructData->second[ageGroup]/ageSize[ageGroup]);
    }
    double area = cohort->getArea();
    double biomass = cohort->getBm();
    if(area > 0. && biomass > 0.) {
      double stockingDegree = iter->CABOVEHA.v()/(cohort->getBm()/cohort->getArea());
      if(stockingDegree < 0.) {stockingDegree = 0.;}
//cout<< "countryIn = "<<Country<<"\tarea = "<<cohort->getArea()<<"\tbm = "<<cohort->getBm()<<"\t stockingDegree = "<<stockingDegree<<endl;
      cohort->setStockingdegree(stockingDegree);
      thinningForest.set(xi,yi, stockingDegree);
      for(int i=0; i<161; ++i) {
	cohort->setBm(i, stockingDegree*cohort->getBm(i));
      }
      cohort->setRotPeriod(161);
      cohort->aging();
    }
//    cohort->setRotPeriod(fi.gUt(cohort->getBm()/cohort->getArea(), maiForest.get(xi,yi), 1));
    Rotation = fi.gUt(cohort->getBm()/cohort->getArea(), maiForest.get(xi,yi), thinningForest.get(xi,yi))+1;
    cohort->setRotPeriod(Rotation);  
//    cohort->setRotPeriod(rotationForest.get(xi,yi));
//      rotationForest.set(xi,yi,Rotation);
  } else {
    cohort->createNormalForest(Rotation, forFlag, thinningForest.get(xi,yi));
    cohort->setStockingdegree(thinningForest.get(xi,yi));         
    cohort->setRotPeriod(Rotation);
  }            

// New forest (planted/afforested)
//cout<< "Create new cohort"<< endl;
        g4m::ageStruct *newCohort = new g4m::ageStruct(&fi, sws, hlv, hle, dbv, dbe, 0, 0, maiForest.get(xi,yi), Rotation, thinningForest.get(xi,yi),0, 0.75); 
        g4m::ageStruct::v newRes; // MG: results vector for the new (planted/afforested) forest
//cout<< "End Create new cohort"<< endl;
// saving results to initial vectors
//cout<<"Push back cohorts"<<endl;
        cohort_all.push_back(cohort);
        newCohort_all.push_back(newCohort);
        iter->asID = asID;
        asID++;


    }     // End for IF unprotected 
  }      // Test only some regions 
    iter++;
kk++;
  }       // End of WHILE cells loop
// cout << "End initCohorts"<<endl;
 }
