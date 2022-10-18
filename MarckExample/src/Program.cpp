#include <iostream>
#include <fstream>
#include "KeysStore.hpp"
#include "Utils.hpp"
#include "Linker/UncompiledNamespace.hpp"
#include "Linker/Link.hpp"
#include <bitset>

using namespace TaiyouConfig;

int main(int agrc, const char* argv[])
{
	std::string mainSource = FetchTCFG("./mainconfig.tcfg");
	TcfgUnit mainUnit = TokenizeTcfg(mainSource);

	std::string ceiraSource = FetchTCFG("./ceira.tcfg");
	TcfgUnit ceiraUnit = TokenizeTcfg(ceiraSource);


	std::vector<TcfgUnit> units{ mainUnit, ceiraUnit };

	TaiyouConfig::Linker::LinkedTcfgUnit LinkedUnits = Linker::LinkUnits(units);

	std::vector<char> binaryFile;

	// Magic Number 
	binaryFile.push_back('T');
	binaryFile.push_back('C');
	binaryFile.push_back('B');
	
	// Version
	binaryFile.push_back(1); // Major Revision
	binaryFile.push_back(0); // Minor Revision

	// Padding
	//for (int i = 0; i < 4 << i++;)
	//	header.push_back(0);

	// Control Keyword: Global Namespace
	binaryFile.push_back(0x69);
	
	// Convert all keys in global namespace to binary
	for (int i = 0; i < LinkedUnits.GlobalKeys.size(); i++)
	{
		TaiyouConfig::Token::UnparsedKey key = LinkedUnits.GlobalKeys[i];

		// Control Keyword: Declaring Key
		binaryFile.push_back(0x42);
		
		// Key Type		
		for (int ch = 0; ch < key.Type.size(); ch++)
		{
			binaryFile.push_back(key.Type[ch]);
		}
		binaryFile.push_back('\0');

		// Key Name
		for (int ch = 0; ch < key.Name.size(); ch++)
		{
			binaryFile.push_back(key.Name[ch]);
		}
		binaryFile.push_back('\0');

		// Key Value
		for (int ch = 0; ch < key.Value.size(); ch++)
		{
			binaryFile.push_back(key.Value[ch]);
		}
		binaryFile.push_back('\0');
	}

	std::ofstream stream("./out.tcb", std::ios::out | std::ios::binary);
	if (!stream)
	{
		std::cerr << "Could not open output file." << std::endl;
		return -1;
	}

	for (int i = 0; i < binaryFile.size(); i++)
	{
		stream.write((char*)&binaryFile[i], sizeof(char));
	}

	stream.close();

	if (!stream.good())
	{
		std::cerr << "Could not write output file." << std::endl;
		return -1;
	}

	// Read the output file just created
	KeysStore keyStore;
	keyStore.LoadDatabase("./out.tcb");

	//std::cout << ceira << std::endl;


	return 0;
}