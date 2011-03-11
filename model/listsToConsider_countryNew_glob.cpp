//Lists of countries, regions and mixed to be considered
//

#include "forest.h"

extern set<int> regions;
extern set<int> years;
extern set<int> countriesList;
extern set<int> countryregList;

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

void listsToConsider(void)
{
// Setting years for output

  years.insert(2000);
  years.insert(2001);
  years.insert(2002);
  years.insert(2003);
  years.insert(2004);
  years.insert(2005);
  years.insert(2006);
  years.insert(2007);
  years.insert(2008);
  years.insert(2009);
  years.insert(2010);
  years.insert(2011);
  years.insert(2012);
  years.insert(2013);
  years.insert(2014);
  years.insert(2015);
  years.insert(2016);
  years.insert(2017);
  years.insert(2018);
  years.insert(2019);
  years.insert(2020);
  years.insert(2021);
  years.insert(2022);
  years.insert(2023);
  years.insert(2024);
  years.insert(2025);
  years.insert(2026);  
  years.insert(2027);
  years.insert(2028);
  years.insert(2029);
  years.insert(2030);
  years.insert(2031);
  years.insert(2032);
  years.insert(2033);
  years.insert(2034);
  years.insert(2035);
  years.insert(2036);  
  years.insert(2037);
  years.insert(2038);
  years.insert(2039);
  years.insert(2040);
  years.insert(2041);
  years.insert(2042);
  years.insert(2043);
  years.insert(2044);
  years.insert(2045);
  years.insert(2046);  
  years.insert(2047);
  years.insert(2048);
  years.insert(2049);
  years.insert(2050);
//----------------------------------------------------------------------------

// Setting regions for calculations:
  regions.insert(1);  //Australia&New Zealand
  regions.insert(3);  //Canada
  regions.insert(5);  //EU_Baltic
  regions.insert(6);  //EU_CentralEast
  regions.insert(7);  // EU_MidWest
  regions.insert(8);  //EU_North
  regions.insert(9);  //EU_South
  regions.insert(10);  // Former USSR
  regions.insert(12);  //JapanReg
  regions.insert(17);  //RCEU (Rest of Central Europe)
  regions.insert(18);  //ROWE (Rest of Western Europe)
  regions.insert(26);  //TurkeyReg
  regions.insert(27);  //USAReg

  regions.insert(2); //BrazilReg
  regions.insert(4); //ChinaReg
  regions.insert(11); //IndiaReg 
  regions.insert(13);  //MexicoReg
  regions.insert(14);  //MidEastNorthAfr
  regions.insert(15);  //Pacific_Islands
  regions.insert(16);  //RCAM (Rest of Central America)
  regions.insert(19);  //RSAM (Rest of South America)
  regions.insert(20);  //RSAS (Rest of South Asia)
  regions.insert(21);  //RSEA_OPA (Rest of South-East Asia Pacific)
  regions.insert(22);  //RSEA_PAC (Rest of South-East Asia Pacific)
  regions.insert(23);  //SouthAfrReg
  regions.insert(24);  //SouthKorea
  regions.insert(25);  //SubSaharanAfr (Sub-Saharan Africa)


//------------------------------------------------------------------------------
// CountryRegMix to be considered
    countryregList.insert(	0	);
    countryregList.insert(	1	);
    countryregList.insert(	2	);
    countryregList.insert(	3	);
    countryregList.insert(	4	);
    countryregList.insert(	5	);
    countryregList.insert(	6	);
    countryregList.insert(	7	);
    countryregList.insert(	8	);
    countryregList.insert(	9	);
    countryregList.insert(	10	);
    countryregList.insert(	11	);
    countryregList.insert(	12	);
    countryregList.insert(	13	);
    countryregList.insert(	14	);
    countryregList.insert(	15	);
    countryregList.insert(	16	);
    countryregList.insert(	17	);
    countryregList.insert(	18	);
    countryregList.insert(	19	);
    countryregList.insert(	20	);
    countryregList.insert(	21	);
    countryregList.insert(	22	);
    countryregList.insert(	23	);
    countryregList.insert(	24	);
    countryregList.insert(	25	);
    countryregList.insert(	26	);
    countryregList.insert(	27	);
    countryregList.insert(	28	);
    countryregList.insert(	29	);
    countryregList.insert(	30	);
    countryregList.insert(	31	);
    countryregList.insert(	32	);
    countryregList.insert(	33	);
    countryregList.insert(	34	);
    countryregList.insert(	35	);
    countryregList.insert(	36	);
    countryregList.insert(	37	);
    countryregList.insert(	38	);
    countryregList.insert(	39	);
    countryregList.insert(	40	);
    countryregList.insert(	41	);
    countryregList.insert(	42	);
    countryregList.insert(	43	);
    countryregList.insert(	44	);
    countryregList.insert(	45	);
    countryregList.insert(	46	);
    countryregList.insert(	47	);
    countryregList.insert(	48	);
    countryregList.insert(	49	);

//------------------------------------------------------------------------------
//countries to be considered
    countriesList.insert(	2	);
    countriesList.insert(	3	);
    countriesList.insert(	6	);
    countriesList.insert(	7	);
    countriesList.insert(	9	);
    countriesList.insert(	10	);
    countriesList.insert(	11	);
    countriesList.insert(	13	);
    countriesList.insert(	15	);
    countriesList.insert(	16	);
    countriesList.insert(	17	);
    countriesList.insert(	18	);
    countriesList.insert(	19	);
    countriesList.insert(	20	);
    countriesList.insert(	21	);
    countriesList.insert(	22	);
    countriesList.insert(	23	);
    countriesList.insert(	24	);
    countriesList.insert(	25	);
    countriesList.insert(	26	);
    countriesList.insert(	27	);
    countriesList.insert(	28	);
    countriesList.insert(	29	);
    countriesList.insert(	30	);
    countriesList.insert(	31	);
    countriesList.insert(	32	);
    countriesList.insert(	33	);
    countriesList.insert(	34	);
    countriesList.insert(	35	);
    countriesList.insert(	37	);
    countriesList.insert(	38	);
    countriesList.insert(	39	);
    countriesList.insert(	41	);
    countriesList.insert(	42	);
    countriesList.insert(	43	);
    countriesList.insert(	44	);
    countriesList.insert(	45	);
    countriesList.insert(	46	);
    countriesList.insert(	47	);
    countriesList.insert(	49	);
    countriesList.insert(	50	);
    countriesList.insert(	51	);
    countriesList.insert(	52	);
    countriesList.insert(	53	);
    countriesList.insert(	56	);
    countriesList.insert(	57	);
    countriesList.insert(	58	);
    countriesList.insert(	59	);
    countriesList.insert(	60	);
    countriesList.insert(	61	);
    countriesList.insert(	62	);
    countriesList.insert(	63	);
    countriesList.insert(	64	);
    countriesList.insert(	65	);
    countriesList.insert(	66	);
    countriesList.insert(	67	);
    countriesList.insert(	68	);
    countriesList.insert(	69	);
    countriesList.insert(	70	);
    countriesList.insert(	71	);
    countriesList.insert(	72	);
    countriesList.insert(	73	);
    countriesList.insert(	74	);
    countriesList.insert(	75	);
    countriesList.insert(	77	);
    countriesList.insert(	78	);
    countriesList.insert(	79	);
    countriesList.insert(	80	);
    countriesList.insert(	82	);
    countriesList.insert(	83	);
    countriesList.insert(	84	);
    countriesList.insert(	85	);
    countriesList.insert(	86	);
    countriesList.insert(	87	);
    countriesList.insert(	88	);
    countriesList.insert(	89	);
    countriesList.insert(	90	);
    countriesList.insert(	91	);
    countriesList.insert(	93	);
    countriesList.insert(	94	);
    countriesList.insert(	96	);
    countriesList.insert(	97	);
    countriesList.insert(	98	);
    countriesList.insert(	99	);
    countriesList.insert(	100	);
    countriesList.insert(	101	);
    countriesList.insert(	103	);
    countriesList.insert(	104	);
    countriesList.insert(	105	);
    countriesList.insert(	106	);
    countriesList.insert(	107	);
    countriesList.insert(	108	);
    countriesList.insert(	109	);
    countriesList.insert(	110	);
    countriesList.insert(	111	);
    countriesList.insert(	112	);
    countriesList.insert(	113	);
    countriesList.insert(	114	);
    countriesList.insert(	115	);
    countriesList.insert(	116	);
    countriesList.insert(	117	);
    countriesList.insert(	118	);
    countriesList.insert(	119	);
    countriesList.insert(	120	);
    countriesList.insert(	121	);
    countriesList.insert(	122	);
    countriesList.insert(	123	);
    countriesList.insert(	124	);
    countriesList.insert(	125	);
    countriesList.insert(	126	);
    countriesList.insert(	127	);
    countriesList.insert(	128	);
    countriesList.insert(	129	);
    countriesList.insert(	130	);
    countriesList.insert(	131	);
    countriesList.insert(	132	);
    countriesList.insert(	133	);
    countriesList.insert(	134	);
    countriesList.insert(	135	);
    countriesList.insert(	136	);
    countriesList.insert(	137	);
    countriesList.insert(	139	);
    countriesList.insert(	140	);
    countriesList.insert(	141	);
    countriesList.insert(	142	);
    countriesList.insert(	143	);
    countriesList.insert(	144	);
    countriesList.insert(	145	);
    countriesList.insert(	146	);
    countriesList.insert(	147	);
    countriesList.insert(	149	);
    countriesList.insert(	150	);
    countriesList.insert(	151	);
    countriesList.insert(	152	);
    countriesList.insert(	154	);
    countriesList.insert(	155	);
    countriesList.insert(	156	);
    countriesList.insert(	158	);
    countriesList.insert(	159	);
    countriesList.insert(	161	);
    countriesList.insert(	162	);
    countriesList.insert(	163	);
    countriesList.insert(	165	);
    countriesList.insert(	166	);
    countriesList.insert(	167	);
    countriesList.insert(	168	);
    countriesList.insert(	170	);
    countriesList.insert(	171	);
    countriesList.insert(	173	);
    countriesList.insert(	174	);
    countriesList.insert(	175	);
    countriesList.insert(	176	);
    countriesList.insert(	177	);
    countriesList.insert(	178	);
    countriesList.insert(	179	);
    countriesList.insert(	180	);
    countriesList.insert(	181	);
    countriesList.insert(	182	);
    countriesList.insert(	183	);
    countriesList.insert(	184	);
    countriesList.insert(	185	);
    countriesList.insert(	186	);
    countriesList.insert(	187	);
    countriesList.insert(	188	);
    countriesList.insert(	189	);
    countriesList.insert(	192	);
    countriesList.insert(	193	);
    countriesList.insert(	194	);
    countriesList.insert(	195	);
    countriesList.insert(	196	);
    countriesList.insert(	197	);
    countriesList.insert(	199	);
    countriesList.insert(	200	);
    countriesList.insert(	201	);
    countriesList.insert(	202	);
    countriesList.insert(	203	);
    countriesList.insert(	204	);
    countriesList.insert(	205	);
    countriesList.insert(	206	);
    countriesList.insert(	207	);
    countriesList.insert(	208	);
    countriesList.insert(	209	);
    countriesList.insert(	210	);
    countriesList.insert(	211	);
    countriesList.insert(	212	);
    countriesList.insert(	214	);
    countriesList.insert(	216	);
    countriesList.insert(	217	);
    countriesList.insert(	218	);
    countriesList.insert(	219	);
    countriesList.insert(	221	);
    countriesList.insert(	222	);
    countriesList.insert(	223	);
    countriesList.insert(	224	);
    countriesList.insert(	226	);
    countriesList.insert(	227	);
    countriesList.insert(	228	);
    countriesList.insert(	230	);
    countriesList.insert(	231	);
    countriesList.insert(	234	);
    countriesList.insert(	235	);
    countriesList.insert(	237	);
    countriesList.insert(	238	);
    countriesList.insert(	239	);
    countriesList.insert(	240	);
    countriesList.insert(	241	);
    countriesList.insert(	243	);

//---------------------------
//Set list of countries to print results

 CountriesNforCover.setListOfCountries(countriesList); 
 CountriesAfforHaYear.setListOfCountries(countriesList); 

 CountriesNforTotC.setListOfCountries(countriesList); 
 CountriesAfforCYear.setListOfCountries(countriesList); 
 CountriesAfforCYear_ab.setListOfCountries(countriesList); 
 CountriesAfforCYear_bl.setListOfCountries(countriesList);   
 CountriesAfforCYear_biom.setListOfCountries(countriesList);
 CountriesAfforCYear_dom.setListOfCountries(countriesList);    
 CountriesAfforCYear_soil.setListOfCountries(countriesList);    
//--------- 
 CountriesOforCover.setListOfCountries(countriesList); 
 CountriesDeforHaYear.setListOfCountries(countriesList); 
 
 CountriesOfor_abC.setListOfCountries(countriesList);
 CountriesOforC_biom.setListOfCountries(countriesList);  
 CountriesDeforCYear.setListOfCountries(countriesList);   
 CountriesDeforCYear_bl.setListOfCountries(countriesList);   
 CountriesDeforCYear_ab.setListOfCountries(countriesList); 
 CountriesDeforCYear_biom.setListOfCountries(countriesList);  
 CountriesDeforCYear_dom.setListOfCountries(countriesList);     
 CountriesDeforCYear_soil.setListOfCountries(countriesList);      

//--------- 

 CountriesWoodHarvestM3Year.setListOfCountries(countriesList); 
 CountriesWoodHarvestPlusM3Year.setListOfCountries(countriesList);  
 CountriesWoodHarvestFmM3Year.setListOfCountries(countriesList);  
 CountriesWoodHarvestDfM3Year.setListOfCountries(countriesList);
 CountriesWoodLoosCYear.setListOfCountries(countriesList);
 CountriesHarvLossesYear.setListOfCountries(countriesList);
//---------     
 CountriesManagedForHa.setListOfCountries(countriesList);
 CountriesManagedCount.setListOfCountries(countriesList); 
 
 CountriesMAI.setListOfCountries(countriesList);
 CountriesCAI.setListOfCountries(countriesList);
 CountriesCAI_new.setListOfCountries(countriesList);

 CountriesFM.setListOfCountries(countriesList);
 CountriesFMbm.setListOfCountries(countriesList);
//---------   
 //CountriesWprod.setListOfCountries(countriesList);
//---------
 CountriesProfit.setListOfCountries(countriesList); 
//*******************************************************************************
//---------

//{
//set<int>::iterator it;
//////it=countriesList.find(43);
//////countriesList.erase(it);
////it=countriesList.find(45);
////countriesList.erase(it);
//////it=countriesList.find(112);
//////countriesList.erase(it);
//////it=countriesList.find(114);
//////countriesList.erase(it);
////it=countriesList.find(122);
////countriesList.erase(it);
//////it=countriesList.find(142);
//////countriesList.erase(it);
//////it=countriesList.find(180);
//////countriesList.erase(it);
//
//cout << "The countries processed:"<<endl;
//for (it=countriesList.begin(); it!=countriesList.end(); it++)
//cout << *it<<endl;
//}  


}