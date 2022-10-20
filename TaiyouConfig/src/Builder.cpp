#include "Builder.h"
#include "Linker/LinkedTcfgUnit.hpp"
#include <iostream>
#include <fstream>

namespace TaiyouConfig::Builder
{
	using namespace TaiyouConfig::Token;
	using namespace TaiyouConfig::Linker;

	int Build(LinkedTcfgUnit linkedUnit[], const char* outputFileName)
	{
		//const char* FileHeader = "TCB" + (char)1 + (char)0;
		//std::cout << FileHeader << std::endl;

		std::vector<char> fileBuffer;
		// Write TCB header
		WriteTCBHeader(&fileBuffer);
		
		// Write all global namespace keys
		WriteGlobalNamespaceKeys(&fileBuffer, linkedUnit);

		// Write all namespaces		
		WriteNamespaces(&fileBuffer, linkedUnit);

		std::ofstream stream(outputFileName, std::ios::out | std::ios::binary);
		if (!stream)
		{
			std::cerr << "Could not open output file." << std::endl;
			return -1;
		}

		for (int i = 0; i < fileBuffer.size(); i++)
		{
			stream.write((char*)&fileBuffer[i], sizeof(char));
		}

		//stream.close();
		stream.flush();

		if (!stream.good())
		{
			std::cerr << "Could not write output file." << std::endl;
			return -1;
		}

		std::cerr << "Builder: --> \"" << outputFileName << "\"" << std::endl;

		return 0;
	}

	inline void WriteKey(std::vector<char>* buffer, TaiyouConfig::Token::UnparsedKey key)
	{
		// Key Type		
		for (int ch = 0; ch < key.Type.size(); ch++)
		{
			buffer->push_back(key.Type[ch]);
		}
		buffer->push_back('\0');

		// Key Name
		for (int ch = 0; ch < key.Name.size(); ch++)
		{
			buffer->push_back(key.Name[ch]);
		}
		buffer->push_back('\0');

		// Key Value
		for (int ch = 0; ch < key.Value.size(); ch++)
		{
			buffer->push_back(key.Value[ch]);
		}
		buffer->push_back('\0');

	}

	inline void WriteGlobalNamespaceKeys(std::vector<char>* buffer, TaiyouConfig::Linker::LinkedTcfgUnit linkedUnit[])
	{
		// Control Keyword: Global Namespace Keys
		buffer->push_back(KEYWORD_GLOBALNAMESPACE);

		for (int i = 0; i < linkedUnit->GlobalKeys.size(); i++)
		{
			TaiyouConfig::Token::UnparsedKey key = linkedUnit->GlobalKeys[i];

			// Control Keyword: Declaring Key
			buffer->push_back(KEYWORD_DECLARE_KEY);

			WriteKey(buffer, key);
		}


	}

	inline void WriteNamespaces(std::vector<char>* buffer, TaiyouConfig::Linker::LinkedTcfgUnit linkedUnit[])
	{
		// Control Keyword: Declaring Namespaces
		buffer->push_back(KEYWORD_DECLARE_NAMESPACE);

		for (int i = 0; i < linkedUnit->Namespaces.size(); i++)
		{
			TaiyouConfig::Token::NamespaceDeclaration namespaceDeclaration = linkedUnit->Namespaces[i];
			
			// Write namespace header
			for (int ch = 0; ch < namespaceDeclaration.Name.size(); ch++)
			{
				buffer->push_back(namespaceDeclaration.Name[ch]);
			}
			buffer->push_back('\0');

			// Write namespace keys
			for (int keyIndex = 0; keyIndex < namespaceDeclaration.InnerTokens.size(); keyIndex++)
			{
				WriteKey(buffer, namespaceDeclaration.InnerTokens[keyIndex]);
			}

		}

	}

	inline void WriteTCBHeader(std::vector<char>* buffer)
	{
		// Magic Number 
		buffer->push_back('T');
		buffer->push_back('C');
		buffer->push_back('B');

		// Version
		buffer->push_back(1); // Major Revision
		buffer->push_back(0); // Minor Revision
	}
}