#pragma once
#include <map>
#include <string>

namespace TaiyouConfig
{
	class KeysStore
	{
		std::map<std::string, std::string> Database;

	public:
		KeysStore();
		
		void LoadDatabase(const char* fileName);


	};
}

