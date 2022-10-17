#pragma once
#include "../TcfgUnit.hpp"

namespace TaiyouConfig::Linker
{
	struct UncompiledNamespace
	{
		std::string Name;
		std::vector<TaiyouConfig::TcfgUnit> Units;
	};
}