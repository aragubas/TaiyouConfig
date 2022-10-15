#include <iostream>
#include "KeysStore.hpp"
#include "Utils.hpp"

int main(int agrc, const char* argv[])
{
	std::string mainSource = TaiyouConfig::FetchTCFG("./config.tcfg");
	TaiyouConfig::TcfgUnit unit = TaiyouConfig::TokenizeTcfg(mainSource);

	for (int i = 0; i < unit.Blocks.size(); i++)
	{
		std::cout << unit.Blocks[i].Value << std::endl;
	}

	TaiyouConfig::KeysStore keysStore;


	std::cout << "Ready" << std::endl;
	return 0;
}