#pragma once
#include <string>
#include <vector>
#include "UnparsedKey.hpp"

namespace TaiyouConfig::Token
{
	class BlockDeclaration
	{
	public:
		std::string Type;
		std::string Value;
		std::vector<UnparsedKey> InnerTokens;

		BlockDeclaration(const char type[], const char value[]);
	};
}


