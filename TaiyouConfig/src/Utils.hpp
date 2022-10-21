#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "TcfgUnit.hpp"

using namespace TaiyouConfig::Token;

namespace TaiyouConfig
{
	// Validates a tcfg file and read contents
	std::string FetchTCFG(const char path[]);

	// Compiles a TCFG file from source
	TcfgUnit TokenizeTcfg(std::string& source);

	std::string ToString(UnparsedKey& unparsedKey);
}