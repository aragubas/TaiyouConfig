#include "Utils.hpp"
#include <filesystem>
#include <fstream>

#define VERBOSE

namespace TaiyouConfig
{
	std::string FetchTCFG(const char path[])
	{
		std::fstream cfgFile;
		std::string entireCfgFile = "";
		cfgFile.open(path, std::ios::in);
		
		if (cfgFile.fail())
		{
			std::cerr << "ERROR; TaiyouConfig: Could not read TCFG File \"" << path << "\"" << std::endl;
			throw std::runtime_error("Could not read TCFG file \"" + std::string(path) + "\"");
		}

		std::string line;
		bool headerValidation = true;
		while (std::getline(cfgFile, line))
		{
			if (headerValidation)
			{
				if (line != "#!TCFG")
				{
					std::cerr << "TaiyouConfig: Invalid TCFG File \"" << path << "\", TCFG Header not found" << std::endl;
					return "";
				}

				headerValidation = false;
				continue;
			}

			entireCfgFile += line + "\n";
		}

		cfgFile.close();

		return entireCfgFile;
	}

	TcfgUnit TokenizeTcfg(std::string source)
	{
		bool commentLineLatch = false;
		bool blockLatch = false;
		bool namespaceBlockLatch = false;
		int blockDepth = 0;
		std::string blockType = "";
		std::string blockValue = "";
		std::string currentNamespace = "GLOBAL";
		int keyDepth = 0;
		std::string keyType = "";
		std::string keyName = "";
		std::string keyValue = "";
		std::vector<TaiyouConfig::Token::UnparsedKey> globalNamespace;
		std::vector<TaiyouConfig::Token::UnparsedKey> blockBuffer;
		std::vector<TaiyouConfig::Token::NamespaceDeclaration> namespaces;

		for (int i = 0; i < source.length(); i++)
		{
			char ch = source[i];

			// Set commentLine Latch
			if (commentLineLatch == false && ch == '#')
			{
				commentLineLatch = true;
				continue;
			}

			// If currently in a comment line latch
			if (commentLineLatch)
			{
				if (ch == '\n')
				{
					commentLineLatch = false;
				}

				continue;
			}

			// Current chracter is '$', not reading a key and blockLatch is false
			if (blockLatch == false && keyDepth == 0 && ch == '$')
			{
				blockLatch = true;
				continue;
			}

			// Currently reading a block
			if (blockLatch)
			{
				// Fetch BlockType
				if (blockDepth == 0)
				{
					if (ch == '\n')
					{
						if (blockType == "end")
						{
#ifdef VERBOSE
							std::cout << ">End Block" << std::endl;
#endif
							if (namespaceBlockLatch)
							{
								namespaces[namespaces.size() - 1].InnerTokens = blockBuffer;
#ifdef VERBOSE
								std::cout << "||Assigned " << blockBuffer.size() << " tokens to block (" << namespaces[namespaces.size() - 1].Type << ", " << namespaces[namespaces.size() - 1].Value << ")" << std::endl << std::endl;
#endif
								// Reset state
								blockBuffer.clear();
								currentNamespace = "GLOBAL";
								blockType = "";
								blockValue = "";
								namespaceBlockLatch = false;
							}

							blockLatch = false;
							continue;
						}
					}

					if (ch == ' ')
					{
						blockDepth++;
						continue;
					}

					if (ch == ' ') { continue; }

					blockType += ch;
				}
				else if (blockDepth == 1) // Fetch BlockName
				{
					if (ch == '\n')
					{
						if (blockType == "namespace")
						{
							currentNamespace = blockValue;
							namespaceBlockLatch = true;
						}

#ifdef VERBOSE
						std::cout << "->Block type(" << blockType << ") value(" << blockValue << ")" << std::endl;
#endif
						namespaces.push_back(TaiyouConfig::Token::NamespaceDeclaration(blockType.c_str(), blockValue.c_str()));

						blockValue = "";
						blockType = "";
						blockDepth = 0;
						blockLatch = false;

						continue;
					}

					if (ch == ' ') { continue; }

					blockValue += ch;
				}

				continue;
			}

			// Fetch KeyType
			if (keyDepth == 0)
			{
				if (ch == ':')
				{
					keyDepth++;
					continue;
				}

				if (ch == ' ' || ch == '\t' || ch == '\n') { continue; }

				keyType += ch;
			}
			else if (keyDepth == 1)
			{
				if (ch == '=')
				{
					keyDepth++;
					continue;
				}

				keyName += ch;
			}
			else if (keyDepth == 2)
			{
				if (ch == '\n')
				{
					keyDepth = 0;

					if (currentNamespace == "GLOBAL")
					{
						globalNamespace.push_back(TaiyouConfig::Token::UnparsedKey(keyType.c_str(), keyName.c_str(), keyValue.c_str()));
#ifdef VERBOSE
						std::cout << "Key: type(" << keyType << ") name(" << keyName << ") value(" << keyValue << "); GLOBAL" << std::endl;
#endif					
					}
					else
					{
						// TODO: Assign token to currrent namespace
						blockBuffer.push_back(TaiyouConfig::Token::UnparsedKey(keyType.c_str(), keyName.c_str(), keyValue.c_str()));
#ifdef VERBOSE
						std::cout << "  Key: type(" << keyType << ") name(" << keyName << ") value(" << keyValue << "); " << currentNamespace << std::endl;
#endif
					}


					keyName = "";
					keyType = "";
					keyValue = "";

					continue;
				}

				// Skip blank characters
				if (ch == ' ') { continue; }

				keyValue += ch;
			}
		}

		TcfgUnit returnUnit;
		returnUnit.Namespaces = namespaces;
		returnUnit.GlobalNamespace = globalNamespace;

#ifdef _DEBUG
		if (blockBuffer.size() != 0)
		{
			std::cout << "WARNING: BlockBuffer is not empty, probably caused by a unclosed block" << std::endl;
		}

		std::cout << std::endl;
		std::cout << "Compilation Finished";
		std::cout << "Found " << globalNamespace.size() << " keys in GLOBAL namespace" << std::endl;
		std::cout << "Found " << namespaces.size() << " block" << std::endl << std::endl;

		std::cout << ">Global Namespace" << std::endl;
		for (int i = 0; i < globalNamespace.size(); i++)
		{
			std::cout << "Key: type(" << globalNamespace[i].Type << ") name(" << globalNamespace[i].Name << ") value(" << globalNamespace[i].Value << ")" << std::endl;
		}

		for (int i = 0; i < namespaces.size(); i++)
		{
			std::cout << std::endl << "->Block type(" << namespaces[i].Type << ") value(" << namespaces[i].Value << ")" << std::endl;

			for (int key = 0; key < namespaces[i].InnerTokens.size(); key++)
			{
				std::cout << "  Key: type(" << namespaces[i].InnerTokens[key].Type << ") name(" << namespaces[i].InnerTokens[key].Name << ") value(" << namespaces[i].InnerTokens[key].Value << ")" << std::endl;
			}
		}
#endif


		return returnUnit;
	}

	std::string ToString(Token::UnparsedKey unparsedKey)
	{
		return unparsedKey.Type + ":" + unparsedKey.Name + "=" + unparsedKey.Value;
	}

}

