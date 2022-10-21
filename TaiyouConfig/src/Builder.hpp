#pragma once
#include "Linker/LinkedTcfgUnit.hpp"

namespace TaiyouConfig::Builder
{
	constexpr char KEYWORD_GLOBALNAMESPACE = 0x69;
	constexpr char KEYWORD_DECLARE_NAMESPACE = 0x16;
	constexpr char KEYWORD_DECLARE_KEY = 0x42;
	constexpr char KEYWORD_STATEMENT_END = 0x25;

	int Build(TaiyouConfig::Linker::LinkedTcfgUnit linkedUnit[], const char* outputFileName);

	void WriteTCBHeader(std::vector<char>& buffer);

	void WriteGlobalNamespaceKeys(std::vector<char>& buffer, TaiyouConfig::Linker::LinkedTcfgUnit linkedUnit[]);

	void WriteNamespaces(std::vector<char>& buffer, TaiyouConfig::Linker::LinkedTcfgUnit linkedUnit[]);

	inline void WriteKey(std::vector<char>& buffer, TaiyouConfig::Token::UnparsedKey key);
}