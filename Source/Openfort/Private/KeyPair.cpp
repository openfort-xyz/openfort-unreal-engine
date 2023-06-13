#include "KeyPair.h"
#include "HexConverter.h"
#include "Keccak256Helper.h"
#include <algorithm>
#include <iostream>
#include "Openfort/External/Keccak256.h"
#include "Openfort/External/secp256k1_recovery.h"
#include "Openfort/External/secp256k1.h"
#include "Openfort/External/Random.h"

bool KeyPair::Generate()
{
	secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);

	unsigned char randomize[32] = { 0 };
	if (!fill_random(randomize, 32)) {
		return false;
	}

	secp256k1_context_randomize(ctx, randomize);

	do {
		if (!fill_random(seckey, 32)) {
			return false;
		}
	} while (!secp256k1_ec_seckey_verify(ctx, seckey));
	secp256k1_ec_pubkey_create(ctx, &pubkey, seckey);

	secp256k1_context_destroy(ctx);
	return true;
}

bool KeyPair::Initialize(std::string seckeyHex)
{
	secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
	std::vector<unsigned char> seckeyVector = HexConverter::FromHexString(seckeyHex);
	std::copy(seckeyVector.begin(), seckeyVector.end(), seckey);
	if (!secp256k1_ec_seckey_verify(ctx, seckey)) {
		return false;
	}
	secp256k1_ec_pubkey_create(ctx, &pubkey, seckey);

	secp256k1_context_destroy(ctx);
	return true;
}

std::string KeyPair::GetPrivateKeyHex() {
	return "0x" + HexConverter::ToHexString(seckey, 32);
}

std::string KeyPair::GetPublicKeyHex()
{
	secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
	unsigned char pubkeySerialized[65] = { 0 };
	size_t len = sizeof(pubkeySerialized);
	secp256k1_ec_pubkey_serialize(ctx, pubkeySerialized, &len, &pubkey, SECP256K1_EC_UNCOMPRESSED);
	secp256k1_context_destroy(ctx);
	return "0x" + HexConverter::ToHexString(pubkeySerialized, len);
}

std::string KeyPair::Sign(std::string messageHex)
{
	unsigned char hash2[32];
	Keccak256Helper::HashMessage(messageHex, hash2);

	secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
	secp256k1_ecdsa_recoverable_signature sig;
	secp256k1_ecdsa_sign_recoverable(ctx, &sig, hash2, seckey, NULL, NULL);

	unsigned char serialized_signature[65] = { 0 };
	int recoveryId = 0;
	secp256k1_ecdsa_recoverable_signature_serialize_compact(ctx, serialized_signature, &recoveryId, &sig);

	serialized_signature[64] = 27 + recoveryId; 
	secp256k1_context_destroy(ctx);
	return "0x" + HexConverter::ToHexString(serialized_signature, 65);
}

std::string KeyPair::Address()
{
	secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
	unsigned char pubkeySerialized[65] = { 0 };
	size_t len = sizeof(pubkeySerialized);
	secp256k1_ec_pubkey_serialize(ctx, pubkeySerialized, &len, &pubkey, SECP256K1_EC_UNCOMPRESSED);

	uint8_t hashedPubKeyBytes[32] = { 0 };
	Keccak256::getHash(&pubkeySerialized[1], 64, hashedPubKeyBytes);

	const std::string hashedPubKeyString = HexConverter::ToHexString(hashedPubKeyBytes, 32).substr(24);

	secp256k1_context_destroy(ctx);
	return "0x" + hashedPubKeyString;
}


