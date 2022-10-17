#include "NamespaceDeclaration.hpp"

namespace TaiyouConfig::Token
{
	NamespaceDeclaration::NamespaceDeclaration(const char type[], const char value[])
	{
		Type = type;
		Value = value;
	}
}