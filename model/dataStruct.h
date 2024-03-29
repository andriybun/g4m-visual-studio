#ifndef G4M_DATASTRUCT_H
#define G4M_DATASTRUCT_H

#include <string>
#include "ipol.h"

namespace g4m{

struct dataStruct
{
	int	x	;
	int	y	;
	int asID;  // index of corresponding data in ageStruct vector

	//  std::string classes;
	ipol<double,double>	COUNTRY;
	ipol<double,double>	POTVEG;
	ipol<double,double>	PROTECT;
	ipol<double,double>	USED;
	ipol<double,double>	LANDAREA	;
	ipol<double,double>	NPP	;
	ipol<double,double>	POPDENS	;
	ipol<double,double>	SAGRSUIT	;
	ipol<double,double>	AGRSUIT	;
	ipol<double,double>	PRICEINDEX	;
	ipol<double,double>	BIOMASS	;
	ipol<double,double>	FOREST	;
	ipol<double,double>	R	;
	ipol<double,double>	GDP	;
	ipol<double,double>	BUILTUP	;
	ipol<double,double>	CROP	;
	ipol<double,double>	FRACLONGPROD	;
	ipol<double,double>	CORRUPTION	;
	ipol<double,double>	SLASHBURN	;
	ipol<double,double>	SPOPDENS	; // MG: added because it's used in forest_calculations
	ipol<double,double>	BIOMASSBL	;
	ipol<double,double>	DECHERB	;
	ipol<double,double>	DECWOOD	;
	ipol<double,double>	DECSOC	;
	ipol<double,double>	CABOVEHA	;
	ipol<double,double>	CBELOWHA	;
	ipol<double,double>	CDEADHA	;
	ipol<double,double>	CLITTERHA	;
	ipol<double,double>	SOCHA	;
	ipol<double,double>	FTIMBER	;
	ipol<double,double>	IIASA_REGION	;
	ipol<double,double>	POLESREG	;
	ipol<double,double>	MAIE	;
	ipol<double,double>	MAIN	;
	ipol<double,double>	MANAGEDSHARE	;
	ipol<double,double>	MANAGEDFLAG	;
	ipol<double,double>	HARVESTCOSTS	;
	ipol<double,double>	SIMUID	;
	ipol<double,double>	COUNTRYREGMIX	;

	//ipol<double,double>	LAND	;

	// Constructor: initializing default values
	dataStruct()
	{
		x=0;
		y=0;
		//    classes = "AAA";
	}
};


struct coeffStruct
{
	// Starting Year of simmulation
	short int bYear;
	// Ending Year of simmulation
	short int eYear;
	// Interaction between cells
	short int cellsInteract;
	// Consider afforestation
	short int inclAffor;
	// No pay
	short int noPay;
	// Belowground biomass
	short int uBiomass;
	// Litter
	short int litter;
	// Soil organic carbon
	int SOC;
	//******************************************************************************
	//** parameters
	//******************************************************************************
	// Priceindex of reference country
	ipol<double,double> PriceIndexE;
	// Minimum Landprice [cash/ha]
	ipol<double,double> PriceLandMinR;
	// Maximum Landprice [cash/ha]
	ipol<double,double> PriceLandMaxR;
	// Factor Carbon uptake (DIMA-Model)
	ipol<double,double> FCuptake;
	// Comercial timbervolume per ton of carbon [m3/tC]
	//  ipol<double,double> FTimber;
	// HarvestingLosses (Share of losses during harvest)
	ipol<double,double> HarvLoos;
	// Carbon price [Cash/tC] (9/25)
	ipol<double,double> PriceC;
	// Share of Long-living products [0-1]
	ipol<double,double> FracLongProd;
	// Decrease rate of long living products
	ipol<double,double> decRateL;
	// Decrease rate of short living products
	ipol<double,double> decRateS;
	// Share of SlashBurn deforestation [0-1]
	ipol<double,double> SlashBurn;
	// Frequency of aid payments (PRICECAID) [Years]
	ipol<double,double> FreqAid;
	// Aid Carbon Price [Cash/tC/FREQAID] (6)
	ipol<double,double> PriceCAid;
	// Maximum rotation time im Years
	ipol<double,double> MaxRotInter;
	// Minimum rotation time im Years
	ipol<double,double> MinRotInter;
	// Baseline
	ipol<double,double> baseline;
	// Maximum Timberprice [cash/m3]
	ipol<double,double> PriceTimberMaxR;
	// Minimum Timberprice [cash/m3]
	ipol<double,double> PriceTimberMinR;
	// Planting costs in reference country [Cash/ha]
	ipol<double,double> PlantingCostsR;
	// Standardised Populationdensity [1-10]
	ipol<double,double> sPopDens;
 
	// Constructor: initializing default values
	coeffStruct()
	{
		bYear = 2000;
		eYear = 2005;
	}
};

}
#endif
