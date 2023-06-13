#include "HexConverter.h"
#include <unordered_map>

std::string HexConverter::ToHexString(unsigned char data[], int len)
{
	char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	std::string s(len * 2, ' ');
	for (int i = 0; i < len; ++i) {
		s[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
		s[2 * i + 1] = hexmap[data[i] & 0x0F];
	}
	return s;
}

std::vector<unsigned char> HexConverter::FromHexString(std::string hexString)
{
	std::unordered_map<wchar_t, unsigned char> hexCharToByteMap = {
		{'0', 0x00},
		{'1', 0x01},
		{'2', 0x02},
		{'3', 0x03},
		{'4', 0x04},
		{'5', 0x05},
		{'6', 0x06},
		{'7', 0x07},
		{'8', 0x08},
		{'9', 0x09},
		{'a', 0x0A},
		{'b', 0x0B},
		{'c', 0x0C},
		{'d', 0x0D},
		{'e', 0x0E},
		{'f', 0x0F},
		{'A', 0x0A},
		{'B', 0x0B},
		{'C', 0x0C},
		{'D', 0x0D},
		{'E', 0x0E},
		{'F', 0x0F}
	};

	std::vector<unsigned char> byteArray;
	if (hexString.substr(0, 2) == "0x") {
		hexString = hexString.substr(2);
	}

	for (auto iter = hexString.begin(); iter != hexString.end(); ++iter)
	{
		wchar_t char_i = *iter;
		++iter;
		if (!(iter != hexString.end()))
		{
			return byteArray;
		}

		wchar_t char_iPlus1 = *iter;
		//map the character to 0xletter then shift the higher one over and bitwise or them
		auto byte_0 = hexCharToByteMap.at(char_i);
		auto byte_1 = hexCharToByteMap.at(char_iPlus1);
		unsigned char byteToAdd = (byte_0 << 4) | byte_1;
		byteArray.push_back(byteToAdd);
	}
	return byteArray;
}
