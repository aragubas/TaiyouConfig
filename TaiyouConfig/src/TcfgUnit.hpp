#pragma once
#include <vector>
#include "Token/UnparsedKey.hpp"
#include "Token/NamespaceDeclaration.hpp"

namespace TaiyouConfig
{
	struct TcfgUnit
	{
		std::vector<Token::NamespaceDeclaration> Namespaces;
		std::vector<Token::UnparsedKey> GlobalNamespace;
	};
}
