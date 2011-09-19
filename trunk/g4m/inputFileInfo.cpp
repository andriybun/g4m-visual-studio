#include "inputFileInfo.h"

inputFileInfoT::inputFileInfoT(std::string infoFile, std::string & workingDir)
{
	xmlParser pathsParser(infoFile);
	xmlParser::executionResultT execResult;

	// TODO: setting up input file names and (maybe) other parameters passed from GUI
	getValueOrInvalidate("input_dir", folders.inputDir);
	getValueOrInvalidate("output_dir", folders.outputDir);
	folders.inputDir = workingDir + folders.inputDir;
	folders.outputDir = workingDir + folders.outputDir;
	folders.workingDir = workingDir;

	// File names
	getValueOrInvalidate("output_parameters_file", files.outputParametersFileName);
	getValueOrInvalidate("simu_bin_file", files.simuBinFileName);
	getValueOrInvalidate("out_map_name", files.outMapFileName);
	getValueOrInvalidate("out_table_name", files.outTableFileName);

	files.outputParametersFileName = workingDir + files.outputParametersFileName;
	files.simuBinFileName = workingDir + files.simuBinFileName;

	produceMaps = true;
	produceTables = true;
}

inputFileInfoT::~inputFileInfoT()
{

}