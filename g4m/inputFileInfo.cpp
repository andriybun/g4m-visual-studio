#include "inputFileInfo.h"

inputFileInfoT::inputFileInfoT(std::string infoFile)
{
	// TODO: setting up input file names and (maybe) other parameters passed from GUI
	folders.inputDir = "data\\";

	files.simuBinFileName = folders.inputDir + "simu.bin";
}

inputFileInfoT::~inputFileInfoT()
{

}