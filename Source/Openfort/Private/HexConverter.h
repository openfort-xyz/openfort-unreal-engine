#pragma once
#include <string>
#include <vector>

class HexConverter
{
public:
	static std::string ToHexString(unsigned char data[], int len);
	static std::vector<unsigned char> FromHexString(std::string);
};