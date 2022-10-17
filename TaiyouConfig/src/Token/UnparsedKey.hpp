#pragma once
#include <string>

namespace TaiyouConfig::Token
{
	class UnparsedKey
	{
	public:
		std::string Name;
		std::string Type;
		std::string Value;

		UnparsedKey(const char type[], const char name[], const char value[]);
		UnparsedKey();
	};
}

