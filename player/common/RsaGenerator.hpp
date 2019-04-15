#pragma once

#include <utility>
#include <string>


#include <cryptopp/queue.h>
#include <cryptopp/files.h>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>

using RsaKeyPair = std::pair<CryptoPP::RSA::PublicKey, CryptoPP::RSA::PrivateKey>;

class RsaGenerator
{
public:
    static RsaKeyPair generate();

};
