#pragma once

#include "RsaGenerator.hpp"

#include <cryptopp/queue.h>
#include <cryptopp/files.h>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>

class RsaManager
{
public:
    static RsaManager& instance();

    void load();

    std::string publicKeyStr() const;
    std::string privateKeyStr() const;

    CryptoPP::RSA::PublicKey publicKey() const;
    CryptoPP::RSA::PrivateKey privateKey() const;

private:
    RsaManager() = default;

private:
    CryptoPP::RSA::PublicKey m_publicKey;
    CryptoPP::RSA::PrivateKey m_privateKey;

};
