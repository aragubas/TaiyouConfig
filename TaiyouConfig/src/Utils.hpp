#pragma once
#include <string>
#include <vector>
#include "TcfgUnit.hpp"

#ifdef _DEBUG
#include <iostream>
#endif

namespace TaiyouConfig
{
	
	// Validates a tcfg file and read contents
	std::string FetchTCFG(const char path[]);

	// Compiles a TCFG file from source
	TcfgUnit TokenizeTcfg(std::string source);

	std::string ToString(Token::UnparsedKey unparsedKey);
}