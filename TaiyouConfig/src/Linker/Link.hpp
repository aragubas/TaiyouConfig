#pragma once
#include <vector>
#include "UncompiledNamespace.hpp"

namespace TaiyouConfig::Linker
{
	std::vector<UncompiledNamespace> LinkUnits(std::vector<TcfgUnit> &units);
}