#pragma once
#include <string>
#include "Openfort/External/secp256k1.h"

class KeyPair
{
private:

	unsigned char seckey[32];
	secp256k1_pubkey pubkey;

public:

	bool Generate();
	bool Initialize(std::string);
	std::string GetPrivateKeyHex();
	std::string GetPublicKeyHex();
	std::string Sign(std::string);
	std::string Address();
};
