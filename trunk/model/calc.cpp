#include "forest.h"
#include "dima.h"

extern g4m::coeffStruct coeff;
extern ofstream fff;

extern map<string, g4m::ipol<double,double> > lprice;
extern map<string, g4m::ipol<double,double> > wprice;
extern map<string, g4m::ipol<double,double> > wprod;

extern set<int> countriesList;

extern double Hurdle_opt[NumberOfCountries];
extern double afforRate_opt[NumberOfCountries];
extern double deforRate_opt[NumberOfCountries];
extern double EmissionsCurCountry[NumberOfCountries+1];
extern double EmissionsCurAfforCountry[NumberOfCountries+1];

extern float minRotNPV[NumberOfCountries];
extern float minMedNPV[NumberOfCountries];
extern float medRotNPV[NumberOfCountries];
extern float medMaxNPV[NumberOfCountries];
extern float maxRotNPV[NumberOfCountries];
extern float minRot[NumberOfCountries];
extern float maxRot[NumberOfCountries];

extern int byear, refYear;

extern double resUse;
extern double deflator;

extern bool cellInteract;
extern bool forNPVcurves;
extern bool forNPVcuvvesDyn;
extern bool gridOutput;

extern countryData CountriesNforCover;
extern countryData CountriesNforTotC;
extern countryData CountriesAfforHaYear;  
extern countryData CountriesAfforCYear;  
extern countryData CountriesAfforCYear_ab;  
extern countryData CountriesAfforCYear_bl;  
extern countryData CountriesAfforCYear_biom;  
extern countryData CountriesAfforCYear_dom;    
extern countryData CountriesAfforCYear_soil;    
extern countryData CountriesOforCover;
extern countryData CountriesDeforHaYear;  
extern countryData CountriesOfor_abC;
extern countryData CountriesOforC_biom; 
extern countryData CountriesDeforCYear;  
extern countryData CountriesDeforCYear_bl;   
extern countryData CountriesDeforCYear_ab; 
extern countryData CountriesDeforCYear_biom;
extern countryData CountriesDeforCYear_dom;   
extern countryData CountriesDeforCYear_soil;     
extern countryData CountriesWoodHarvestM3Year;    
extern countryData CountriesWoodHarvestPlusM3Year; 
extern countryData CountriesWoodHarvestFmM3Year;
extern countryData CountriesWoodHarvestDfM3Year;
extern countryData CountriesWoodLoosCYear;   
extern countryData CountriesHarvLossesYear;
extern countryData CountriesManagedForHa;     
extern countryData CountriesManagedCount;  
extern countryData CountriesMAI;    
extern countryData CountriesCAI;    
extern countryData CountriesCAI_new;      
extern countryData CountriesFM;   
extern countryData CountriesFMbm;     
extern countryData CountryRotation; 
extern countryData CountriesWprod;
extern countryData CountriesProfit;
extern countryData CountryregWoodHarvestM3Year; 
extern countryData CountryregWoodHarvestFmM3Year;
extern countryData CountryregWoodHarvestDfM3Year;  
extern countryData CountryregWprod;
extern countryData CountryregRotation; 

extern griddata2<double> SD_grid_1990;        
extern griddata2<double> SD_grid_00;        
extern griddata2<double> SD_grid_10;        
extern griddata2<double> harvestm3_grid_20;     
extern griddata2<double> harvestm3_grid_30; 
extern griddata2<double> harvestm3_grid_50;     
extern griddata2<double> bmabtC_of_grid_20;         
extern griddata2<double> bmabtC_of_grid_30;   
extern griddata2<double> bmabtC_of_grid_50;             
extern griddata2<double> bmabtC_nf_grid_20;         
extern griddata2<double> bmabtC_nf_grid_30;   
extern griddata2<double> bmabtC_nf_grid_50;
extern griddata2<double> fmgGco2_grid_20; 
extern griddata2<double> fmgGco2_grid_30;     
extern griddata2<double> fmgGco2_grid_50;         
extern griddata2<double> mai_m3ha_grid_20;        
extern griddata2<double> mai_m3ha_grid_30;        
extern griddata2<double> mai_m3ha_grid_50;        
extern griddata2<double> SD_grid_20;        
extern griddata2<double> SD_grid_30;        
extern griddata2<double> SD_grid_50;    
extern griddata2<double> RL_grid_20;        
extern griddata2<double> RL_grid_30;        
extern griddata2<double> RL_grid_50; 


void calc(g4m::dataStruct &it, g4m::incrementTab &fi, g4m::ageStruct &cohort, g4m::ageStruct &newCohort,
          dat &singleCell, griddataLite<char> &managedForest, griddata2<double> &maiForest, griddata2<double> &rotationForest,
          griddata2<double> &rotationForestNew, griddata2<double> &thinningForest, griddata2<double> &thinningForestNew, 
          griddata2<double> &harvestGrid, int year, double priceC, int asID, griddata2<double> &OforestShGrid)
{
	double EmissionsCur=0.;
	double EmissionsProductCur= 0.;  
	double EmissionsLitterCur = 0.;  
	double EmissionsSOCCur = 0.;      
	double EmissionsSlashBurnCur = 0.;
	double EmissionsDeadBurnCur = 0.;
	double EmissionsCRootBurnCur = 0.;
	double EmissionsSOCAfforCur = 0.; 
	double EmissionsLitterAfforCur = 0.;
	double EmissionsAfforCur = 0.;
	//  double EmissionsCurNP=0.;
	//  double EmissionsProductCurNP= 0.;  
	//  double EmissionsLitterCurNP = 0.;  
	//  double EmissionsSOCCurNP = 0.;      
	//  double EmissionsSlashBurnCurNP = 0.;
	//  double EmissionsDeadBurnCurNP = 0.;
	//  double EmissionsCRootBurnCurNP = 0.;
	//  double EmissionsSOCAfforCurNP = 0.; 
	//  double EmissionsLitterAfforCurNP = 0.;
	//  double EmissionsAfforCurNP = 0.; 
	double PlantPhytHaBmGr = 0.;
	//  double PlantPhytHaBmGrNP =0.;
	double PlantPhytHaBmGrG =0.;
	double PlantPhytHaBlGr = 0.;
	//  double PlantPhytHaBlGrNP = 0.;

	double sawnW = 0.;
	double restW = 0.;
	double sawnThW = 0.;
	double restThW = 0.;
	double sawnWlost = 0.;
	double restWlost = 0.;
	double sawnThWlost = 0.;
	double restThWlost = 0.;
	double sawnWnew = 0.;
	double restWnew = 0.;
	double sawnThWnew = 0.;
	double restThWnew = 0.;
	double sawnWlostNew = 0.;
	double restWlostNew = 0.;
	double sawnThWlostNew = 0.;
	double restThWlostNew = 0.;
	double forNPV_CH = 0.;
	double abBiomassO = 0.;
	double harvLlost = 0.;
	double harvLlostNew = 0.;
	double harvL = 0.;
	double harvLNew = 0.;

	double harvWood = 0.; // Total current harvested wood in the cell, m3
	double harvWoodLost = 0.; // Total current "lost" wood in the cell, tC (in remote forests)
	double harvWoodNew = 0.; // Total current harvested wood in the cell, m3
	double harvWoodLostNew = 0.; // Total current "lost" wood in the cell, tC (in remote forests)

	double harvWoodPlus = 0.; // harvestable wood (m3/ha) including 50% of residues
	double harvWoodNewPlus = 0.; // harvestable wood (m3/ha) including 50% of residues  

	double defBiomass = 0.;

	double harvestFcM3 = 0.; // current harvested wood from final cut in the cell, m3/ha
	double harvestThM3 =0.; // current harvested wood from thinning in the cell, m3/ha
	double harvestTotPlusM3 = 0.; //Total current harvested wood in the cell, m3, including residues (harvest losses)
	double CAI_m3ha = 0.; //Current annual increment of old forest m3/ha
	double CAI_new_m3ha = 0.; //Current annual increment of new forest (planted after byear) m3/ha

	double harvMAI =0.;  
	double fval = 0.; // Net present value of forestry
	double TimberPrice = 0.;  
	double aval = 0.;

	double deforestHa = 0.; // deforestation rate ha/year
	double afforestHa = 0.; // afforestation rate ha/year

	double phytNewIncr = 0.; 
	double FMsink_ab = 0.;
	double FMsink_bm = 0.;

	double harvestDfM3Ha = 0.; // wood obtained at deforestation
	double deforWoodTotM3 = 0.;
	double harvestFmTotM3 = 0.;
	double harvestTotM3 = 0.;

	double CurPlantPhytBmGr;
	double CurPlantPhytBlGr;
	double plantingCosts;

	//----------Initialise cohorts-----------------
	int rotationTimeCurr;
	int rotMAI = 0;
	int biomassRot = 0;

	//  decision.setYear(year);
	//MG: setup forest Age


	int Age = year-byear;
	int xi = (it.x);
	int yi = (it.y);
	int Country = (int)it.COUNTRY[2000];

	//  double forestShare0 = 0;
	//   if (it.FOREST.v()+(it.CROP.v())+(it.BUILTUP.v())>1)
	//      {forestShare0 = (1-(it.CROP.v()+it.BUILTUP.v()));}
	//   else {forestShare0 = it.FOREST.v();}  

	double OforestShare = singleCell.OforestShare;
	double AforestShare = singleCell.AforestShare;
	if (OforestShare < 0.) {OforestShare = 0.;}	       
	if (AforestShare < 0.) {AforestShare = 0.;}	   

	if (year>byear)
	{
		rotationTimeCurr = rotationForest.get(xi,yi);
		singleCell.Rotation = rotationTimeCurr;
		if (year == byear)
		{
			cohort.setRotPeriod(rotationTimeCurr);       
			//  cohort.setStockingdegree(thinningForest.get(xi,yi));

			newCohort.setRotPeriod(rotationForest.get(xi,yi));       
			//  newCohort.setStockingdegree(thinningForest.get(xi,yi));

			rotationForestNew.set(xi,yi,rotationForest.get(xi,yi));
			thinningForestNew.set(xi,yi,thinningForest.get(xi,yi));
		}

		g4m::ageStruct::v res = cohort.aging(); //MG

		g4m::ageStruct::v newRes = newCohort.aging();

		if (thinningForest.get(xi,yi)<0)
		{
			//   if (Country == 33 || Country == 156) {  
				if (OforestShare>0)
				{
					sawnWlost = res.enSw ;           // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
					restWlost = res.enRw ;           // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
					sawnThWlost = res.vnSw ;         // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
					restThWlost = res.vnRw ;         // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
					harvLlost = res.hv;
				}
				if (AforestShare>0)
				{
					sawnWlostNew = newRes.enSw;      // MG: get harvestable sawnwood for the set (new) forest tC/ha for final cut. 
					restWlostNew = newRes.enRw;      // MG: get harvestable restwood for the set (new) forest tC/ha for final cut. 
					sawnThWlostNew = newRes.vnSw;    // MG: get harvestable sawnwood for the set (new) forest tC/ha for thinning. 
					restThWlostNew = newRes.vnRw;    // MG: get harvestable restwood for the set (new) forest tC/ha for thinning.
					harvLlostNew = newRes.hv;        
				}
		}
		else if (thinningForest.get(xi,yi)>0)
		{
			if (OforestShare>0)
			{
				sawnW = res.enSw ;           // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
				restW = res.enRw ;           // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
				sawnThW = res.vnSw ;         // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
				restThW = res.vnRw ;         // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
				harvL = res.hv;              // MG: get harvest losses
			}
			if (AforestShare>0)
			{
				sawnWnew = newRes.enSw;      // MG: get harvestable sawnwood for the set (new) forest tC/ha for final cut. New (planted) forest
				restWnew = newRes.enRw;      // MG: get harvestable restwood for the set (new) forest tC/ha for final cut. 
				sawnThWnew = newRes.vnSw;    // MG: get harvestable sawnwood for the set (new) forest tC/ha for thinning. 
				restThWnew = newRes.vnRw;    // MG: get harvestable restwood for the set (new) forest tC/ha for thinning.
				harvLNew = newRes.hv;                
			}
		}
		// }else{
		//    sawnW = res.enSw ;           // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
		//    restW = res.enRw ;           // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
		//    sawnThW = res.vnSw ;         // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
		//    restThW = res.vnRw ;         // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.
		//    sawnWnew = newRes.enSw;      // MG: get harvestable sawnwood for the set (new) forest tC/ha for final cut. New (planted) forest
		//    restWnew = newRes.enRw;      // MG: get harvestable restwood for the set (new) forest tC/ha for final cut. 
		//    sawnThWnew = newRes.vnSw;    // MG: get harvestable sawnwood for the set (new) forest tC/ha for thinning. 
		//    restThWnew = newRes.vnRw;    // MG: get harvestable restwood for the set (new) forest tC/ha for thinning.
		//    harvL = res.hv;
		//    harvLNew = newRes.hv;           
		// }        

		// SCaling to the area=1ha (to take into account deforestation)
		double relAreaO = 0.; 
		if (OforestShare>0)
		{
			relAreaO = cohort.getArea();
			if (relAreaO > 0)
			{
				abBiomassO = cohort.getBm();
				harvWood = (sawnW + restW + sawnThW + restThW) * it.FTIMBER.v(); // Total current harvested wood in the cell, m3
				harvWoodLost = (sawnWlost + restWlost + sawnThWlost + restThWlost + harvLlost); // Total current "lost" wood in the cell, tC (in remote forests)
				harvWoodNew = (sawnWnew + restWnew + sawnThWnew + restThWnew) * it.FTIMBER.v(); // Total current harvested wood in the cell, m3
				harvWoodLostNew = (sawnWlostNew + restWlostNew + sawnThWlostNew + restThWlostNew + harvLlostNew); // Total current "lost" wood in the cell, tC (in remote forests)
				harvWoodPlus = harvWood + resUse * harvL * it.FTIMBER.v(); // harvestable wood (m3/ha) including 50% of residues
				harvWoodNewPlus = harvWoodNew + resUse * harvLNew * it.FTIMBER.v(); // harvestable wood (m3/ha) including 50% of residues  
				if (relAreaO < 1.)
				{
					abBiomassO /= relAreaO;
					//cout<<"asID=\t"<<asID<<"\tyear=\t"<<year<<"\trelAreaO=\t"<<relAreaO<<"\tabBiomassO=\t"<<abBiomassO<<endl;               
				}
			}
		}

		// Rotation time fitted to get certain biomass under certain MAI (w/o thinning)
		//  int biomasRot = 0;
		if (singleCell.forestShare > 0 && it.CABOVEHA.v() > 0 && maiForest.get(xi,yi)> 0)
		{
			//    biomasRot = fi.gU(it.CABOVEHA"].v(), maiForest.get(xi,yi), 1);
			rotMAI = fi.gTopt(maiForest.get(xi,yi), 1);
		}
		//  if (biomasRot < 0) biomasRot = 0;
		//  if ((biomasRot == 0)||(harvWood < 0)) harvWood = 0.;
		if ((rotationTimeCurr<= 0)||(harvWood < 0)) harvWood = 0.;
		// MG: setup price of carbon
		//  double PriceCi = 0;
		if (year > refYear)
		{
			coeff.PriceC.clear();
			////  coeff.PriceC.insert(0, PriceCi * LinPrice2020[Age] * it.CORRUPTION"].v());
			coeff.PriceC.insert(0, priceC * it.CORRUPTION.v());
		}
		string regprice, regprice0;
		char prstr[5];
		char regstr[3];
		int2str((int)it.POLESREG.v(),regstr);
		//  if (PriceCi!=0) {
		//    int2str(PriceC,prstr);
		//    regprice = "re"+string(regstr)+"price"+ string(prstr);
		//  } else 
		{
			regprice = "re"+string(regstr)+"price0";
		}
		regprice0 = "re"+string(regstr)+"price0";
		//  {
		//// ---- Checking NPV of forestry for current harvest------------       
		//    dima decision(year
		//         , it.NPP
		//         , it.SPOPDENS
		//         , it.SAGRSUIT
		//         , it.PRICEINDEX
		//		 , coeff.PriceIndexE
		//		 , it.R
		//		 , coeff.PriceC
		//		 , coeff.PlantingCostsR
		//		 , coeff.PriceLandMinR
		//		 , coeff.PriceLandMaxR
		//		 , coeff.MaxRotInter
		//		 , coeff.MinRotInter
		//		 , coeff.decRateL
		//		 , coeff.decRateS
		//		 , it.FRACLONGPROD
		//		 , coeff.baseline
		//		 , it.FTIMBER
		//		 , coeff.PriceTimberMaxR
		//		 , coeff.PriceTimberMinR
		//		 , coeff.FCuptake
		//         , it.GDP
		//         , coeff.HarvLoos
		//         , forestShare0
		//         , wprice[regprice]
		//         , wprice[regprice0].v(2000)
		//         , rotationTimeCurr
		//         , harvWood + harvWoodLost * it.FTIMBER.v() );
		//    if (year<=2005) {
		//      forNPV_CH = decision.forVal(); // MG: use internal NPV
		//    } else {
		//      forNPV_CH = decision.forValExt();
		//    } // MG: use External NPV
		//    if (forNPV_CH < 0 || forNPV_CH == 0 || forNPV_CH > 0) 
		//   {forNPV_CH=forNPV_CH;
		//   }else{forNPV_CH=0;}
		////-------------------------------------------------------------------------------
		//  }
		//  dima decision(year
		//		       , it.NPP"]
		//		       , it.SPOPDENS"]
		//		       , it.SAGRSUIT"]
		//		       , it.PRICEINDEX"]
		//		       , coeff.PriceIndexE
		//		       , it.R"]
		//		       , coeff.PriceC
		//		       , coeff.PlantingCostsR
		//		       , coeff.PriceLandMinR
		//		       , coeff.PriceLandMaxR
		//		       , coeff.MaxRotInter
		//		       , coeff.MinRotInter
		//		       , coeff.decRateL
		//		       , coeff.decRateS
		//		       , it.FRACLONGPROD"]
		//		       , coeff.baseline
		//		       , it.FTIMBER"].v()
		//		       , coeff.PriceTimberMaxR
		//		       , coeff.PriceTimberMinR
		//		       , coeff.FCuptake
		//		       , it.GDP"]
		//		       , coeff.HarvLoos
		//		       , forestShare0
		//		       , wprice[regprice]
		//		       , wprice[regprice0].v(2000)
		//               , singleCell.Rotation
		//               , singleCell.potHarvest);
		//             

		harvMAI = maiForest.get(xi,yi)*it.FTIMBER.v()*(1-coeff.HarvLoos.v());

		dima decision(year
			, it.NPP
			, it.SPOPDENS
			, it.SAGRSUIT
			, it.PRICEINDEX
			, coeff.PriceIndexE
			, it.R
			, coeff.PriceC
			, coeff.PlantingCostsR
			, coeff.PriceLandMinR
			, coeff.PriceLandMaxR
			, coeff.MaxRotInter
			, coeff.MinRotInter
			, coeff.decRateL
			, coeff.decRateS
			, it.FRACLONGPROD
			, coeff.baseline
			, it.FTIMBER
			, coeff.PriceTimberMaxR
			, coeff.PriceTimberMinR
			, coeff.FCuptake
			, it.GDP
			, coeff.HarvLoos
			, OforestShare
			, wprice[regprice]
			, wprice[regprice0].v(2000)
			, rotMAI
			, harvMAI);

		//Timber price
		TimberPrice = decision.priceTimberExt(); // MG: use External TimberPrice

		//Forestry Value
		fval = decision.forValExt();
		if (fval < 0 || fval == 0 || fval > 0) 
		{
			fval=fval;
		}
		else
		{
			fval=0;
		}

		//Agricultural Value
		aval = decision.agrVal2000() * lprice[regprice].v(year)/lprice[regprice0].v(2000); // MG: use GLOBIOM price
		if (cellInteract) {aval = 2 * aval * (1 - OforestShGrid.GetMin(xi,yi));} // agriculture is more attractive if neighbor cells are deforested

		double crop = it.CROP.v(year);
		double builtup = it.BUILTUP.v(year);
		double maxfor = 1. - (builtup + crop);
		if (maxfor < 0.) maxfor = 0;
		//  double freeLand = maxfor - OforestShare;
		double spopdens = it.SPOPDENS.v(year);
		//defrorestation speed
		double defShare = 0.;
		//MG: Afforestation speed
		double affShare = 0.;

		double gdp = 1644.; // MG: gdp definition
		if (it.POPDENS.v(year) > 0.)
		{
			gdp = 200000. * it.GDP.v(year) / it.POPDENS.v(year);
			gdp = gdp * deflator; // Global GDP deflator GDP(1995)/GDP(2000)=8.807/10 (World Bank)
		}

		// Deforestation due to new infrastructure and minimal crop production
		//if (Country==68){
		//cout<<"asID=\t"<<asID<<"\tbeforeInfrastr OforestShare=\t"<<OforestShare<<"\tprevOForShare=\t"<<singleCell.prevOForShare<<"\tmaxfor=\t"<<maxfor<<"\tAforestShare=\t"<<AforestShare<<endl;               
		//}
		if (OforestShare + AforestShare > maxfor){OforestShare = maxfor-AforestShare;}
		if (OforestShare<0) OforestShare=0;
		//if (Country==68){
		//cout<<"asID=\t"<<asID<<"\tafterInfrastr OforestShare=\t"<<OforestShare<<"\tprevOForShare=\t"<<singleCell.prevOForShare<<"\tmaxfor=\t"<<maxfor<<"\tAforestShare=\t"<<AforestShare<<endl;               
		//}


		//MG: Deforestation due to agriculture and forestry land competition
		if(it.AGRSUIT.v(year) > 0 && OforestShare > 0)
		{
			defShare = 0.05/(1. + exp(1.799e+00
				+ 2.200e-01/OforestShare
				+ 1.663e-01/it.AGRSUIT.v(year)
				- 4.029e-02 * it.POPDENS.v(year)
				+ 5.305e-04 * it.POPDENS.v(year) * it.POPDENS.v(year)
				+ 1.282e-04 * gdp ));
			//cout<<"defshare= "<<defShare<<"\t deforRate_opt= "<<deforRate_opt[Country-1]<<"\t Country= "<<Country<<endl;
			defShare *= deforRate_opt[Country-1];
			//}

			// MG: End of gdp definition
			double defIncome = 0.;
			//Pay if Carbon get's to air (Harvest goes to products)
			double pDefIncome = abBiomassO *
	 					(TimberPrice * it.FTIMBER.v()
						* (1. -coeff.HarvLoos.v())
						- coeff.PriceC.v(year) * (1 + it.R.v(year))
						* (it.FRACLONGPROD.v(year) * coeff.decRateL.v(year)
						/ (coeff.decRateL.v(year) + it.R.v(year))
						+ it.FRACLONGPROD.v(year) * coeff.decRateS.v(year) //MG: mistake: must be DECRATES but not DECRATEL
						/ (coeff.decRateS.v(year) + it.R.v(year))));
			//*******************************************************************************************************
			/*if (asID >= 14) {
			cout << it.R"].v(year) << endl;
			cout << it.CLITTERHA"].v() << endl;
			cout << it.DECWOOD"].v(year) << endl;
			cout << it.DECHERB"].v(year) << endl;
			cout << it.CBELOWHA"].v() << endl;
			cout << it.SOCHA"].v() << endl;
			cout << it.DECSOC"].v(year) << endl;
			system("pause");
			}*/
			//*******************************************************************************************************
			double pDefLoss = - coeff.PriceC.v(year) * (1 + it.R.v(year))
							  * (it.CLITTERHA.v()*(0.3*it.DECWOOD.v(year)/(it.DECWOOD.v(year)+it.R.v(year))
							  + 0.7*it.DECHERB.v(year)/(it.DECHERB.v(year)+it.R.v(year))) 
							  + it.CBELOWHA.v()*0.3*it.DECHERB.v(year)/(it.DECHERB.v(year)+it.R.v(year))
							  + it.SOCHA.v()*it.DECSOC.v(year)/(it.DECSOC.v(year)+it.R.v(year)));
			//Immediate Pay if deforested (Slash and Burn)
			double sDefIncome = abBiomassO *
        						(TimberPrice * it.FTIMBER.v()
        						* (1. -coeff.HarvLoos.v())
        						- coeff.PriceC.v(year));
			double sDefLoss = - coeff.PriceC.v(year) * (it.CDEADHA.v()+ it.CBELOWHA.v(year)*0.7);
			defIncome = pDefIncome * (1. - it.SLASHBURN.v())
        				+ sDefIncome * it.SLASHBURN.v()
        				+ pDefLoss + sDefLoss;
			//  if ((aval + defIncome) > ((fval ) * Hurdle_opt[Country-1] ) && (OforestShare > 0)) {  // MG: adjust the multiplier to account for a forest saving policy in some countries
			if ((aval + defIncome) > (fval * Hurdle_opt[Country-1]))
			{ 
				OforestShare -= defShare; // Decrease Forest share
				//if (Country==68){
				//cout<<"asID=\t"<<asID<<"\tDefor OforestShare=\t"<<OforestShare<<"\tprevOForShare=\t"<<singleCell.prevOForShare<<"\tmaxfor=\t"<<maxfor<<"\tAforestShare=\t"<<AforestShare<<"\tdefShare=\t"<<defShare<<endl;               
				//}
				if (OforestShare > maxfor) OforestShare = maxfor;
				if (OforestShare < 0.) OforestShare = 0.;
			}
		} // end for if(it.AGRSUIT.v(year) > 0 && OforestShare > 0)

		//cout<<"NPP= "<< it.NPP"].v()<<"\t aval= "<<aval<<"\t fval= "<<fval<<"\t maxfor= "<<maxfor<<"\t potveg= "<<it.POTVEG"].v()<<"\t OforestShare= "<<OforestShare <<endl;

		//MG: Afforestation
		if (OforestShare + AforestShare < maxfor)
		{
			// MG: We afforest only places, where potential vegetation is forest
			if ((it.POTVEG.v())<9 && (it.NPP.v())>0)
			{ 
				affShare = 0.01/(1+exp(1.+0.1/it.AGRSUIT.v(year)+1/(0.001*gdp)));
				if (affShare < 1./(it.LANDAREA.v()*1000000.)) affShare = 0.;   // minimun one tree (1m^2)
				//cout<<"affshare= "<<affShare<<"\t afforRate_opt= "<<afforRate_opt[Country-1]<<endl;
				affShare *= afforRate_opt[Country-1]; 
				//cout<<"defshare= "<<defShare<<"\t affShare= "<<affShare<<endl;
				if (aval < (fval + coeff.PriceC.v()* (((it.SOCHA.v()*0.4/(8*decision.rotInter()))
                                                   + 5/(1.053*decision.rotInter()))/it.R.v())) * Hurdle_opt[Country-1])
				{
					AforestShare += affShare;
					//cout<< "AforestShare0= "<<AforestShare<<endl;
					//if (AforestShare > maxfor) AforestShare = maxfor;
					if (AforestShare < 0.)
					{
						AforestShare = 0.;
						affShare = 0.;
					}
					if (OforestShare + AforestShare > maxfor) AforestShare = maxfor - OforestShare;
					if (AforestShare < 0.) AforestShare = 0.;
					//if (Country==68){
					//cout<<"asID=\t"<<asID<<"\tAffor OforestShare=\t"<<OforestShare<<"\tprevOForShare=\t"<<singleCell.prevOForShare<<"\tmaxfor=\t"<<maxfor<<"\tAforestShare=\t"<<AforestShare<<"\taffShare=\t"<<affShare<<endl;               
					//}
					//cout<< "AforestShare2= "<<AforestShare<<endl;
				}
			}
		}
		else
		{ 
			//if (Country==68){
			//cout<<"asID=\t"<<asID<<"\tElse OforestShare=\t"<<OforestShare<<"\tprevOForShare=\t"<<singleCell.prevOForShare<<"\tmaxfor=\t"<<maxfor<<"\tAforestShare=\t"<<AforestShare<<endl;               
			//}
			OforestShare = maxfor - AforestShare;
			if (OforestShare<0) OforestShare=0;
		}
		if (OforestShare>singleCell.prevOForShare) OforestShare=singleCell.prevOForShare;    
		if (AforestShare<singleCell.AforestSharePrev) AforestShare=singleCell.AforestSharePrev;         
		//  if (OforestShare > maxfor) {OforestShare = maxfor;}
		//  if (OforestShare < 0.) {OforestShare = 0.;}	       
		//  if (AforestShare < 0.) {AforestShare = 0.;}	       
		//  if (AforestShare > maxfor) {AforestShare = maxfor;}	       
		//  if (OforestShare + AforestShare > maxfor) {AforestShare = maxfor - OforestShare;}
		//  if (AforestShare < 0.) {AforestShare = 0.;}	
		//  if (OforestShare + AforestShare > maxfor) {OforestShare = maxfor - AforestShare;} // MG: 28 Jan 2011 try to correct a bug with too high affor biomass
		//  if (OforestShare < 0.) {OforestShare = 0.;}	

		//cout<< "AforestShare3= "<<AforestShare<<endl;  	    
		singleCell.forestAgeShare[Age] = AforestShare - singleCell.AforestSharePrev;
		if (singleCell.forestAgeShare[Age] < 0.) {singleCell.forestAgeShare[Age]=0.;}
		//cout<<"singleCell.AforestSharePrev= "<<singleCell.AforestSharePrev<<"\t singleCell.forestAgeShare[Age]= "<<singleCell.forestAgeShare[Age]<<endl;
		newCohort.afforest(singleCell.forestAgeShare[Age]); // MG: Afforest

		singleCell.deforestA[Age] = singleCell.prevOForShare - OforestShare;
		if (singleCell.deforestA[Age] < 0.) {singleCell.deforestA[Age]=0.;}
		if (singleCell.deforestA[Age] > 1.) {singleCell.deforestA[Age]=1.;}

		if (year < 2000) // MG: We start deforestation in 2000, because we have initial data for 2000 (discussion with Hannes Bottcher 10.09.09)
		{
			OforestShare = singleCell.OforestShare;
			AforestShare = 0.;  
		}

		//  g4m::ageStruct::v resDefor = cohort.deforest(singleCell.deforestA[Age],0); //MG: deforest set forest and get deforested biomass. Devide by refForShare to get per ha value
		g4m::ageStruct::v resDefor = cohort.deforest(singleCell.deforestA[Age],0); //MG: deforest set forest and get deforested biomass. Devide by refForShare to get per ha value
		defBiomass = (resDefor.enSw + resDefor.enRw + resDefor.hv);///deforestA[Age]; // deforested aboveground biomass per ha // only stemwood !!!

		//if (defBiomass>0){
		//cout<<"asID=\t"<<asID<<"\tcountry=\t"<<Country<<"\tyear=\t"<<year<<"\tabBiomassO=\t"<<abBiomassO<<"\tdefBiomass=\t"<<defBiomass<<"\thv=\t"<<resDefor.hv<<endl;
		//}

		singleCell.OforestShare = OforestShare;
		singleCell.AforestShare = AforestShare;  
		singleCell.forestShare = OforestShare + AforestShare;
		deforestHa = singleCell.deforestA[Age]*singleCell.LandAreaHa;
		singleCell.deforestHaTot += deforestHa;
		afforestHa = singleCell.forestAgeShare[Age]*singleCell.LandAreaHa;
		singleCell.afforestHaTot += afforestHa;
		//  singleCell.ProdLongA[Age] = defBiomass*it.FRACLONGPROD.v()*(1-coeff.HarvLoos.v())*deforestHa;
		singleCell.ProdLongA[Age] = defBiomass*it.FRACLONGPROD.v()*(1-coeff.HarvLoos.v()) * (1. - it.SLASHBURN.v())*deforestHa;
		singleCell.ProdShortA[Age] = singleCell.ProdLongA[Age];
		singleCell.LitterA[Age] = it.CLITTERHA.v()*deforestHa;
		singleCell.FineRootA[Age] = it.CBELOWHA.v()*0.3*deforestHa;
		singleCell.SOCA[Age] = it.SOCHA.v()*deforestHa;
		singleCell.SOCaffor[Age] = it.SOCHA.v() * afforestHa;
		singleCell.LitterAffor[Age] = it.CLITTERHA.v() * afforestHa;
		//********* Emissions from deforestation *************
		for (int i=0;i<=Age;i++)
		{
			// Calculate Emissions from deforestation in current cell for current year caused by decomposition
			EmissionsProductCur = EmissionsProductCur + singleCell.ProdLongA[i]*coeff.decRateL.v()
					+ singleCell.ProdShortA[i]*coeff.decRateS.v();
			EmissionsLitterCur = EmissionsLitterCur + singleCell.LitterA[i]*0.3*it.DECWOOD.v(year)
					+ singleCell.LitterA[i]*0.7*it.DECHERB.v() + singleCell.FineRootA[i]
					* it.DECHERB.v(year);
			if (singleCell.SOCA[i] >= it.SOCHA.v()*0.6*singleCell.deforestA[i]*singleCell.LandAreaHa)
			{
				EmissionsSOCCur = EmissionsSOCCur + singleCell.SOCA[i] * it.DECSOC.v(year);
			}  
			// MG: Recalculate carbon pools
			singleCell.ProdLongA[i] = singleCell.ProdLongA[i]*(1-coeff.decRateL.v());
			singleCell.ProdShortA[i] = singleCell.ProdShortA[i]*(1-coeff.decRateS.v());
			singleCell.LitterA[i] = singleCell.LitterA[i]*0.3*(1-it.DECWOOD.v())
								  + singleCell.LitterA[i]*0.7*(1-it.DECHERB.v());
			singleCell.FineRootA[i] = singleCell.FineRootA[i]*(1-it.DECHERB.v());
			singleCell.SOCA[i] = singleCell.SOCA[i]*(1-it.DECSOC.v(year));
			if (singleCell.SOCA[i]<it.SOCHA.v()*0.6*singleCell.deforestA[i])
			{
				singleCell.SOCA[i] = it.SOCHA.v()*0.6*singleCell.deforestA[i]*singleCell.LandAreaHa; // SOCagr;
			}
		}
		// Emissions from deforestation in current cell for current year caused by burning  
		EmissionsSlashBurnCur = EmissionsSlashBurnCur + abBiomassO*it.SLASHBURN.v(year)* deforestHa;
		EmissionsDeadBurnCur = EmissionsDeadBurnCur + it.CDEADHA.v()* deforestHa;
		EmissionsCRootBurnCur = EmissionsCRootBurnCur + it.CBELOWHA.v(year)*0.7 * deforestHa;
		//Emissions for current cell summed over years
		singleCell.EmissionsProduct += EmissionsProductCur;
		singleCell.EmissionsLitter += EmissionsLitterCur;
		singleCell.EmissionsSOC += EmissionsSOCCur;
		singleCell.EmissionsSlashBurn += EmissionsSlashBurnCur;
		singleCell.EmissionsDeadBurn += EmissionsDeadBurnCur;
		singleCell.EmissionsCRootBurn += EmissionsCRootBurnCur;
		//Total emissions in current cell for current year
		EmissionsCur = EmissionsProductCur+EmissionsLitterCur+EmissionsSOCCur+EmissionsSlashBurnCur
				   + EmissionsDeadBurnCur+EmissionsCRootBurnCur;
		//Total emissions in current cell summed over years       
		singleCell.EmissionsTot += EmissionsCur;    
		EmissionsCurCountry[Country] += EmissionsCur;
		//*************** END Emissions from deforestation ****************
		//*************** Afforestation "negative" emissions block ********
		//double sumAgeShare = 0.;
		if (AforestShare>0)
		{
			for (int ia=0; ia<Age; ia++)
			{
				if (singleCell.forestAgeShare[ia]>0)
				{
					double abovePhCur = newCohort.getBm(Age-ia) * singleCell.forestAgeShare[ia]; // afforested biomass per ha * afforShare
					PlantPhytHaBmGr += abovePhCur;
					//if (((((it.x)*GridStepLon+GridStepLon/2-180) ==-7.75) && (((it.y)*GridStepLat+GridStepLat/2-90)==10.25))
					//                   || ((((it.x)*GridStepLon+GridStepLon/2-180) ==-55.75) && (((it.y)*GridStepLat+GridStepLat/2-90)==4.75)))
					//{
					//sumAgeShare+=singleCell.forestAgeShare[ia];                   
					//cout<<"x=\t"<<(it.x)*GridStepLon+GridStepLon/2-180<<"\ty=\t"<<(it.y)*GridStepLat+GridStepLat/2-90<<"\tyear=\t"<<year;
					//cout<<"\tia=\t"<<ia<<"\tabovePhCur=\t"<<abovePhCur<<"\tforestAgeShare[ia]=\t"<<singleCell.forestAgeShare[ia];
					//cout<<"\Bm(ia)=\t"<<newCohort.getBm(Age-ia)<<"\tPlantPhytHaBmGr\t"<<PlantPhytHaBmGr<<"\tsumAgeShare=\t"<<sumAgeShare<<endl;
					//}
					if (singleCell.LitterAffor[ia]<5*singleCell.forestAgeShare[ia]*singleCell.LandAreaHa)
					{
						double CurEmissionsLitterAfforCur = 0.95 * pow((1.-exp(-0.1*abovePhCur/singleCell.forestAgeShare[ia])),3)
										* singleCell.forestAgeShare[ia]*singleCell.LandAreaHa;
						EmissionsLitterAfforCur += CurEmissionsLitterAfforCur;
						singleCell.LitterAffor[ia] += CurEmissionsLitterAfforCur;
					}
					if (singleCell.SOCaffor[ia]<=it.SOCHA.v()*1.4* singleCell.forestAgeShare[ia]*singleCell.LandAreaHa)
					{
						if (it.POTVEG.v()==4||it.POTVEG.v()==6)
						{
							EmissionsSOCAfforCur += 0.04 * pow((1.-exp(-1.2*singleCell.LitterAffor[ia]
									/ (singleCell.forestAgeShare[ia]*singleCell.LandAreaHa))),3) 
									* singleCell.forestAgeShare[ia]*singleCell.LandAreaHa;
							singleCell.SOCaffor[ia] += 0.04 * pow((1.-exp(-1.2*singleCell.LitterAffor[ia]
									/ (singleCell.forestAgeShare[ia]*singleCell.LandAreaHa))),3)
									* singleCell.forestAgeShare[ia]*singleCell.LandAreaHa;                   
						}
						if (it.POTVEG.v()==8)
						{
							EmissionsSOCAfforCur += 0.2 * pow((1.-exp(-1.2*singleCell.LitterAffor[ia]
									/ (singleCell.forestAgeShare[ia]*singleCell.LandAreaHa))),3) 
									* singleCell.forestAgeShare[ia]*singleCell.LandAreaHa;
							singleCell.SOCaffor[ia] += 0.2 * pow((1.-exp(-1.2*singleCell.LitterAffor[ia]
								   / (singleCell.forestAgeShare[ia]*singleCell.LandAreaHa))),3) 
								   * singleCell.forestAgeShare[ia]*singleCell.LandAreaHa; 
						}
						else
						{
							EmissionsSOCAfforCur += 0.35 * pow((1.-exp(-1.2*singleCell.LitterAffor[ia]
									/ (singleCell.forestAgeShare[ia]*singleCell.LandAreaHa))),3) 
									* singleCell.forestAgeShare[ia]*singleCell.LandAreaHa; 
							singleCell.SOCaffor[ia] += 0.35 * pow((1.-exp(-1.2*singleCell.LitterAffor[ia]
									/ (singleCell.forestAgeShare[ia]*singleCell.LandAreaHa))),3) 
									* singleCell.forestAgeShare[ia]*singleCell.LandAreaHa;
						}
					} // End for if SOCaffor <= ...
				} // End for if forestShare > 0
			} // End for age loop
		} // End for if AforestShare>0
		//if (((((it.x)*GridStepLon+GridStepLon/2-180) ==-7.75) && (((it.y)*GridStepLat+GridStepLat/2-90)==10.25))
		//                   || ((((it.x)*GridStepLon+GridStepLon/2-180) ==-55.75) && (((it.y)*GridStepLat+GridStepLat/2-90)==4.75)))
		//{
		//for (int ia=0;ia<Age;ia++){
		//cout<<"x=\t"<<(it.x)*GridStepLon+GridStepLon/2-180<<"\ty=\t"<<(it.y)*GridStepLat+GridStepLat/2-90;    
		//cout<<"\tafter loop year=\t"<<year<<"\tia=\t"<<ia<<"\tforestAgeShare[ia]=\t"<<singleCell.forestAgeShare[ia];
		//cout<<"\t(Age-ia)=\t"<<(Age-ia)<<"\tBm(Age-ia)=\t"<<newCohort.getBm(Age-ia)<<"\tPlantPhytHaBmGr\t"<<PlantPhytHaBmGr<<endl;
		//}
		//}
		//     Belowground phytomass of planted forest = 18% for tropical, 22 for temperate and 25 for boreal of aboveground phytomass
		//  PlantPhytHaBmGr = newCohort.getBm();
		double coeffBl = 0.22;
		if (it.POTVEG.v()==1 || it.POTVEG.v()==2){coeffBl = 0.18;}
		if (it.POTVEG.v()==6 || it.POTVEG.v()==7) {coeffBl = 0.25;}
		if (it.POTVEG.v()==3 || it.POTVEG.v()==4 || it.POTVEG.v()==5||(it.POTVEG.v()==8)) {coeffBl = 0.22;}
		PlantPhytHaBlGr = PlantPhytHaBmGr * coeffBl;

		CurPlantPhytBmGr=(PlantPhytHaBmGr-singleCell.prevPlantPhytHaBmGr) * singleCell.LandAreaHa;
		CurPlantPhytBlGr=(PlantPhytHaBlGr-singleCell.prevPlantPhytHaBlGr) * singleCell.LandAreaHa;
		// Emissions in current cell summed over years 
		singleCell.EmLitterAffor += EmissionsLitterAfforCur;
		singleCell.EmSOCAffor += EmissionsSOCAfforCur;
		// Total (negative) emissions from afforestation for current year and current cell
		EmissionsAfforCur = (CurPlantPhytBmGr+CurPlantPhytBlGr) + EmissionsLitterAfforCur+EmissionsSOCAfforCur;
		EmissionsCurAfforCountry[Country] += EmissionsAfforCur;
		//Total emissions in current cell summed over years       
		singleCell.EmissionsAffor += EmissionsAfforCur;
		//////////////////// END Afforestation "negative" emissions block                

		//if (Country==11 && year>2010){ cout<<"countryCalc = \t"<<Country<<"\t deforWoodTotM3Prev = \t"<<singleCell.deforWoodTotM3<<endl;       }

		//double deforWoodTotM3 = (defBiomass-(1-resUse)*resDefor.hv) * singleCell.deforestA[Age] * it.FTIMBER.v() * singleCell.LandAreaHa;
		//double deforWoodTotM3 = (defBiomass-resDefor.hv)*singleCell.deforestA[Age]*it.FTIMBER.v()*singleCell.LandAreaHa;
		//double harvestTotM3 = (harvWood*OforestShare+harvWoodNew*singleCell.AforestSharePrev+(defBiomass-resDefor.hv)*singleCell.deforestA[Age]*it.FTIMBER.v())*singleCell.LandAreaHa;

		harvestDfM3Ha = (defBiomass-resDefor.hv)*singleCell.deforestA[Age]*(1.-it.SLASHBURN.v()) * it.FTIMBER.v(); // wood obtained at deforestation
		deforWoodTotM3 = harvestDfM3Ha * singleCell.LandAreaHa;
		harvestFmTotM3 = (harvWood*OforestShare+harvWoodNew*singleCell.AforestSharePrev)*singleCell.LandAreaHa;
		harvestTotM3 = harvestFmTotM3 + deforWoodTotM3;

		//double harvestTotM3 = (harvWood*OforestShare+harvWoodNew*singleCell.AforestSharePrev)*singleCell.LandAreaHa;
		harvestGrid.set(xi,yi,harvestTotM3);

		//if (Country==11 && thinningForest.get(xi,yi)>0){ cout<<"countryCalc = \t"<<Country<<"\t deforWoodTotM3 = \t"<<deforWoodTotM3<<"\tdeforest=\t"<<singleCell.deforestA[Age]<<"\toforest=\t"<<OforestShare<<endl;       }
		//if (Country==11){ cout<<"countryCalc = \t"<<Country<<"\t deforWoodDiff = \t"<<deforWoodTotM3-singleCell.deforWoodTotM3<<endl;       }
		//if (Country==47){ cout<<"countryCalc = \t"<<Country<<"\t deforWoodTotM3 = \t"<<deforWoodTotM3;
		//cout<<"\t harvest=\t"<<harvWood*OforestShare*singleCell.LandAreaHa<<"\tnewHarvest=\t"<<harvWoodNew*singleCell.AforestSharePrev*singleCell.LandAreaHa;
		//cout<<"\tnewHarvestGrid=\t"<<harvestGrid.get(xi,yi)<<"\tharvTot=\t"<<harvestTotM3;
		//cout<<"\t thinning=\t"<<thinningForest.get(xi,yi)<<"\trot=\t"<<rotationForest.get(xi,yi)<<endl;}

		harvestFcM3 = (sawnW + restW + sawnWnew + restWnew)* it.FTIMBER.v(); // current harvested wood from final cut in the cell, m3/ha
		harvestThM3 =(sawnThW + restThW + sawnThWnew + restThWnew) * it.FTIMBER.v(); // current harvested wood from thinning in the cell, m3/ha
		harvestTotPlusM3 = (harvWoodPlus*OforestShare + harvWoodNewPlus*singleCell.AforestSharePrev+(defBiomass-(1-resUse)*resDefor.hv) * (1. - it.SLASHBURN.v())*singleCell.deforestA[Age]* it.FTIMBER.v())*singleCell.LandAreaHa; //Total current harvested wood in the cell, m3, including residues (harvest losses)
		// CAI_m3ha = (abBiomassO-singleCell.ObiomassPrev + harvWoodLost + sawnW + restW + sawnThW + restThW + harvL) * it.FTIMBER"].v(); //Current annual increment m3/ha
		// CAI_m3ha = (abBiomassO-singleCell.ObiomassPrev + sawnW + restW + sawnThW + restThW + harvL) * it.FTIMBER.v(); //Current annual increment m3/ha
		// CAI_m3ha = (abBiomassO-singleCell.ObiomassPrev*(1-singleCell.deforestA[Age]) + sawnW + restW + sawnThW + restThW + harvL + harvWoodLost) * it.FTIMBER.v(); //Current annual increment m3/ha
		CAI_m3ha = ((abBiomassO-singleCell.ObiomassPrev) + sawnW + restW + sawnThW + restThW + harvL + harvWoodLost) * it.FTIMBER.v(); //Current annual increment m3/ha
		CAI_m3ha = CAI_m3ha < 0. ? 0. : CAI_m3ha;
		CAI_new_m3ha = (phytNewIncr + harvWoodLostNew + sawnWnew + sawnThWnew + restThWnew + restWnew + harvLNew) * it.FTIMBER.v();
		CAI_new_m3ha = CAI_new_m3ha < 0. ? 0. : CAI_new_m3ha;
		if (AforestShare > 0) {phytNewIncr = CurPlantPhytBmGr/(AforestShare * singleCell.LandAreaHa);}

		// should we account for deforested biomass when estimating CAI???
		// should we account for deforested biomass when estimating harvest?

		//double FMsink_ab = (abBiomassO-singleCell.ObiomassPrev*(1-singleCell.deforestA[Age]))*3.6666666667/1000;
		FMsink_ab = (abBiomassO-singleCell.ObiomassPrev)*3.6666666667/1000;
		FMsink_bm = FMsink_ab * (1+coeffBl);

		//if (Country==11 && year>2010){ cout<<"countryCalc = \t"<<Country<<"\t deforWoodTotM3 = \t"<<singleCell.deforWoodTotM3<<endl;       }

		if (thinningForest.get(xi,yi) < 0)
		{
			biomassRot = fi.gU(abBiomassO, maiForest.get(xi,yi), 1)+1;  
		}
		else
		{
			biomassRot = fi.gUt(abBiomassO, maiForest.get(xi,yi), thinningForest.get(xi,yi))+1;
		}

		//--------------------------------------------------------------------------------
		// Estimation of NPV vs rotation time
		//-------------------------------------------------------------------------------

		if (forNPVcurves)
		{
			if (year == refYear)
			{
				if (OforestShare>0)
				{
					if (abBiomassO>0)
					{
						if (maiForest.get(xi,yi)>0)
						{
							int rotMax = 0;
							if (thinningForest.get(xi,yi) > 0)
							{
								rotMax = fi.gTopt(maiForest.get(xi,yi), 3);  //  rotMaxBmTh 
							}
							else if (thinningForest.get(xi,yi) < 0)
							{
								rotMax = fi.gTopt(maiForest.get(xi,yi), 2); //  rotMaxBm
							}
							int rotMed = int((rotMax + rotMAI)/2);
							minRot[Country-1] += rotMAI * OforestShare * singleCell.LandAreaHa ;
							maxRot[Country-1] += rotMax * OforestShare * singleCell.LandAreaHa ; 
							//rotMAI
							//(rotMAI+rotMed)/2
							//rotMed
							//(rotMed+rotMax)/2
							//rotMax

							// NPV curves
							//minRotNPV[209];   // country average NPV if max MAI rotation is applied to all forests
							//minMedNPV[209];   // NPV at rotation between min and medium rotation
							//medRotNPV[209];   // NPV at rotation between min and medium rotation
							//medMaxNPV[209];     // NPV at rotation between medium and max biomass rotation
							//maxRotNPV[209];  // country average NPV if max biomass rotation is applied to all forests
							//minRot[209];    // country average min rotation (max harvest)
							//maxRot[209];    // country average max biomass rotation
							//---------------------

							//forNPV(g4m::dataStruct &plot, double forest, int year, int rotation, double maiV, 
							//	double priceW, double harvestedW, double wCpriceorr, double wCpriceorr0, double Cprice)
							{
								g4m::ageStruct cohortTmp = cohort;
								cohortTmp.setRotPeriod(rotMAI);
								g4m::ageStruct::v resTmp = cohortTmp.aging();
								//g4m::ageStruct::v resTmpNew = newCohort_all[asID]->aging();
								double sawW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
								double restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
								double sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
								double restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
								double harvestTmp = (sawnW + restW + sawnThW + restThW + resUse*resTmp.hv)  * it.FTIMBER.v();
								//	* OforestShare * singleCell.LandAreaHa * it.FTIMBER"].v(); // Total current harvested wood in the cell, m3
     
								minRotNPV[Country-1] += forNPV(it, OforestShare, year, rotMAI, maiForest.get(xi,yi)*it.FTIMBER.v(),
																TimberPrice, harvestTmp, cohortTmp.getArea(0), wprice[regprice], wprice[regprice0].v(2000), 0.)
																* OforestShare * singleCell.LandAreaHa;
							}
							{
								g4m::ageStruct cohortTmp = cohort;
								cohortTmp.setRotPeriod(int((rotMAI+rotMed)/2));

								g4m::ageStruct::v resTmp = cohortTmp.aging();
								//g4m::ageStruct::v resTmpNew = newCohort_all[asID]->aging();
								double sawW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
								double restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
								double sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
								double restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
								double harvestTmp = (sawnW + restW + sawnThW + restThW + resUse*resTmp.hv)  * it.FTIMBER.v();
								//	* OforestShare * singleCell.LandAreaHa * it.FTIMBER"].v(); // Total current harvested wood in the cell, m3

								minMedNPV[Country-1] += forNPV(it, OforestShare, year, int((rotMAI+rotMed)/2), maiForest.get(xi,yi)*it.FTIMBER.v(), 
																TimberPrice, harvestTmp, cohortTmp.getArea(0),wprice[regprice], wprice[regprice0].v(2000), 0.)
																* OforestShare * singleCell.LandAreaHa;
							}
							{
								g4m::ageStruct cohortTmp = cohort;
								cohortTmp.setRotPeriod(rotMed);

								g4m::ageStruct::v resTmp = cohortTmp.aging();
								//g4m::ageStruct::v resTmpNew = newCohort_all[asID]->aging();
								double sawW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
								double restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
								double sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
								double restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
								double harvestTmp = (sawnW + restW + sawnThW + restThW + resUse*resTmp.hv)  * it.FTIMBER.v();
								//	* OforestShare * singleCell.LandAreaHa * it.FTIMBER"].v(); // Total current harvested wood in the cell, m3

								medRotNPV[Country-1] += forNPV(it, OforestShare, year, rotMed, maiForest.get(xi,yi)*it.FTIMBER.v(), 
																TimberPrice, harvestTmp, cohortTmp.getArea(0), wprice[regprice], wprice[regprice0].v(2000), 0.)
																* OforestShare * singleCell.LandAreaHa;
							}
							{
								g4m::ageStruct cohortTmp = cohort;
								cohortTmp.setRotPeriod(int((rotMed+rotMax)/2));

								g4m::ageStruct::v resTmp = cohortTmp.aging();
								//g4m::ageStruct::v resTmpNew = newCohort_all[asID]->aging();
								double sawW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
								double restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
								double sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
								double restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
								double harvestTmp = (sawnW + restW + sawnThW + restThW + resUse*resTmp.hv)  * it.FTIMBER.v();
								//	* OforestShare * singleCell.LandAreaHa * it.FTIMBER"].v(); // Total current harvested wood in the cell, m3

								medMaxNPV[Country-1] += forNPV(it, OforestShare, year, int((rotMed+rotMax)/2), maiForest.get(xi,yi)*it.FTIMBER.v(), 
																TimberPrice, harvestTmp, cohortTmp.getArea(0), wprice[regprice], wprice[regprice0].v(2000), 0.)
																* OforestShare * singleCell.LandAreaHa;
							}
							{
								g4m::ageStruct cohortTmp = cohort;
								cohortTmp.setRotPeriod(rotMax);
						        
								g4m::ageStruct::v resTmp = cohortTmp.aging();
								//g4m::ageStruct::v resTmpNew = newCohort_all[asID]->aging();
								double sawW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
								double restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
								double sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
								double restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
								double harvestTmp = (sawnW + restW + sawnThW + restThW + resUse*resTmp.hv) * it.FTIMBER.v();
								//	* OforestShare * singleCell.LandAreaHa * it.FTIMBER"].v(); // Total current harvested wood in the cell, m3
						         
								maxRotNPV[Country-1] += forNPV(it, OforestShare, year, rotMax, maiForest.get(xi,yi)*it.FTIMBER.v(), 
																TimberPrice, harvestTmp, cohortTmp.getArea(0), wprice[regprice], wprice[regprice0].v(2000), 0.)
																* OforestShare * singleCell.LandAreaHa;
							}
							//cout<< "minRotNPV[Country-1] = "<<minRotNPV[Country-1] <<"    maxRotNPV[Country-1] "<<maxRotNPV[Country-1] <<endl;
						}
					}
				}
			}
		}
		//-------------------------------------------------------------------------------
		//*******************************************************************************

		//***********************************************
		//NPV for dynamic harvest and prices
		if (forNPVcuvvesDyn)
		{
			if (year == refYear)
			{
				if (OforestShare>0)
				{
					if (abBiomassO>0)
					{
						if (maiForest.get(xi,yi)>0)
						{
							int rotMax = 0;
							if (thinningForest.get(xi,yi) > 0)
							{
								rotMax = fi.gTopt(maiForest.get(xi,yi), 3);  //  rotMaxBmTh 
							}
							else if (thinningForest.get(xi,yi) < 0)
							{
								rotMax = fi.gTopt(maiForest.get(xi,yi), 2); //  rotMaxBm
							}

							int rotMed = int((rotMax + rotMAI)/2);
							minRot[Country-1] += rotMAI * OforestShare * singleCell.LandAreaHa ;
							maxRot[Country-1] += rotMax * OforestShare * singleCell.LandAreaHa ;                     

							double maiV = maiForest.get(xi,yi)*it.FTIMBER.v();

							{ 
								int rotation = rotMAI;                       
								g4m::ageStruct cohortTmp = cohort;
								cohortTmp.setRotPeriod(rotation);
								g4m::ageStruct::v resTmp;

								double currF = 0.;
								double npvSum = 0.;
								int j=0;
								do
								{
									resTmp = cohortTmp.aging();
									double sawW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
									double restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
									double sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
									double restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
									double harvestTmp = (sawnW + restW + sawnThW + restThW + resUse*resTmp.hv)  * it.FTIMBER.v();
									double harvestedArea = cohortTmp.getArea(0);

									double NPVone = forNPVfd(it, OforestShare, year+j, rotation, maiV, wprice[regprice], wprice[regprice0].v(2000), harvestTmp, harvestedArea);
									currF = NPVone/(pow(1+it.R[2000],j));
									npvSum += currF;
									j++;
									//cout<<"j= "<<j<<"   currF= "<<currF<<"   npvSum= "<<npvSum<<endl;
								}
								while (currF > 0.00001 && j < 100);

								//cout<< "forestValueOneExt = "<<forestValueOneExt<<"  npvSum = "<<npvSum<<"  j = "<<j<<"  harvestedArea = "<<harvestedArea<<endl; 
								minRotNPV[Country-1] += npvSum * OforestShare * singleCell.LandAreaHa;
							}
							//-------------------------------
							{
								int rotation = int((rotMAI+rotMed)/2);
								g4m::ageStruct cohortTmp = cohort;
								cohortTmp.setRotPeriod(rotation);
								g4m::ageStruct::v resTmp;

								double currF = 0.;
								double npvSum = 0.;
								int j=0;
								do
								{
									resTmp = cohortTmp.aging();
									double sawW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
									double restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
									double sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
									double restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
									double harvestTmp = (sawnW + restW + sawnThW + restThW + resUse*resTmp.hv)  * it.FTIMBER.v();
									double harvestedArea = cohortTmp.getArea(0);

									double NPVone = forNPVfd(it, OforestShare, year+j, rotation, maiV, wprice[regprice], wprice[regprice0].v(2000), harvestTmp, harvestedArea);
									currF = NPVone/(pow(1+it.R[2000],j));
									npvSum += currF;
									j++;
									//cout<<"j= "<<j<<"   currF= "<<currF<<"   npvSum= "<<npvSum<<endl;
								}
								while (currF > 0.00001 && j < 100);
								minMedNPV[Country-1] +=npvSum * OforestShare * singleCell.LandAreaHa;
							}
							//-------------------------------
							{
								int rotation = rotMed;
								g4m::ageStruct cohortTmp = cohort;
								cohortTmp.setRotPeriod(rotation);
								g4m::ageStruct::v resTmp;

								double currF = 0.;
								double npvSum = 0.;
								int j=0;
								do
								{
									resTmp = cohortTmp.aging();
									double sawW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
									double restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
									double sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
									double restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
									double harvestTmp = (sawnW + restW + sawnThW + restThW + resUse*resTmp.hv)  * it.FTIMBER.v();
									double harvestedArea = cohortTmp.getArea(0);

									double NPVone = forNPVfd(it, OforestShare, year+j, rotation, maiV, wprice[regprice], wprice[regprice0].v(2000), harvestTmp, harvestedArea);
									currF = NPVone/(pow(1+it.R[2000],j));
									npvSum += currF;
									j++;
									//cout<<"j= "<<j<<"   currF= "<<currF<<"   npvSum= "<<npvSum<<endl;
								}
								while (currF > 0.00001 && j < 100);
								medRotNPV[Country-1] += npvSum * OforestShare * singleCell.LandAreaHa;
							}
							//-------------------------------
							//-------------------------------
							{
								int rotation = int((rotMed+rotMax)/2);
								g4m::ageStruct cohortTmp = cohort;
								cohortTmp.setRotPeriod(rotation);
								g4m::ageStruct::v resTmp;

								double currF = 0.;
								double npvSum = 0.;
								int j=0;
								do
								{
									resTmp = cohortTmp.aging();
									double sawW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
									double restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
									double sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
									double restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
									double harvestTmp = (sawnW + restW + sawnThW + restThW + resUse*resTmp.hv)  * it.FTIMBER.v();
									double harvestedArea = cohortTmp.getArea(0);

									double NPVone = forNPVfd(it, OforestShare, year+j, rotation, maiV, wprice[regprice], wprice[regprice0].v(2000), harvestTmp, harvestedArea);
									currF = NPVone/(pow(1+it.R[2000],j));
									npvSum += currF;
									j++;
									//cout<<"j= "<<j<<"   currF= "<<currF<<"   npvSum= "<<npvSum<<endl;
								}
								while (currF > 0.00001 && j < 100);
								medMaxNPV[Country-1] +=npvSum * OforestShare * singleCell.LandAreaHa;
							}
							//-------------------------------
							{
								int rotation = rotMax;
								g4m::ageStruct cohortTmp = cohort;
								cohortTmp.setRotPeriod(rotation);
								g4m::ageStruct::v resTmp;

								double currF = 0.;
								double npvSum = 0.;
								int j=0;
								do
								{
									resTmp = cohortTmp.aging();
									double sawW = resTmp.enSw;      // MG: get harvestable sawnwood for the set (old) forest tC/ha for final cut. 
									double restW = resTmp.enRw;      // MG: get harvestable restwood for the set (old) forest tC/ha for final cut. 
									double sawnThW = resTmp.vnSw;    // MG: get harvestable sawnwood for the set (old) forest tC/ha for thinning. 
									double restThW = resTmp.vnRw;    // MG: get harvestable restwood for the set (old) forest tC/ha for thinning.            
									double harvestTmp = (sawnW + restW + sawnThW + restThW + resUse*resTmp.hv)  * it.FTIMBER.v();
									double harvestedArea = cohortTmp.getArea(0);

									double NPVone = forNPVfd(it, OforestShare, year+j, rotation, maiV, wprice[regprice], wprice[regprice0].v(2000), harvestTmp, harvestedArea);
									currF = NPVone/(pow(1+it.R[2000],j));
									npvSum += currF;
									j++;
									//cout<<"j= "<<j<<"   currF= "<<currF<<"   npvSum= "<<npvSum<<endl;
								}
								while (currF > 0.00001 && j < 100);
								maxRotNPV[Country-1] += npvSum * OforestShare * singleCell.LandAreaHa;
							}
							//-------------------------------
						}
					}
				}
			}
		}
		plantingCosts = decision.plantingCosts();

		/*
		// MG: Writing to file at specified time points

		//	    if ((year==2000) || (year==2001)|| (year==2002)|| (year==2003)|| (year==2004) || (year==2005) || (year==2006)
		//		if ((year==2020) || (year==2030) || (year==2050)) {
		//	    if ((year==2000) || (year==2005) 
		//                         || (year==2010) || (year==2015) || (year==2020) || (year==2030) || (year==2050)
		//                         || (year==2100)
		//         )
			{
			  ireportYear++;
			  double deltaReportYear = (year-prevReportYear);
			  if (deltaReportYear == 0.) {deltaReportYear = 1.;};
			  double DefCurHa = (prevOForShareRP - OforestShare) * LandAreaHa / deltaReportYear;
			  if (DefCurHa < 0) {DefCurHa = 0;}
			  double AffCurHa = (AforestShare - prevAForShareRP) * LandAreaHa / deltaReportYear;
			  if (AffCurHa < 0) {AffCurHa = 0;}
			  double EmissionsDeforCur = (EmissionsTot - EmissionsTotPrev)/deltaReportYear;
			  double EmissionsAfforCur1 = (EmissionsAffor - EmissionsAfforPrev)/deltaReportYear;

		// Country Arrays
		//   CountriesNforCover 
		//   CountriesNforBiomass 
		//   CountriesAfforHaYear   
		//   CountriesAfforCYear  
		//   CountriesOforCover
		//   CountriesOforBiomass
		//   CountriesDeforHaYear 
		//   CountriesDeforCYear  

			  CountriesNforCover.inc(Country,year,AforestShare * LandAreaHa);
			  CountriesAfforHaYear.inc(Country,year,AffCurHa);          
			  prevReportYear = year;
			  prevOForShareRP = OforestShare;
			  prevAForShareRP = AforestShare;
			  EmissionsTotPrev = EmissionsTot;
			  EmissionsAfforPrev = EmissionsAffor;
				  
		//		  prevOForShareRPNP = OfsNoPay;
		//		  prevAForShareRPNP = AfsNoPay;		  
		//		  prevPlantPhytHaBmGr = PlantPhytHaBmGr;

		//cohortNew.aging(); // MG: we grow planted forest

			} // MG: End of output years loop

			AforestSharePrev = AforestShare;
			prevOForShare = OforestShare;
		    
			prevPlantPhytHaBmGr = PlantPhytHaBmGr;
			prevPlantPhytHaBlGr = PlantPhytHaBlGr;
		    
			cropland = crop;
			builtupland = builtup;


		//        cout<<Country+1<<"\t"<<Age+1<<"\t"<<iprice<<"\t"<<EmissionsCurAfforCountry[Country]<<endl;
		//      arrdef.set(Country,Age+1,iprice+1,EmissionsCurCountry[Country]);
		//      arraff.set(Country,Age+1,iprice+1,EmissionsCurAfforCountry[Country]);
		      
		//      arrdefNP.set(Country,Age+1,iprice+1,EmissionsCurCountryNP[Country]);
		//      arraffNP.set(Country,Age+1,iprice+1,EmissionsCurAfforCountryNP[Country]);
		      


		//cout<< "vIncr= " <<decision.vIncr()<< "    gTopt= "<< rotInterM<< "   rotInter()="<<decision.rotInter()<<endl;
		//cout<<"harvWood= "<< harvWood<<"     HarvestVol()= "<<decision.woodHarvestVol()<<"     HarvWood_gHbm="<<fi.gHbm(rotInterM,decision.vIncr())<<endl;  	  
		//cout<<"    NPV= "<<decision.forVal() <<endl;

		//cout << "forFlag = " << forFlag << endl;


		if (forFlag > 0)
		{

		cout <<xi<<"\t"<<yi;
		cout <<"\t"<<Country;
		cout <<"\t"<<year;
		cout <<"\t" << rotationTimeCurr << "\t"<< Rotation <<"\t" << MAIRot  <<"\t";

		//cout <<sawnThW*OforestShare*LandAreaHa<<"\t"<<restThW*OforestShare*LandAreaHa<<"\t";
		//cout <<sawnW*OforestShare*LandAreaHa<<"\t"<<restW*OforestShare*LandAreaHa<<"\t"<<abBiomassO*OforestShare*LandAreaHa<<"\t";
		//cout <<it.CABOVEHA"].v()*OforestShare*LandAreaHa<<"\t";

		cout <<sawnThW*OforestShare*LandAreaHa*4;
		cout <<"\t"<<restThW*OforestShare*LandAreaHa*4<<"\t";
		cout <<sawnW*OforestShare*LandAreaHa*4;
		cout <<"\t"<<restW*OforestShare*LandAreaHa*4<<"\t";

		cout << sawnThWpot*OforestShare*LandAreaHa*4<<"\t" << restThWpot*OforestShare*LandAreaHa*4 << "\t";
		cout << sawnWpot*OforestShare*LandAreaHa*4 <<"\t" << restWpot*OforestShare*LandAreaHa*4;


		cout << "\t" << abBiomassO*OforestShare*LandAreaHa <<"\t";
		cout << it.CABOVEHA"].v()*refForShare*LandAreaHa <<"\t";

		cout << maiForest.get(xi,yi) << "\t" <<fval << "\t" << forNPV_CH;
		cout << "\t" << TimberPrice << "\t";


		cout <<sawnThW<<"\t"<<restThW<<"\t";
		cout <<sawnW<<"\t"<<restW<<"\t"<<abBiomassO<<"\t";

		cout <<it.CABOVEHA"].v()<<"\t";

		cout << refForShare*LandAreaHa<<"\t";

		cout<< int(managedForest.get(xi,yi))<<"\t";

		cout<< int(thinningForest.get(xi,yi))<<"\t";

		cout<< EmissionsTot<<"\t";
		cout<<EmissionsAffor<<"\t";

		cout<<harvWoodLost*OforestShare*LandAreaHa;
		cout << "\t" << OforestShare * LandAreaHa;  // Remained old forest, ha
		cout << "\t" << AforestShare * LandAreaHa; // Planted forest, ha (accumulated)

		cout << "\t" << PlantPhytHaBmGr*LandAreaHa;
		cout << "\t" << EmLitterAffor;
		cout << "\t" << EmSOCAffor;
		cout << "\t" << harvWoodNew;
		cout << "\t" << harvWoodLostNew;
		cout<< endl;
		}
		*/
		//**** Estimation of n-year average of wood obtained from deforestation ****
		//short int N = 1; // number of averaging years (N<=5)
		//for (int i=1;i<N;i++) {singleCell.deforWoodArray[i-1] = singleCell.deforWoodArray[i];
		//                      singleCell.deforAreaArray[i-1] = singleCell.deforAreaArray[i];}
		//singleCell.deforWoodArray[N-1] = deforWoodTotM3;
		//if (year >= 2000)
		//{singleCell.deforAreaArray[N-1] = singleCell.deforestA[Age];
		//}else{singleCell.deforAreaArray[N-1] = 0.;}
		//
		//double deforWoodArray_sum=0;
		//double deforAreaArray_sum=0;
		//for (int i=0;i<N;i++) {deforWoodArray_sum+=singleCell.deforWoodArray[i];
		//                       deforAreaArray_sum+=singleCell.deforAreaArray[i];
		////if(Country==11){cout<<"deforWoodArray["<<i<<"]=\t"<<singleCell.deforWoodArray[i]<<endl;}
		//}
		////if(Country==11){cout<<"deforWoodArray_sum=\t"<<deforWoodArray_sum<<"\t Age=\t"<<Age<<endl;}
		//double deforWoodTotM3_ave = 0;
		//double deforestShare_ave = 0;
		//if (Age<N){deforWoodTotM3_ave = deforWoodArray_sum/(Age+1);
		//           deforestShare_ave = deforAreaArray_sum/(Age+1);
		//}else {deforWoodTotM3_ave = deforWoodArray_sum/N;
		//      deforestShare_ave = deforAreaArray_sum/N;}
		//if(Country==11){cout<<"country=\t"<<Country<<"\t year= \t"<<year<<"\t Age=\t"<<Age<<"\t deforWoodTotM3= \t"<<deforWoodTotM3<<"\t deforWoodTotM3_ave= \t"<<deforWoodTotM3_ave<<"\t deforestShare= \t"<<singleCell.deforestA[Age]<<"\t deforestShare_ave= \t"<<deforestShare_ave<<endl;}
		//----------------------------------------------------------------------------
	} // end for IF (year>byear)

	if (fff.is_open())
	{
		if (countriesList.find(Country) != countriesList.end())
		{
			////cout<<"OforShare= "<< OforestShare<< "\tAforestShare= "<<AforestShare<<"\tcab= "<<it.CABOVEHA"].v()<<"\tNPP= "<<it.NPP"].v(2000)<<"\trotMAI= "<<rotMAI<<"\tharvMAI= "<<harvMAI<<"\tforNPV_CH= "<<forNPV_CH<<"\tfVal= "<<fval<<"\t aval= "<<aval<<"\t rotationTimeCurr= "<<rotationTimeCurr<<"\t rotationTimeCurrNew= "<<rotationForestNew.get(xi,yi)<< endl;
			fff<<asID<<"\t"<<year<<"\t"<<Country<<"\t"<<OforestShare<<"\t"<<AforestShare<<"\t"<<it.CABOVEHA.v()<<"\t"<<abBiomassO<<"\t"<<maiForest.get(xi,yi)*it.FTIMBER.v()<<"\t"<< rotMAI;
			fff<<"\t"<<harvestThM3<<"\t"<<harvestFcM3<<"\t"<<harvestTotM3<<"\t"<<harvMAI<<"\t"<<singleCell.rotBiomass<<"\t"<<forNPV_CH<<"\t"<<fval<<"\t"<<aval<<"\t"<<rotationTimeCurr<<"\t"<<rotationForestNew.get(xi,yi)<<"\t"<<TimberPrice;
			fff<<"\t"<<(thinningForest.get(xi,yi))<<"\t"<<int(managedForest.get(xi,yi));
			fff<<"\t"<<EmissionsSOCAfforCur/singleCell.afforestHaTot<<"\t"<<PlantPhytHaBmGr<<"\t"<<EmissionsLitterAfforCur/(singleCell.forestAgeShare[Age]*singleCell.LandAreaHa);
			fff<<"\t"<<singleCell.LitterAffor[Age]/(singleCell.forestAgeShare[Age]*singleCell.LandAreaHa)<< endl;

			//fff<<asID<<"\t"<<year<<"\t"<<Country<<"\t"<<priceC<<"\t"<<deforestHa<<"\t"<<afforestHa<<"\t"<<OforestShare*singleCell.LandAreaHa<<"\t"<<AforestShare*singleCell.LandAreaHa;
			//fff<<"\t"<<EmissionsCur*3.6666666667/1000<<"\t"<<defBiomass*deforestHa*3.6666666667/1000<<"\t"<<it.CBELOWHA.v()*deforestHa*3.6666666667/1000;
			//fff<<"\t"<<(defBiomass+it.CBELOWHA.v())*deforestHa*3.6666666667/1000<<"\t"<<(EmissionsDeadBurnCur+EmissionsLitterCur)*3.6666666667/1000;
			//fff<<"\t"<<EmissionsSOCCur*3.6666666667/1000;
			//fff<<"\t"<<EmissionsAfforCur*3.6666666667/1000<<"\t"<<CurPlantPhytHaBmGr*3.6666666667/1000<<"\t"<<CurPlantPhytHaBlGr*3.6666666667/1000;
			//fff<<"\t"<<(CurPlantPhytHaBmGr+CurPlantPhytHaBlGr)*3.6666666667/1000<<"\t"<<EmissionsLitterAfforCur*3.6666666667/1000<<"\t"<<EmissionsSOCAfforCur*3.6666666667/1000;
			//fff<<endl;
		}
		//fff << "asID"<<"\tyear"<<"\tCountry"<< "\tpriceCO2"<<"\tdeforHaYear"<<"\tafforHaYear"<<"\tOforestHa"<< "\tAforestHa";
		//fff << "\tdeforCYear"<<"\tdeforCYear_ab"<<"\tdeforCYear_bl"<<"\tdeforCYear_biom"<<"\tdeforCYear_dom"<<"\tdeforCYear_soil";
		//fff << "\tafforCYear"<<"\tafforCYear_ab"<<"\tafforCYear_bl"<<"\tafforCYear_biom"<<"\tafforCYear_dom"<<"\tafforCYear_soil";
		//fff << endl;
	}

	// Output results for countries

	CountriesNforCover.inc(Country,year,AforestShare * singleCell.LandAreaHa);
	CountriesAfforHaYear.inc(Country,year,afforestHa);          
	double AforBm = (PlantPhytHaBmGr + PlantPhytHaBlGr) * singleCell.LandAreaHa ; // accumulated tot living biomass of planted forest, tC
	CountriesNforTotC.inc(Country,year,AforBm);
	CountriesAfforCYear.inc(Country,year,EmissionsAfforCur*3.6666666667/1000);  
	CountriesAfforCYear_ab.inc(Country,year, CurPlantPhytBmGr*3.6666666667/1000);  
	CountriesAfforCYear_bl.inc(Country,year, CurPlantPhytBlGr*3.6666666667/1000);   
	double EmAffBm = (CurPlantPhytBmGr+CurPlantPhytBlGr)*3.6666666667/1000; // CO2 sink in forest living biomass of planted forest, mtCO2/year
	CountriesAfforCYear_biom.inc(Country,year, EmAffBm);
	CountriesAfforCYear_dom.inc(Country,year, EmissionsLitterAfforCur*3.6666666667/1000);    
	CountriesAfforCYear_soil.inc(Country,year, EmissionsSOCAfforCur*3.6666666667/1000);     
	//--------- 
	CountriesOforCover.inc(Country,year,OforestShare * singleCell.LandAreaHa);

	CountriesDeforHaYear.inc(Country,year,deforestHa);  

	CountriesOfor_abC.inc(Country,year,abBiomassO*OforestShare*singleCell.LandAreaHa);
	double OforBm = (abBiomassO+it.CBELOWHA.v()); // tot living biomass of old forest, tC/ha
	CountriesOforC_biom.inc(Country,year, OforBm*OforestShare*singleCell.LandAreaHa);   
	CountriesDeforCYear.inc(Country,year,EmissionsCur*3.6666666667/1000);    
	CountriesDeforCYear_ab.inc(Country,year, defBiomass*deforestHa*3.6666666667/1000);
	CountriesDeforCYear_bl.inc(Country,year,it.CBELOWHA.v()*deforestHa*3.6666666667/1000);      
	double EmDefBm = (defBiomass+it.CBELOWHA.v())*3.6666666667/1000; // CO2 lost from living biomass of old forests due to deforestation, mtCO2/hayear
	CountriesDeforCYear_biom.inc(Country,year,EmDefBm*deforestHa);       
	CountriesDeforCYear_dom.inc(Country,year, (EmissionsDeadBurnCur+EmissionsLitterCur)*3.6666666667/1000);     
	CountriesDeforCYear_soil.inc(Country,year, EmissionsSOCCur*3.6666666667/1000);     
	//--------- 

	CountriesWoodHarvestM3Year.inc(Country,year,harvestTotM3);
	CountriesWoodHarvestPlusM3Year.inc(Country,year,harvestTotPlusM3);    
	CountriesWoodHarvestFmM3Year.inc(Country,year,harvestFmTotM3);
	CountriesWoodHarvestDfM3Year.inc(Country,year,deforWoodTotM3);     

	CountriesWoodLoosCYear.inc(Country,year,(harvWoodLost*OforestShare+harvWoodLostNew*singleCell.AforestSharePrev)*singleCell.LandAreaHa*3.6666666667/1000);
	//     if (harvestTotM3>0) 
	CountriesHarvLossesYear.inc(Country,year,(harvL*OforestShare+harvLNew*singleCell.AforestSharePrev)* it.FTIMBER.v()*singleCell.LandAreaHa);
	//---------
	if (thinningForest.get(xi,yi)>0 & thinningForestNew.get(xi,yi)>0)
	{
		CountriesManagedCount.inc(Country,year,1);
		CountriesManagedForHa.inc(Country,year, (AforestShare+OforestShare) * singleCell.LandAreaHa);
	}
	else if (thinningForest.get(xi,yi)>0 & thinningForestNew.get(xi,yi)<0)
	{
		CountriesManagedCount.inc(Country,year,1);
		CountriesManagedForHa.inc(Country,year,OforestShare * singleCell.LandAreaHa);
	}

	CountriesMAI.inc(Country,year,maiForest.get(xi,yi)*it.FTIMBER.v());
	CountriesCAI.inc(Country,year,CAI_m3ha * OforestShare * singleCell.LandAreaHa);

	CountriesCAI_new.inc(Country,year,CAI_new_m3ha * singleCell.AforestSharePrev*singleCell.LandAreaHa);     
	//CountriesCAI_new.inc(Country,year,(phytNewIncr + sawnWnew + sawnThWnew + restThWnew + restWnew + harvLNew) * it.FTIMBER.v()*singleCell.AforestSharePrev*singleCell.LandAreaHa);     
	CountriesFM.inc(Country,year,FMsink_ab*OforestShare*singleCell.LandAreaHa);
	CountriesFMbm.inc(Country,year,FMsink_bm*OforestShare*singleCell.LandAreaHa);

	double profit = harvestTotM3 * TimberPrice - plantingCosts * cohort.getArea(0) * OforestShare * singleCell.LandAreaHa;
	CountriesProfit.inc(Country, year, profit);

	CountryRotation.inc(Country,year,rotationTimeCurr);

	//cout<<Country<<endl;
	short int countryreg = it.COUNTRYREGMIX[2000];     
	CountryregWoodHarvestM3Year.inc(countryreg,year,harvestTotM3);
	CountryregWoodHarvestFmM3Year.inc(countryreg,year,harvestFmTotM3);
	CountryregWoodHarvestDfM3Year.inc(countryreg,year,deforWoodTotM3);     

	char countryregch[2];
	int2str(countryreg,countryregch);
	string countryregstr = "re"+string(countryregch)+"price0";
	countryreg = (int)it.COUNTRYREGMIX[2000];
	//if (countryreg==1 ||countryreg==17){
	//cout<<"countryreg=\t"<<countryreg<<"\t"<<countryregstr<<"\t"<<wprod[countryregstr].v(year)<<endl;
	//}
	CountryregWprod.set(countryreg,year,wprod[countryregstr].v(year));
	CountryregRotation.inc(countryreg,year,rotationTimeCurr);
	//---------------------------------------------------------------------------------    
	//-- ASCII grid output--------------------------
	if (gridOutput)
	{
		if (year == 1990)
		{
			SD_grid_1990.set(xi,yi,thinningForest.get(xi,yi));
		}
		if (year == 2000)
		{
			SD_grid_00.set(xi,yi,thinningForest.get(xi,yi));
		}
		if (year == 2010)
		{
			SD_grid_10.set(xi,yi,thinningForest.get(xi,yi));
		}
		if (year == 2020)
		{
			harvestm3_grid_20.set(xi,yi,harvestTotM3);
			bmabtC_of_grid_20.set(xi,yi,abBiomassO*OforestShare*singleCell.LandAreaHa);
			bmabtC_nf_grid_20.set(xi,yi,PlantPhytHaBmGr*singleCell.AforestSharePrev*singleCell.LandAreaHa);
			fmgGco2_grid_20.set(xi,yi,FMsink_ab*OforestShare*singleCell.LandAreaHa);     
			mai_m3ha_grid_20.set(xi,yi,maiForest.get(xi,yi)*it.FTIMBER.v());
			SD_grid_20.set(xi,yi,thinningForest.get(xi,yi));
			RL_grid_20.set(xi,yi,rotationTimeCurr);
		}
		if (year == 2030)
		{
			harvestm3_grid_30.set(xi,yi,harvestTotM3);
			bmabtC_of_grid_30.set(xi,yi,abBiomassO*OforestShare*singleCell.LandAreaHa);
			bmabtC_nf_grid_30.set(xi,yi,PlantPhytHaBmGr*singleCell.AforestSharePrev*singleCell.LandAreaHa);
			fmgGco2_grid_30.set(xi,yi,FMsink_ab*OforestShare*singleCell.LandAreaHa);     
			mai_m3ha_grid_30.set(xi,yi,maiForest.get(xi,yi)*it.FTIMBER.v());
			SD_grid_30.set(xi,yi,thinningForest.get(xi,yi));
			RL_grid_30.set(xi,yi,rotationTimeCurr);
		}
		if (year == 2050)
		{
			harvestm3_grid_50.set(xi,yi,harvestTotM3);
			bmabtC_of_grid_50.set(xi,yi,abBiomassO*OforestShare*singleCell.LandAreaHa);
			bmabtC_nf_grid_50.set(xi,yi,PlantPhytHaBmGr*singleCell.AforestSharePrev*singleCell.LandAreaHa);
			fmgGco2_grid_50.set(xi,yi,FMsink_ab*OforestShare*singleCell.LandAreaHa);     
			mai_m3ha_grid_50.set(xi,yi,maiForest.get(xi,yi)*it.FTIMBER.v());
			SD_grid_50.set(xi,yi,thinningForest.get(xi,yi));
			RL_grid_50.set(xi,yi,rotationTimeCurr);
		}
	}
	//-----------------------------------------------------------------------------
	OforestShGrid.set(xi,yi,OforestShare);

	singleCell.prevOForShare = OforestShare;
	singleCell.prevPlantPhytHaBmGr = PlantPhytHaBmGr;
	singleCell.prevPlantPhytHaBlGr = PlantPhytHaBlGr;
	singleCell.rotBiomass = biomassRot;
	//singleCell.usedForest = ;
	singleCell.SD = thinningForest.get(xi,yi);
	//singleCell.FMsink = (abBiomassO-singleCell.ObiomassPrev)*OforestShare*singleCell.LandAreaHa*3.6666666667/1000; // FM sink in GgCO2/yr
	singleCell.FMsink = FMsink_ab; // FM sink in GgCO2/yr/ha
	singleCell.FMsink_Bm = FMsink_bm;
	singleCell.ObiomassPrev = abBiomassO;
	//singleCell.deforWoodTotM3 = deforWoodTotM3_ave;// stem wood obtained from deforestation, 5-year average
	singleCell.deforWoodTotM3 = deforWoodTotM3;
	singleCell.CAI = CAI_m3ha;
	//singleCell.deforWoodTotM3 = 0.;
	//singleCell.deforestShare = deforestShare_ave; 
	if (year >= 2000) {singleCell.deforestShare = singleCell.deforestA[Age];}else{singleCell.deforestShare = 0.;}

	singleCell.afforestHaYear = afforestHa;
	singleCell.deforestHaYear = deforestHa;
	singleCell.harvestTot = (harvestFcM3 + harvestThM3) + harvestDfM3Ha; 
	singleCell.harvestFcM3Ha = harvestFcM3;
	singleCell.harvestThM3Ha = harvestThM3;
	singleCell.oforestBm = OforBm;  

	singleCell.emissionsD_Bm = EmDefBm;  

	if (OforestShare>0){singleCell.emissionsD_S = EmissionsSOCCur/(OforestShare*singleCell.LandAreaHa)*3.6666666667/1000;}else{singleCell.emissionsD_S = 0;}

	if (singleCell.AforestSharePrev * singleCell.LandAreaHa>0)
	{
		singleCell.aforestBm = AforBm/(singleCell.AforestSharePrev * singleCell.LandAreaHa);
		singleCell.emissionsA_Bm = EmAffBm/(singleCell.AforestSharePrev * singleCell.LandAreaHa); 
		singleCell.emissionsA_S = EmissionsSOCAfforCur/(AforestShare * singleCell.LandAreaHa)*3.6666666667/1000;
	}else{
		singleCell.aforestBm = 0;
		singleCell.emissionsA_Bm = 0;  
	}

	//if (singleCell.aforestBm > 200){
	//if (((((it.x)*GridStepLon+GridStepLon/2-180) ==-7.75) && (((it.y)*GridStepLat+GridStepLat/2-90)==10.25))
	//                   || ((((it.x)*GridStepLon+GridStepLon/2-180) ==-55.75) && (((it.y)*GridStepLat+GridStepLat/2-90)==4.75))){
	//cout<<"x=\t"<<(it.x)*GridStepLon+GridStepLon/2-180<<"\ty=\t"<<(it.y)*GridStepLat+GridStepLat/2-90<<"\tyear=\t"<<year;
	//cout<<"\t AforShare=\t"<<AforestShare;
	//cout<<"\tlandArea=\t"<< singleCell.LandAreaHa<<"\t AforBm=\t"<<AforBm<<"\t AforBmHa=\t"<<singleCell.aforestBm<<"\t EmAffBm=\t"<<EmAffBm<<"\t AforSharePrev=\t"<<singleCell.AforestSharePrev<<endl;                      
	//cout<<"------ end ------"<<endl;
	//}

	if (AforestShare * singleCell.LandAreaHa>0)
	{
		singleCell.emissionsA_S = EmissionsSOCAfforCur/(AforestShare * singleCell.LandAreaHa)*3.6666666667/1000;
	}
	else
	{
		singleCell.emissionsA_S = 0;
	}
	
	singleCell.AforestSharePrev = AforestShare;
}