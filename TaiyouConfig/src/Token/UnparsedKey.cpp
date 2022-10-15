#include "UnparsedKey.hpp"

namespace TaiyouConfig::Token
{
	UnparsedKey::UnparsedKey(const char type[], const char name[], const char value[])
	{
		Type = type;
		Name = name;
		Value = value;
	}
}