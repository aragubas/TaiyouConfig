#pragma once
#include <string>
#include <vector>
#include "UnparsedKey.hpp"

namespace TaiyouConfig::Token
{
	struct NamespaceDeclaration
	{
		std::string Name;
		std::vector<UnparsedKey> InnerTokens;
	};
}


