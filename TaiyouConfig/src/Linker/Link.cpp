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
				for (int iAllNamespaces = 0; iAllNamespaces < allNamespaces.size(); iAllNamespaces++)
				{
					// Merge namespaces of the same name
					if (allNamespaces[iAllNamespaces].Name == declaration.Name)
					{
						// Merge the two namespaces togheder
						for (int key = 0; key < declaration.InnerTokens.size(); key++)
						{
							UnparsedKey currentKey = declaration.InnerTokens[key];
							bool isKeyDuplicate = false;


							// Check if key already exists in allNamespaces
							for (int innerKeyIndex = 0; innerKeyIndex < allNamespaces[iAllNamespaces].InnerTokens.size(); innerKeyIndex++)
							{
								UnparsedKey key = allNamespaces[iAllNamespaces].InnerTokens[innerKeyIndex];

								// Duplicate key found
								if (key.Type == currentKey.Type && key.Name == currentKey.Name)
								{
									isKeyDuplicate = true;
									break;
								}
							}

							if (!isKeyDuplicate)
							{
								allNamespaces[iAllNamespaces].InnerTokens.push_back(currentKey);
							}
							else
							{
								// TODO: Throw duplicate key found exception
								std::cout << "Linker; WARNING: Duplicate key found, " << ToString(currentKey) << std::endl;
							}

						}

						namespacesMerged = true;
						break;
					}
				}

				// Only adds the current namespace if not merged
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