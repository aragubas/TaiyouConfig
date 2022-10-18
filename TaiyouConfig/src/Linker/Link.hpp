#pragma once
#include <vector>
#include "UncompiledNamespace.hpp"
#include "LinkedTcfgUnit.hpp"

namespace TaiyouConfig::Linker
{
	LinkedTcfgUnit LinkUnits(std::vector<TcfgUnit> &units);
}