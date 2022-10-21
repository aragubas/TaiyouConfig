#include "Utils.hpp"
#include <filesystem>
#include <fstream>

using namespace TaiyouConfig::Token;

namespace TaiyouConfig
{
	// Validates and loads source file TCFG
	std::string FetchTCFG(const char path[])
	{
		std::fstream cfgFile;
		std::string entireCfgFile = "";
		cfgFile.open(path, std::ios::in);
		
		if (cfgFile.fail())
		{
			// TODO: Throw a custom error
			//std::cerr << "ERROR; TaiyouConfig: Could not read TCFG File \"" << path << "\"" << std::endl;
			throw std::runtime_error("Could not read TCFG file \"" + std::string(path) + "\"");
		}

		std::string line;
		while (std::getline(cfgFile, line))
		{
			entireCfgFile += line + "\n";
		}

		cfgFile.close();

		return entireCfgFile;
	}

	// Tokenizes TCFG source file
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
							if (namespaceBlockLatch)
							{
								namespaces[namespaces.size() - 1].InnerTokens = blockBuffer;
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
							
							namespaces.push_back(NamespaceDeclaration(blockValue.c_str()));
						} else
						{
							// TODO; Throw an error: Invalid block type
						}
						

						// Reset State
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

					UnparsedKey newKey = UnparsedKey(keyType.c_str(), keyName.c_str(), keyValue.c_str());

					if (currentNamespace == "GLOBAL")
					{
						globalNamespace.push_back(newKey);
					}
					else
					{
						// TODO: Assign token to currrent namespace					
						blockBuffer.push_back(newKey);
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

		return returnUnit;
	}

	// Converts UnparsedKey to human-readable string
	std::string ToString(Token::UnparsedKey& unparsedKey)
	{
		return "\"" + unparsedKey.Type + ":" + unparsedKey.Name + "=" + unparsedKey.Value + "\"";
	}

}

