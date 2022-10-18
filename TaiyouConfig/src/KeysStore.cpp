#include "KeysStore.hpp"
#include <fstream>
#include <iostream>
#include <exception>

namespace TaiyouConfig
{
	KeysStore::KeysStore()
	{

	}

	void KeysStore::LoadDatabase(const char* fileName)
	{
		std::fstream fileStream;
		fileStream.open(fileName, std::ios::in);

		if (fileStream.fail())
		{
			throw std::runtime_error("Could not read database \"" + std::string(fileName) + "\", Could not open file");
		}

		char currentChar;
		bool headerValidated = false;
		int headerMagicNumberIndex = -1;
		char headerMagicNumber[3] = { 0x00, 0x00, 0x00 };
		while (fileStream.get(currentChar))
		{
			if (!headerValidated)
			{
				headerMagicNumberIndex++;

				// Fetch magic number
				if (headerMagicNumberIndex <= 2)
				{
					headerMagicNumber[headerMagicNumberIndex] = currentChar;
					continue;
				}

				if (headerMagicNumberIndex == 3)
				{
					// Validate Header 84, 67, 66
					if (headerMagicNumber[0] == 0x84 && headerMagicNumber[1] == 0x67 &&
						headerMagicNumber[2] == 0x66)
					{
						throw new std::runtime_error("Invalid database file");
					}
					else
					{
						headerValidated = true;
					}
				}

			}
		}

		fileStream.close();
	}

}