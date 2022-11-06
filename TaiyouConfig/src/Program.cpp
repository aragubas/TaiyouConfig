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
	std::cout << "TaiyouConfigCompiler (tcfg_c) v1.0.0 by Aragubas" << std::endl;
	std::cout << "Builder file version: " << std::to_string(OutputFile_Version_Major) << "." << std::to_string(OutputFile_Version_Minor) << std::endl;

	std::cout << std::endl;

	std::cout << "Usage: tcfg_c file1.tcfg file2.tcfg [...] -out <output file path>" << std::endl;
	std::cout << "You can also: tcfg_c -src <source path> file1.tcfg file2.cfg [...] -out <output file path>" << std::endl;
	std::cout << "Input files specified directly must be in the current working directory" << std::endl;
	std::cout << "If no arguments are provided, all .tcfg files in the current working directory and all subfolders will be used as input files" << std::endl;
	std::cout << "  and the output file will be created in the current working directory with the name of \"out.tcb\"" << std::endl;

	std::cout << std::endl;

	std::cout << "-src <path>               Source path" << std::endl;
	std::cout << "-out <file path>          Output file path" << std::endl;
	std::cout << "-verbose                  Enables verbose output" << std::endl;
	std::cout << "-list                     Print all input files" << std::endl;
}

int main(int argc, char* argv[])
{
	// Always enable VerboseOutput in debug builds
#ifdef _DEBUG
	VerboseOutput = true;
#endif

	std::vector<std::string> inputFiles;
	std::vector<TcfgUnit> compilationUnits;
	std::string outputFileName = "./out.tcb";
	bool listInputFiles = false;

	// No arguments provided, automatic mode
	if (argc == 0 || argc == 1)
	{  
		if (argc == 0)
		{
			std::cout << "WARNING: The first argument should contain the working directory of this executable." << std::endl << "  This may result in unexpected behaviour when parsing arguments." << std::endl;
		}

		for (const std::filesystem::directory_entry& file : recursive_directory_iterator("./"))
		{
			if (file.path().extension() == ".tcfg")
			{
				inputFiles.push_back(file.path().generic_string());
			}
		}
	}
	else if (argc == 2) // Only one argument was provided
	{
		if (strcmp(argv[1], "--help") || strcmp(argv[1], "-help") || strcmp(argv[1], "/?") || strcmp(argv[1], "help"))
		{
			print_help();
			return 0;
		}
	}
	else // More than two arguments provided
	{
		bool outputSwitch = false;
		bool sourcePathSwitch = false;
		bool inputFile = false;
		bool outputPathDefined = false;
		std::string sourcePathDir;

		// ceira.tcfg -out ceira.out
		for (int i = 1; i < argc; i++)
		{
			std::string argument = argv[i];

			// Argument is a switch
			if (argument.starts_with("-"))
			{ 
				if (argument == "-out")
				{
					outputSwitch = true;
					inputFile = false;

					continue;
				}
				else if (argument == "-verbose")
				{
					VerboseOutput = true;

					continue;
				}
				else if (argument == "-src")
				{
					sourcePathSwitch = true;
					inputFile = false;

					continue;
				}
				else
				{
					std::cout << "Error; Invalid switch \"" << argument << "\"" << std::endl;
					std::cout << "Try --help (or -help, /? and help) to see a list of available arguments and switches." << std::endl;
					return -1;
				}
			}
			else
			{
				if (!outputSwitch && !sourcePathSwitch)
					inputFile = true;
			}

			if (inputFile)
			{
				inputFiles.push_back(argv[i]);
			}
			else if (outputSwitch)
			{
				if (!outputPathDefined)
				{
					outputPathDefined = true;
					outputFileName = argument;

					outputSwitch = false;
				}
				else
				{
					std::cout << "Error; Output file defined multiple times." << std::endl;
					return -1;
				}

			}
			else if (sourcePathSwitch)
			{
				if (sourcePathDir != "")
				{
					std::cout << "Error; Source path defined multiple times." << std::endl;
					return -1;
				}
				
				sourcePathDir = argument;

				sourcePathSwitch = false;
			}

		}

		if (sourcePathDir != "")
		{
			for (const std::filesystem::directory_entry& file : recursive_directory_iterator(sourcePathDir))
			{
				if (file.path().extension() == ".tcfg")
				{
					inputFiles.push_back(file.path().generic_string());
				}
			}
		}
		
	}
	
	if (inputFiles.size() == 0)
	{
		std::cout << "Error; Nothing to do, no input files found." << std::endl;
		return 0;
	}

	if (VerboseOutput)
		std::cout << "Input; " << inputFiles.size() << " input files" << std::endl;


	for (int i = 0; i < inputFiles.size(); i++)
	{
		std::string source = FetchTCFG(inputFiles[i].c_str());
		compilationUnits.push_back(TokenizeTcfg(source));
	}

	TaiyouConfig::Linker::LinkedTcfgUnit LinkedUnits = Linker::LinkUnits(compilationUnits);

	return TaiyouConfig::Builder::Build(&LinkedUnits, outputFileName.c_str());
}