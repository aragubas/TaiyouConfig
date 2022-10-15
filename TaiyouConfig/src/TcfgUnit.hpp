#pragma once
#include <vector>
#include "Token/UnparsedKey.hpp"
#include "Token/BlockDeclaration.hpp"

namespace TaiyouConfig
{
	class TcfgUnit
	{
	public:
		std::vector<TaiyouConfig::Token::UnparsedKey> GlobalNamespace;
		std::vector<TaiyouConfig::Token::UnparsedKey> BlockBuffer;
		std::vector<TaiyouConfig::Token::BlockDeclaration> Blocks;

		TcfgUnit();
	};
}
