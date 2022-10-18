#pragma once
#include <vector>
#include "../Token/UnparsedKey.hpp"
#include "../Token/NamespaceDeclaration.hpp"
 
namespace TaiyouConfig::Linker
{
	struct LinkedTcfgUnit
	{
		std::vector<Token::NamespaceDeclaration> Namespaces;
		std::vector<Token::UnparsedKey> GlobalKeys;
	};
}