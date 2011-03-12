#ifndef READINPUT_CPP
#define READINPUT_CPP

#include "forest.h"

extern settingsT settings;

extern map<string, g4m::ipol<double,double> > lprice;
extern map<string, g4m::ipol<double,double> > wprice;
extern map<string, g4m::ipol<double,double> > wprod;
extern map<string, g4m::ipol<double,double> > cprice;

extern short int yearNwp[11];

pair<double, string> getNumber(string str);

vector< pair<double, string> > header;

//vector<string> asplit(string str);
 
//******************************************************************************
// reads detailed data for plots from input file
//******************************************************************************
int readInputDet(dataDetStruct &data_all)
 {
//Opening a file  
  ifstream fp;
//   string dataInputDir = "C:/MGusti/CurrentWork/GFM/georgPrgs/dima/InputData/";
//   string dataInputDir2 = "data/";
//   string dataInputDir2 = "C:/MGusti/CurrentWork/GFM/georgPrgs/dima/DeforAforCCurves_growth/ManagementPlus/";      

  string dataInputDir = settings.inputPath + "\\";
  string dataInputDir2 = settings.inputPath + "\\";
//cout<< dataInputDir<<endl;
//cout<< dataInputDir2<<endl;

//  string FileName = homeDir + "DataDet.txt";
//  string FileName = dataInputDir + "data_cramer_sort.txt";
//   string FileName = dataInputDir + "b2_cramer_europe_mai.txt"; 
//   string FileName = dataInputDir + "b2_cramer_europe_mai_corr.txt";
//   string FileName = dataInputDir + "b2_cramer_europe_mai_corr_v24wd1c.txt";         
//      string FileName = dataInputDir + "b2_cramer_europe_mai_corr_v24wd1c_gr_nc.txt"; // "original" forest share for Greece is used 4.8 mil ha
      string FileName = dataInputDir + "weo_cramer_europe_mai_corr_v24wd1c_gr_nc.txt"; // Population and GDP are scaled to WEO data

  fp.open(FileName.c_str(), ios::in);
  if (!fp.is_open()) {
    cout << "Cannot read " << FileName << endl;
    system("pause");
    exit(0);
  }
  
//cout << "> Reading the rest of input data..." << FileName << endl;
  string line;
  getline(fp,line);
  stringstream ss(line);
  string buf;
  ss >> buf; ss >> buf;                      // skip x,y
  while (ss >> buf) {
    header.push_back(getNumber(buf));
    transform(header.back().second.begin(),header.back().second.end(),
    header.back().second.begin(), (int(*)(int)) toupper);
  }
  int LineNum = 0;   
  while (!fp.eof()) {
    getline(fp,line);
    if(line[0] != '#' && line.size()>0) {
      g4m::dataStruct d;
      stringstream ss(line);
      double val,x,y;
      ss >> x;
      ss >> y;
      d.x = int((x-GridStepLon/2+180)/GridStepLon);
      d.y = int((y-GridStepLat/2+90)/GridStepLat);
      int ElemNum = 0;
      while (ss >> val) {
//        if (header[ElemNum].first <= eyear)
         {
          if (!strcmp(header[ElemNum].second.c_str(),"COUNTRY"))
            d.COUNTRY.insert(header[ElemNum].first,val);
          else if (!strcmp(header[ElemNum].second.c_str(),"POTVEG"))
            d.POTVEG.insert(header[ElemNum].first,val);
          else if (!strcmp(header[ElemNum].second.c_str(),"PROTECT"))
            d.PROTECT.insert(header[ElemNum].first,val);
          else if (!strcmp(header[ElemNum].second.c_str(),"USED"))
            d.USED.insert(header[ElemNum].first,val);
          else if (!strcmp(header[ElemNum].second.c_str(),"LANDAREA"))
            d.LANDAREA.insert(header[ElemNum].first,val);
          else if (!strcmp(header[ElemNum].second.c_str(),"NPP"))
            d.NPP.insert(header[ElemNum].first,val);

          else if (!strcmp(header[ElemNum].second.c_str(),"POPDENS"))
            d.POPDENS.insert(header[ElemNum].first,val);
          else if (!strcmp(header[ElemNum].second.c_str(),"SPOPDENS")) //MG: Added
            d.SPOPDENS.insert(header[ElemNum].first,val);        
            
          else if (!strcmp(header[ElemNum].second.c_str(),"SAGRSUIT"))
            d.SAGRSUIT.insert(header[ElemNum].first,val);
          else if (!strcmp(header[ElemNum].second.c_str(),"AGRSUIT"))
            d.AGRSUIT.insert(header[ElemNum].first,val);

          else if (!strcmp(header[ElemNum].second.c_str(),"PRICEINDEX"))
            d.PRICEINDEX.insert(header[ElemNum].first,val);

          else if (!strcmp(header[ElemNum].second.c_str(),"BIOMASS"))
            d.BIOMASS.insert(header[ElemNum].first,val);
          else if (!strcmp(header[ElemNum].second.c_str(),"BIOMASSBL"))
            d.BIOMASSBL.insert(header[ElemNum].first,val);            
          else if (!strcmp(header[ElemNum].second.c_str(),"CABOVEHA"))
            d.CABOVEHA.insert(header[ElemNum].first,val);    
          else if (!strcmp(header[ElemNum].second.c_str(),"CBELOWHA"))
            d.CBELOWHA.insert(header[ElemNum].first,val);    
          else if (!strcmp(header[ElemNum].second.c_str(),"CDEADHA"))
            d.CDEADHA.insert(header[ElemNum].first,val);                                               
          else if (!strcmp(header[ElemNum].second.c_str(),"CLITTERHA"))
            d.CLITTERHA.insert(header[ElemNum].first,val);     
          else if (!strcmp(header[ElemNum].second.c_str(),"SOCHA"))
            d.SOCHA.insert(header[ElemNum].first,val);  

          else if (!strcmp(header[ElemNum].second.c_str(),"DECHERB"))
            d.DECHERB.insert(header[ElemNum].first,val);                                               
          else if (!strcmp(header[ElemNum].second.c_str(),"DECWOOD"))
            d.DECWOOD.insert(header[ElemNum].first,val);     
          else if (!strcmp(header[ElemNum].second.c_str(),"DECSOC"))
            d.DECSOC.insert(header[ElemNum].first,val);              
                          
          else if (!strcmp(header[ElemNum].second.c_str(),"FOREST"))
            d.FOREST.insert(header[ElemNum].first,val);
          else if (!strcmp(header[ElemNum].second.c_str(),"R"))
            d.R.insert(header[ElemNum].first,val);
          else if (!strcmp(header[ElemNum].second.c_str(),"GDP"))
            d.GDP.insert(header[ElemNum].first,val);
          else if (!strcmp(header[ElemNum].second.c_str(),"BUILTUP"))
            d.BUILTUP.insert(header[ElemNum].first,val);
          else if (!strcmp(header[ElemNum].second.c_str(),"CROP"))
            d.CROP.insert(header[ElemNum].first,val); 

          else if (!strcmp(header[ElemNum].second.c_str(),"FRACLONGPROD"))
            d.FRACLONGPROD.insert(header[ElemNum].first,val);
          else if (!strcmp(header[ElemNum].second.c_str(),"CORRUPTION"))
            d.CORRUPTION.insert(header[ElemNum].first,val);
          else if (!strcmp(header[ElemNum].second.c_str(),"SLASHBURN"))
            d.SLASHBURN.insert(header[ElemNum].first,val);

          else if (!strcmp(header[ElemNum].second.c_str(),"IIASA_REGION"))
            d.IIASA_REGION.insert(header[ElemNum].first,val);       

          else if (!strcmp(header[ElemNum].second.c_str(),"FTIMBER"))
            d.FTIMBER.insert(header[ElemNum].first,val);                   

          else if (!strcmp(header[ElemNum].second.c_str(),"POLESREG"))
            d.POLESREG.insert(header[ElemNum].first,val);                        

          else if (!strcmp(header[ElemNum].second.c_str(),"MAIE"))
            d.MAIE.insert(header[ElemNum].first,val);        

          else if (!strcmp(header[ElemNum].second.c_str(),"MAIN"))
            d.MAIN.insert(header[ElemNum].first,val);                    

          else if (!strcmp(header[ElemNum].second.c_str(),"MANAGEDSHARE"))
            d.MANAGEDSHARE.insert(header[ElemNum].first,val);                    

          else if (!strcmp(header[ElemNum].second.c_str(),"MANAGEDFLAG"))
            d.MANAGEDFLAG.insert(header[ElemNum].first,val);                                

          else if (!strcmp(header[ElemNum].second.c_str(),"HARVESTCOSTS"))
            d.HARVESTCOSTS.insert(header[ElemNum].first,val);               
            
          else if (!strcmp(header[ElemNum].second.c_str(),"SIMUID"))
            d.SIMUID.insert(header[ElemNum].first,val);               
			
          else if (!strcmp(header[ElemNum].second.c_str(),"COUNTRYREGMIX"))
            d.COUNTRYREGMIX.insert(header[ElemNum].first,val);               			            

//          else if (!strcmp(header[ElemNum].second.c_str(),"LAND"))
//            d.LAND.insert(header[ElemNum].first,val);        
            
          // ...
         }
        ElemNum++;
      }
      data_all.push_back(d);
      LineNum++;
    }
  }
  fp.close();
//cout << "Successfully read " << LineNum << " lines." << endl;
  int ret = LineNum;
//  int regions[11];
//  regions[0]=1;
//  regions[1]=3;
//  regions[2]=2;
//  regions[3]=4;
//  regions[4]=5;
//  regions[5]=9;
//  regions[6]=11;
//  regions[7]=10;
//  regions[8]=8;
//  regions[9]=7;
//  regions[10]=6;
  ifstream fplp;
//  FileName = dataInputDir2+"landPriceNew0.txt"; 
//  FileName = dataInputDir2+"landPrice_ref2_031110.txt";  
//  FileName = dataInputDir2+"landPrice_ref_bas_ydfr_131210.txt";  
//  FileName = dataInputDir2+"landPrice_base_201210.txt";  
//  FileName = dataInputDir2+"landPrice_ref_201210.txt";  
///    FileName = dataInputDir2+"landRent_bas_ydfr_y05.txt"; 
  FileName = dataInputDir2+"landPrice_weo_base_280211.txt"; 
    
  fplp.open(FileName.c_str(), ios::in);
  if (!fplp.is_open()) {
    cout << "Cannot read " << FileName << endl;
    system("pause");
    exit(0);
  }
  getline(fplp,line); // we don't need 1st line
  int lineNum=0;
  while (!fplp.eof()) {
    getline(fplp,line);
    if(line[0] != '#' && line.size()>0) {
      stringstream ss(line);
      string regID;
      double val;
      ss >> regID;
      int ElemNum = 0;
      string colname;
      colname = "re" + IntToStr(lineNum+1) + "price0";
      while (ss >> val) {
        lprice[colname].insert(2000+(ElemNum)*10,val);
        ElemNum++;
      }
      lineNum++;
    }
  }
//cout<<"lprice[27].v(2000)= "<<lprice["re27price0"].v(2010)<<endl;  
  fplp.close();  

  ifstream fpwp;
//  FileName = dataInputDir2+"woodPriceNew0.txt"; 
//  FileName = dataInputDir2+"woodPrice_ref2_031110.txt";
//  FileName = dataInputDir2+"woodPrice_ref_bas_ydfr_131210.txt"; 
//  FileName = dataInputDir2+"woodPrice_base_201210.txt"; 
//  FileName = dataInputDir2+"woodPrice_ref_201210.txt"; 
//  FileName = dataInputDir2+"woodPrice_bas_ydfr_y05.txt"; 
  FileName = dataInputDir2+"woodPrice_weo_base_280211.txt"; 
    
  fpwp.open(FileName.c_str(), ios::in);
  if (!fpwp.is_open()) {
    cout << "Cannot read " << FileName << endl;
    system("pause");
    exit(0);
  }
  getline(fpwp,line);  // we don't need 1st line
  LineNum=0;
  while (!fpwp.eof()) {
    getline(fpwp,line);
    if(line[0] != '#' && line.size()>0) {
      stringstream ss(line);
      string regID;
      double val;
      ss >> regID;
      int ElemNum = 0;
      string colname;
      colname = "re" + IntToStr(LineNum+1) + "price0";
      while (ss >> val) {
        wprice[colname].insert(2000+(ElemNum)*10,val);
        ElemNum++;
      }
      LineNum++;
    }
  }
//cout<<"wprice[27].v(2000)= "<<wprice["re27price0"].v(2010)<<endl;
  fpwp.close();



  ifstream fppr;
//  FileName = dataInputDir2+"woodProductionCountriesNew0_261109.txt"; 
//  FileName = dataInputDir2+"woodProductionCountriesNew0_250210.txt"; 
//  FileName = dataInputDir2+"woodProductionCountriesNew0_280510.txt"; 
//  FileName = dataInputDir2+"woodProductionCountriesNew0_180610.txt";   
//  FileName = dataInputDir2+"woodProductionCountriesNew0_180610_2005.txt";     
//  FileName = dataInputDir2+"woodProductionCountriesNew0_180610_hist2005.txt";       
//  FileName = dataInputDir2+"woodProductionCountriesNew0_260610.txt";   // Italy is updated
//  FileName = dataInputDir2+"woodProductionCountriesNew0_260610_hist2005.txt";     
//  FileName = dataInputDir2+"woodProductionCountriesNew0_290610.txt";   // Sweden is updated
//  FileName = dataInputDir2+"woodProductionCountriesNew0_140710.txt";   // Belgium is corrected
//  FileName = dataInputDir2+"woodProductionCountriesNew0_140710_hist2005.txt";   // Belgium is corrected  
//  FileName = dataInputDir2+"woodProductionCountriesNew0_ref_260710.txt";   // Reference scenario
//  FileName = dataInputDir2+"woodProductionCountriesNew0_ref_080910.txt"; // new referense
//  FileName = dataInputDir2+"woodProductionCountries_ref_bas_ydfr_var_141210.txt"; // new referense
//  FileName = dataInputDir2+"woodProductionCountries_ref_bas_ydfr_ave_141210.txt"; // new referense
//  FileName = dataInputDir2+"woodProductionCountryReg_base_160111.txt"; // new referense
//  FileName = dataInputDir2+"woodProductionCountries_ref_201210.txt"; // new referense
//  FileName = dataInputDir2+"woodProductionCountryReg_refbas_glob_160111.txt"; // Wood production for the global run (by countryreg mix)
  FileName = dataInputDir2+"woodProductionCountryReg_weo_bas_glob_060311.txt"; // Wood production for the global run (by countryreg mix) WEO baseline
  
  fppr.open(FileName.c_str(), ios::in);
  if (!fppr.is_open()) {
    cout << "Cannot read " << FileName << endl;
    system("pause");
    exit(0);
  }
  getline(fppr,line);  // we don't need 1st line
  LineNum=0;
  while (!fppr.eof()) {
    getline(fppr,line);
    if(line[0] != '#' && line.size()>0) 
    {
      stringstream ss(line);
      string regID;
      double val;
      ss >> regID;
      int ElemNum = 0;
      string colname;
      colname = "re" + IntToStr(LineNum+1) + "price0";
		while (ss >> val)
		{
			wprod[colname].insert(yearNwp[ElemNum],val);
			ElemNum++;
		}
		LineNum++;
    }
  }
cout<<"wprod[re1price0].v(2010)= "<<wprod["re1price0"].v(2020)<<endl;  
cout<<"wprod[re27price0].v(2010)= "<<wprod["re27price0"].v(2010)<<endl;
  fppr.close();



//  ifstream fppr;
//  FileName = dataInputDir2+"woodProduction0.txt"; 
//  fppr.open(FileName.c_str(), ios::in);
//  if (!fppr.is_open()) {
//    cout << "Cannot read " << FileName << endl;
//    system("pause");
//    exit(0);
//  }
//  getline(fppr,line);  // we don't need 1st line
//  LineNum=0;
//  while (!fppr.eof()) {
//    getline(fppr,line);
////    if(line[0] != '#' && line.size()>0) 
//    {
//      stringstream ss(line);
//      string regID;
//      double val;
//      ss >> regID;
//      int ElemNum = 0;
//      string colname;
//      char regstr[2]; 
////cout << "regstr"<<"\t"<<regstr<<endl; 
//      int2str(LineNum+1,regstr);
//      colname = "re" + string(regstr) + "price0";
////cout<<"colname"<<"\t"<<colname<<endl;
//      while (ss >> val) {
//        wprod[colname].insert(2000+(ElemNum)*10,val);
////cout<< wprod[colname].v()<<endl;         
//        ElemNum++;
//      }
//      LineNum++;
//    }
//  }
////cout<<"wprod[re27price0].v(2010)= "<<wprod["re27price0"].v(2010)<<endl;
//  fppr.close();


  return(ret);
// finished reading file
 }



//******************************************************************************
// reading coefficients
//******************************************************************************
void readCoeff(g4m::coeffStruct &coeff)
 {
//Opening a file  

  ifstream fp;
//  string FileName = homeDir + "coeff.txt";
//  string FileName = "dimaCoeff_borig.txt";
//  string FileName = "dimaCoeff_borig.txt";
  string FileName = settings.coeffPath;
  fp.open(FileName.c_str(), ios::in);
  if (!fp.is_open()) {
    cout << "Cannot read " << FileName << endl;
    system("pause");
    exit(0);
  }
//  cout << "> Reading coefficients..." << endl;
  while (!fp.eof())
    {
     string line;
     getline(fp, line);
     if(line[0] != '#')                        //Jump over lines starting with #
       {
        transform(line.begin(),line.end(),line.begin(), (int(*)(int)) toupper);
        stringstream ss(line);
        string buf;
        double val;
        ss >> buf;
        pair<double,string> tmp=getNumber(buf);
        ss >> val;
        if (!strcmp(tmp.second.c_str(),"BYEAR"))
          coeff.bYear = int(val);
        else if (!strcmp(tmp.second.c_str(),"EYEAR"))
          coeff.eYear = int(val);
        else if (!strcmp(tmp.second.c_str(),"CELLSINTERACT"))
          coeff.cellsInteract = int(val);
        else if (!strcmp(tmp.second.c_str(),"INCLAFFOR"))
          coeff.inclAffor = int(val);
        else if (!strcmp(tmp.second.c_str(),"NOPAY"))
          coeff.noPay = int(val);
        else if (!strcmp(tmp.second.c_str(),"UBIOMASS"))
          coeff.uBiomass = int(val);
        else if (!strcmp(tmp.second.c_str(),"LITTER"))
          coeff.litter = int(val);
        else if (!strcmp(tmp.second.c_str(),"SOC"))
          coeff.SOC = int(val);
        else if (!strcmp(tmp.second.c_str(),"PRICELANDMINR"))
          coeff.PriceLandMinR.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"PRICELANDMAXR"))
          coeff.PriceLandMaxR.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"FCUPTAKE"))
          coeff.FCuptake.insert(tmp.first,val);
//        else if (!strcmp(tmp.second.c_str(),"FTIMBER"))
//          coeff.FTimber.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"HARVLOOS"))
          coeff.HarvLoos.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"PRICEC"))
          coeff.PriceC.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"FRACLONGPROD"))
          coeff.FracLongProd.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"DECRATEL"))
          coeff.decRateL.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"DECRATES"))
          coeff.decRateS.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"SLASHBURN"))
          coeff.SlashBurn.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"FREQAID"))
          coeff.FreqAid.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"PRICECAID"))
          coeff.PriceCAid.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"MAXROTINTER"))
          coeff.MaxRotInter.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"MINROTINTER"))
          coeff.MinRotInter.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"BASELINE"))
          coeff.baseline.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"PRICETIMBERMAXR"))
          coeff.PriceTimberMaxR.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"PRICETIMBERMINR"))
          coeff.PriceTimberMinR.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"PRICEINDEXE"))
          coeff.PriceIndexE.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"PLANTINGCOSTS"))
          coeff.PlantingCostsR.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"SPOPDENS"))
          coeff.sPopDens.insert(tmp.first,val);
       }
    }
  fp.close();
//cout<<"coeff.PriceIndexE= "<< coeff.PriceIndexE.v()<<endl;
//  cout << "Coefficients successfully read..." << endl;
// finished reading file
 }

//******************************************************************************
// splits string to pair <number,string>
//******************************************************************************
pair<double, string> getNumber(string str)
{ // Returns a pair of double, string;
  string text;
  string number;
  for(unsigned int i=0; i < str.size(); ++i) {
    if((str[i] >= '0' && str[i] <= '9') || str[i] == '.')
       number += str[i];
    else if((str[i] != '[') && (str[i] != ']'))
      text += str[i];
  }
  double x = 0;
  stringstream ss(number);
  ss >> x;
  return(pair<double, string>(x, text));
}



//pair<double, string> getNumber(string str) {
//  //Returns a number which is in a string
//  string text;
//  string number;
//  for(int i=0; i < str.size(); ++i) {
//    if((str[i] >= '0' && str[i] <= '9') || str[i] == '.') {
//      number += str[i];
//    } else {
//      text += str[i];
//    }
//  }
//  double x = 0;
//  stringstream ss(number); // Insert the string into a stream
//  ss >> x;
//
//  return(pair<double, string>(x, text));
//}

//vector<double> dsplit(string str) {
//  //Splits up a string and returns the tokens in a double vector
//  double buf; // Have a double buffer
//  stringstream ss(str); // Insert the string into a stream
//
//  vector<double> tokens; // Create vector to hold our words
//
//  while (ss >> buf) {
//    tokens.push_back(buf);
//  }
//  return(tokens);
//}
//
//vector<string> asplit(string str) {
//  //Splits up a string and returns the tokens in a string vector
//  string buf; // Have a buffer string
//  stringstream ss(str); // Insert the string into a stream
//
//  vector<string> tokens; // Create vector to hold our words
//
//  while (ss >> buf) {
//    tokens.push_back(buf);
//  }
//  return(tokens);
//}


#endif
