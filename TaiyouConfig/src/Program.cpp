#include <iostream>
#include <filesystem>
#include "Utils.hpp"
#include "Linker/Link.hpp"
#include "Builder.hpp"

using namespace TaiyouConfig;
using std::filesystem::recursive_directory_iterator;
bool VerboseOutput = false;

uint8_t OutputFile_Version_Major = 1;
uint8_t OutputFile_Version_Minor = 0;

void print_help()
{
	std::cout << "TaiyouConfigCompiler (TCFG_C) v1.0.0 by Aragubas" << std::endl;
	std::cout << "Builder file version: " << std::to_string(OutputFile_Version_Major) << "." << std::to_string(OutputFile_Version_Minor) << std::endl;

	std::cout << "Usage: tcfg_c <file1.tcfg> <file2.tcfg> [...] -out <output file path>" << std::endl;
	std::cout << "If no arguments are provided, all .tcfg files in the current working directory and all subfolders will be used as input files" << std::endl;
	std::cout << "  and the output file will be created in the current working directory with the name of \"out.tcb\"" << std::endl;
}

int main(int argc, const char* argv[])
{
	// Always enable VerboseOutput in debug builds
#ifdef _DEBUG
	VerboseOutput = true;
#endif

	std::vector<std::string> inputFiles;
	std::vector<TcfgUnit> compilationUnits;
	std::string outputFileName = "./out.tcb";

	// No arguments provided, automatic mode
	if (argc == 1)
	{
		for (const std::filesystem::directory_entry& file : recursive_directory_iterator("./"))
		{
			if (file.path().extension() == ".tcfg")
			{
				inputFiles.push_back(file.path().generic_string());
			}
		}
	}
	else if (argc == 2)
	{
		if (strcmp(argv[1], "--help") || strcmp(argv[1], "-help") || strcmp(argv[1], "--/?") || strcmp(argv[1], "help"))
		{
			print_help();
			return 0;
		}
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