#include "Keccak256Helper.h"
#include "HexConverter.h"
#include "Openfort/External/Keccak256.h"
#include <algorithm>

std::string Keccak256Helper::ChecksumEncode(std::string addr)
{
	// address must be lowercase or hashedAddress will be incorrect
	std::transform(addr.begin(), addr.end(), addr.begin(),
		[](unsigned char c) { return std::tolower(c); });

	const auto hexAddrCStr = (unsigned char*)addr.c_str();
	uint8_t hashedAddress[32] = { 0 };
	Keccak256::getHash(&hexAddrCStr[0], 40, hashedAddress);
	const std::string hashedPubKeyHexString = HexConverter::ToHexString(hashedAddress, 32);

	std::string checksummedPubKeyHexString;
	for (auto nibbleIndex = 0; nibbleIndex < addr.length(); ++nibbleIndex)
	{
		if (std::isdigit(addr[nibbleIndex]))
		{
			checksummedPubKeyHexString.push_back(addr[nibbleIndex]);
		}
		else
		{
			auto nibble = hashedPubKeyHexString[nibbleIndex];
			const char div = '7';
			if (nibble > div)
			{
				checksummedPubKeyHexString.push_back(std::toupper(addr[nibbleIndex]));
			}
			else
			{
				checksummedPubKeyHexString.push_back(std::tolower(addr[nibbleIndex]));
			}
		}
	}
	return checksummedPubKeyHexString;
}

void Keccak256Helper::HashMessage(std::string messageHex, unsigned char hash[32])
{
	std::vector<unsigned char> messageBytesVector = HexConverter::FromHexString(messageHex);
	std::string msgSizeStr = std::to_string(messageBytesVector.size());

	std::string msgFirtChar = "\x19";
	std::string msgPrefix = msgFirtChar + "Ethereum Signed Message:\n" + std::to_string(messageBytesVector.size());
	int msgPrefixLength = msgPrefix.length();
	const char* msgPrefixBytes = msgPrefix.c_str();

	const int msgSize = msgPrefixLength + messageBytesVector.size();
	uint8_t* bytes = new uint8_t[msgSize];

	std::copy(msgPrefixBytes, msgPrefixBytes + msgPrefixLength, bytes);
	std::copy(messageBytesVector.begin(), messageBytesVector.end(), bytes + msgPrefixLength);

	Keccak256::getHash(bytes, msgSize, hash);
}
