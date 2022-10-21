#include "Link.hpp"
#include "../Utils.hpp"
#include <vector>
#include <map>
#include <iostream>

extern bool VerboseOutput;

using namespace TaiyouConfig::Token;

namespace TaiyouConfig::Linker
{
	LinkedTcfgUnit LinkUnits(std::vector<TcfgUnit> &units)
	{
		std::vector<NamespaceDeclaration> allNamespaces;
		//std::vector<UnparsedKey> allGlobalyDefinedKeys;
		// Key: <type>:<name>
		typedef std::map<std::string, UnparsedKey> globalKeysMapType;
		std::map<std::string, UnparsedKey> allGlobalyDefinedKeys;

		for (int i = 0; i < units.size(); i++)
		{
			// Run though every namespace inside the unit and merges namespaces if necessary
			for (int i2 = 0; i2 < units[i].Namespaces.size(); i2++)
			{
				NamespaceDeclaration declaration = units[i].Namespaces[i2];

				// Check if allNamespaces already have a namespace with the same name
				bool namespacesMerged = false;
				for (int ceira = 0; ceira < allNamespaces.size(); ceira++)
				{
					// Merge namespaces of the same name
					if (allNamespaces[ceira].Name == declaration.Name)
					{
						// Merge the two namespaces togheder
						for (int key = 0; key < declaration.InnerTokens.size(); key++)
						{
							UnparsedKey currentKey = declaration.InnerTokens[key];

							allNamespaces[ceira].InnerTokens.push_back(currentKey);
						}

						namespacesMerged = true;
						break;
					}
				}

				// Only adds the current namespace if merged
				if (!namespacesMerged)
				{
					allNamespaces.push_back(declaration);
				}
			}

			// Run through every key declared in GlobalNamespace
			for (int i2 = 0; i2 < units[i].GlobalNamespace.size(); i2++)
			{
				UnparsedKey key = units[i].GlobalNamespace[i2];
				std::string keyName = key.Type + ":" + key.Name;

				// Key not found, add to the list
				if (allGlobalyDefinedKeys.find(keyName) == allGlobalyDefinedKeys.end())
				{
					allGlobalyDefinedKeys[keyName] = key;				
				}
				else
				{
					// Duplicate key found
					// TODO; Linker error: Duplicate key found
				}

			}

		}

		LinkedTcfgUnit returnUnit;
		returnUnit.Namespaces = allNamespaces;
		// Builds the list of global keys
		for (globalKeysMapType::const_iterator it = allGlobalyDefinedKeys.begin(); it != allGlobalyDefinedKeys.end(); ++it)
		{
			returnUnit.GlobalKeys.push_back(it->second);
		}

		if (VerboseOutput)
		{
			std::cout << "Linker; Linker Finished" << std::endl << "GLOBAL keys:" << std::endl;

			for (int i = 0; i < returnUnit.GlobalKeys.size(); i++)
			{
				std::cout << "  " << ToString(returnUnit.GlobalKeys[i]) << std::endl;
			}

			std::cout << std::endl;

			for (int i = 0; i < returnUnit.Namespaces.size(); i++)
			{
				std::cout << returnUnit.Namespaces[i].Name << " keys:" << std::endl;

				for (int i2 = 0; i2 < returnUnit.Namespaces[i].InnerTokens.size(); i2++)
				{
					std::cout << "  " << ToString(returnUnit.Namespaces[i].InnerTokens[i2]) << std::endl;
				}

			}
			std::cout << std::endl;
		}


		return returnUnit;
	}
}