#include "inputFileInfo.h"

inputFileInfoT::inputFileInfoT(std::string infoFile)
{
	// TODO: setting up input file names and (maybe) other parameters passed from GUI
	folders.inputDir = "data\\";
	folders.outputDir = "..\\GLOBIOM GUI\\data\\g4m\\";

	// File names
	files.simuBinFileName = folders.inputDir + "simu.bin";
	files.outMapFileName = /*folders.outputDir +*/ "my_test_map";
	files.outTableFileName = /*folders.outputDir +*/ "my_test_table";
}

inputFileInfoT::~inputFileInfoT()
{

}