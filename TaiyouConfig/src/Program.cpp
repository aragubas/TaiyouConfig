#include <iostream>
#include <filesystem>
#include "Utils.hpp"
#include "Linker/Link.hpp"
#include "Builder.h"

using namespace TaiyouConfig;
using std::filesystem::recursive_directory_iterator;
 
int main(int agrc, const char* argv[])
{
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

	for (int i = 0; i < inputFiles.size(); i++)
	{
		std::string source = FetchTCFG(inputFiles[i].c_str());
		compilationUnits.push_back(TokenizeTcfg(source));
	}

	TaiyouConfig::Linker::LinkedTcfgUnit LinkedUnits = Linker::LinkUnits(compilationUnits);

	int buildResult = TaiyouConfig::Builder::Build(&LinkedUnits, outputFileName.c_str());

	return 0;
}