#include "inputFileInfo.h"

inputFileInfoT::inputFileInfoT(std::string infoFile, std::string & workingDir)
{
	xmlParser pathsParser(infoFile);
	xmlParser::executionResultT execResult;

	// TODO: setting up input file names and (maybe) other parameters passed from GUI
	getValueOrInvalidate("input_dir", folders.inputDir);
	getValueOrInvalidate("output_dir", folders.outputDir);

	// File names
	getValueOrInvalidate("simu_bin_file", files.simuBinFileName);
	getValueOrInvalidate("out_map_name", files.outMapFileName);
	getValueOrInvalidate("out_table_name", files.outTableFileName);

	produceMaps = true;
	produceTables = true;
}

inputFileInfoT::~inputFileInfoT()
{

}