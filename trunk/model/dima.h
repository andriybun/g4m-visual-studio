#ifndef DIMA_H_
#define DIMA_H_

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>

#include "ipol.h"

using namespace std;

//////////
// DIMA //
//////////
class dima
{
public:
	double forVal();      //Value of Forestry during multiple rotation
	double forValNC();      //Value of Forestry during multiple rotation
	double agrVal();      //Net present Value of Agriculture
	double amenVal();      //Net present Value of Agriculture
	double minPriceC();      //Get the minimum Carbonprice where Forest=Argic
	int setYear(int);
	double setForest(double);
	double forValExt();   //MG: Value of Forestry using External Timber price
	double forValNCExt();      //MG: Value of Forestry during multiple rotation with 0 C price and using External wood price
	double agrVal2000();   //MG: Net present Value of Agriculture in the year 2000
	double plantingCosts();    //Costs to plant x ha of forest  
	dima(int ayear
		, g4m::ipol<double,double> &anpp          //npp [kg-C/m2/year]
		, g4m::ipol<double,double> &asPopDens   //Standardised (1-10) population density
		, g4m::ipol<double,double> &asAgrSuit   //Standardised (1-10) agricultural suitability
		, g4m::ipol<double,double> &apriceIndex  //Priceindex
		, g4m::ipol<double,double> &apriceIndex0 //Priceindex ref Country
		, g4m::ipol<double,double> &ar          //discount rate
		, g4m::ipol<double,double> &apriceC      //carbon price [$/tC]
		, g4m::ipol<double,double> &aplantingCosts0 //Planting costs ref Country [$/ha]
		, g4m::ipol<double,double> &apriceLandMin0   //Minimum Landprice in ref country [$/ha]
		, g4m::ipol<double,double> &apriceLandMax0  //Maximum Landprice in ref country [$/ha]
		, g4m::ipol<double,double> &amaxRotInter   //Maximal rotation intervall [years]
		, g4m::ipol<double,double> &aminRotInter       //Minimal rotation intervall [years]
		, g4m::ipol<double,double> &adecLongProd  //Decay rate for long lived prod
		, g4m::ipol<double,double> &adecShortProd  //Decay rate for short lived pro
		//Fraction of carbon stored in longterm products
		, g4m::ipol<double,double> &afracLongProd
		//Fraction of carbon substracted due to baseline considerations
		, g4m::ipol<double,double> &abaseline
		//Comercial timbervolume per ton of carbon [m3/tC]
		//       , g4m::ipol<double,double> aftimber
		, g4m::ipol<double,double> &aftimber
		//         , int aftimber
		//Maximal timber price in reference country [$/m3]
		, g4m::ipol<double,double> &apriceTimberMax0
		//Minimal timber price in reference country {$/m3]
		, g4m::ipol<double,double> &apriceTimberMin0
		, g4m::ipol<double,double> &afcuptake  //Factor of carbon uptake from npp
		, g4m::ipol<double,double> &agdp       //Gross domestic production
		, g4m::ipol<double,double> &aharvLoos  //Harvesting losses
		, double aforest      //Share of landarea used by forest [0-1]
		, g4m::ipol<double,double> awpricecorr //MG: Added for wood price correction
		, double awpricecorr0 //MG: Added for wood price correction in year 2000 when price of carbon=0
		, double arotInterM  //MG: Added rotation interval estimated from Georg's Forest Management Tool
		, double aharvWood   //MG: Added harvestable wood from Georg's Forest Management Tool
		) : year(ayear)
		, npp(anpp)
		, sPopDens(asPopDens)
		, sAgrSuit(asAgrSuit)
		, priceIndex(apriceIndex)
		, priceIndex0(apriceIndex0)
		, r(ar)
		, priceC(apriceC)
		, plantingCosts0(aplantingCosts0)
		, priceLandMin0(apriceLandMin0)
		, priceLandMax0(apriceLandMax0)
		, maxRotInter(amaxRotInter)
		, minRotInter(aminRotInter)
		, decLongProd(adecLongProd)
		, decShortProd(adecShortProd)
		, fracLongProd(afracLongProd)
		, baseline(abaseline)
		, ftimber(aftimber)
		, priceTimberMax0(apriceTimberMax0)
		, priceTimberMin0(apriceTimberMin0)
		, fcuptake(afcuptake)
		, gdp(agdp)
		, harvLoos(aharvLoos)
		, forest(aforest)
		, wpricecorr(awpricecorr)
		, wpricecorr0(awpricecorr0)
		, rotInterM(arotInterM)
		, harvWood(aharvWood)
	{
		;
	}
	double priceTimber();      //Timber price
	double priceTimberExt();  //MG: Timber price External
	//private:
	int year;
	double rotInter();         //Rotation interval of a Forest in Years
	double woodHarvestVol();   //Harvest volume of the timber
	double cUptake();          //mean anual carbon uptake
	double vIncr();            //Harvestable wood-volum increment (m3/ha/year)
private:
	double beta();             //Fraction of carbon costs during harvest
	double cBenefit();         //Carbon benefit
	double priceHarvest();     //Price to harvest the timber
	double forestValueOne();   //Value of Forestry during one rotation
	double forestValueOneExt();//MG: Value of Forestry during one rotation  with External Timber price
	double forestValueOneNC();   //Value of Forestry during one rotation
	double forestValueOneNCExt();   //MG: Value of Forestry during one rotation with External Timber price
	//  double plantingCosts();    //Costs to plant x ha of forest

	g4m::ipol<double,double> plantingCosts0;     //Costs to Plant 1ha of forests in ref country
	g4m::ipol<double,double> r;                  //Discount rate
	g4m::ipol<double,double> sAgrSuit;           //Standardised Agricultural Suitability
	g4m::ipol<double,double> sPopDens;           //Standardised Population Density (1-10)
	g4m::ipol<double,double> npp;                //neto primary production
	g4m::ipol<double,double> priceIndex;         //Price Index
	g4m::ipol<double,double> priceIndex0;        //Price Index of reference country
	g4m::ipol<double,double> priceC;             //Carbon Price
	g4m::ipol<double,double> priceLandMin0;      //Minimum Landprice in ref country
	g4m::ipol<double,double> priceLandMax0;      //Maximum Landprice in ref country
	g4m::ipol<double,double> maxRotInter;        //Maximal rotation intervall in years
	g4m::ipol<double,double> minRotInter;        //Minimal rotation intervall
	g4m::ipol<double,double> decLongProd;        //Decay rate for long lived products
	g4m::ipol<double,double> decShortProd;       //Decay rate for short lived products
	g4m::ipol<double,double> fracLongProd;       //Fraction of carbon stored in longterm products
	//Fraction of carbon substracted due to baseline considerations
	g4m::ipol<double,double> baseline;

	//  g4m::ipol<double,double> ftimber;          //Comercial timbervolume per ton of carbon (m3/tC)
	g4m::ipol<double,double> ftimber;
	//      int ftimber;

	g4m::ipol<double,double> priceTimberMax0;    //Maximal timber price in reference country
	g4m::ipol<double,double> priceTimberMin0;    //Minimal timber price in reference country
	g4m::ipol<double,double> fcuptake;            //Factor of carbon uptake from npp
	g4m::ipol<double,double> gdp;                 //Gross domestic production
	g4m::ipol<double,double> harvLoos;
	double forest;                //Share of landarea used by forest [0-1]
	g4m::ipol<double,double> wpricecorr;          //MG: Added for wood price correction
	double wpricecorr0;          //MG: Added for wood price correction in year 2000
	double rotInterM;            //MG: Added rotation interval estimated from Georg's Forest Management Tool
	double harvWood;             //MG: Added harvestable wood estimated from Georg's Forest Management Tool
};

#endif
