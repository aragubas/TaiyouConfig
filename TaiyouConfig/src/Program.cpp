#include <iostream>
#include <filesystem>
#include "Utils.hpp"
#include "Linker/Link.hpp"
#include "Builder.hpp"

using namespace TaiyouConfig;
using std::filesystem::recursive_directory_iterator;
bool VerboseOutput = false;

int main(int agrc, const char* argv[])
{
	// Always enable VerboseOutput in debug builds
#ifdef _DEBUG
	VerboseOutput = true;
#endif

	std::vector<std::string> inputFiles;
	std::vector<TcfgUnit> compilationUnits;
	std::string outputFileName = "./out.tcb";

	// No arguments provided, automatic mode
	if (agrc == 1)
	{
		for (const std::filesystem::directory_entry& file : recursive_directory_iterator("./"))
		{
			if (file.path().extension() == ".tcfg")
			{
				inputFiles.push_back(file.path().generic_string());
			}
		}
	}
	else
	{
		throw std::runtime_error("Input file list not implemented yet.");
	}


	if (inputFiles.size() == 0)
	{
		std::cout << "STOP: No input files." << std::endl;
		return -1;
	}

	if (VerboseOutput)
		std::cout << "Input; " << inputFiles.size() << " input files" << std::endl;


	for (int i = 0; i < inputFiles.size(); i++)
	{
		std::string source = FetchTCFG(inputFiles[i].c_str());
		compilationUnits.push_back(TokenizeTcfg(source));
	}

	TaiyouConfig::Linker::LinkedTcfgUnit LinkedUnits = Linker::LinkUnits(compilationUnits);

	int buildResult = TaiyouConfig::Builder::Build(&LinkedUnits, outputFileName.c_str());

	return 0;
}