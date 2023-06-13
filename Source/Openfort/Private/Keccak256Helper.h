#pragma once
#include <string>

class Keccak256Helper
{
public:
	static std::string ChecksumEncode(std::string addr);
	static void HashMessage(std::string messageHex, unsigned char hash[32]);
};

