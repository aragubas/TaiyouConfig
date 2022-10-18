#include "Link.hpp"
#include <vector>
#include <map>
#include <iostream>
#include "../Utils.hpp"

namespace TaiyouConfig::Linker
{
	LinkedTcfgUnit LinkUnits(std::vector<TcfgUnit> &units)
	{
		std::vector<Token::NamespaceDeclaration> allNamespaces;
		//std::vector<Token::UnparsedKey> allGlobalyDefinedKeys;
		// Key: <type>:<name>
		typedef std::map<std::string, Token::UnparsedKey> globalKeysMapType;
		std::map<std::string, Token::UnparsedKey> allGlobalyDefinedKeys;

		for (int i = 0; i < units.size(); i++)
		{
			// Run though every namespace inside the unit and merges namespaces if necessary
			for (int i2 = 0; i2 < units[i].Namespaces.size(); i2++)
			{
				Token::NamespaceDeclaration declaration = units[i].Namespaces[i2];

				// Check if allNamespaces already have a namespace with the same name
				bool namespacesMerged = false;
				for (int ceira = 0; ceira < allNamespaces.size(); ceira++)
				{
					// Merge namespaces of the same name
					if (allNamespaces[ceira].Value == declaration.Value)
					{
						// Merge the two namespaces togheder
						for (int key = 0; key < declaration.InnerTokens.size(); key++)
						{
							Token::UnparsedKey currentKey = declaration.InnerTokens[key];

							allNamespaces[ceira].InnerTokens.push_back(currentKey);

#ifdef _DEBUG
							std::cout << "Link-Merge; " << allNamespaces[ceira].Value << ": " << ToString(currentKey) << std::endl;
#endif
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

			// Run though every key declared in GlobalNamespace
			for (int i2 = 0; i2 < units[i].GlobalNamespace.size(); i2++)
			{
				Token::UnparsedKey key = units[i].GlobalNamespace[i2];
				std::string keyName = key.Type + ":" + key.Name;

				// Key not found, add to the list
				if (allGlobalyDefinedKeys.find(keyName) == allGlobalyDefinedKeys.end())
				{
					allGlobalyDefinedKeys[keyName] = key;
#ifdef _DEBUG
					std::cout << "Link-Global; GlobalKey: " << ToString(key) << std::endl;
#endif
				}
				else
				{
					// Duplicate key found
#ifdef _DEBUG
					std::cout << "Link-Global; Duplicate key found: " << ToString(key) << std::endl;
#endif
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



		return returnUnit;
	}
}