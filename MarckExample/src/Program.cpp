#include <iostream>
#include "KeysStore.hpp"
#include "Utils.hpp"
#include "Linker/UncompiledNamespace.hpp"
#include "Linker/Link.hpp"

using namespace TaiyouConfig;

int main(int agrc, const char* argv[])
{
	std::string mainSource = FetchTCFG("./mainconfig.tcfg");
	TcfgUnit mainUnit = TokenizeTcfg(mainSource);

	std::string ceiraSource = FetchTCFG("./ceira.tcfg");
	TcfgUnit ceiraUnit = TokenizeTcfg(ceiraSource);


	std::vector<TcfgUnit> units{ mainUnit, ceiraUnit };

	TaiyouConfig::Linker::LinkedTcfgUnit LinkedUnits = Linker::LinkUnits(units);


	std::cout << "Ready" << std::endl;
	std::cin.get();
	return 0;
}