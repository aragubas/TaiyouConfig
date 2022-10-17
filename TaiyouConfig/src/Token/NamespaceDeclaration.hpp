#pragma once
#include <string>
#include <vector>
#include "UnparsedKey.hpp"

namespace TaiyouConfig::Token
{
	struct NamespaceDeclaration
	{
		std::string Type;
		std::string Value;
		std::vector<UnparsedKey> InnerTokens;

		NamespaceDeclaration(const char type[], const char value[]);
	};
}


