#pragma once

#include "common/crypto/CryptoUtils.hpp"

class RsaManager
{
public:
    static RsaManager& instance();

    void load();

    CryptoPP::RSA::PublicKey publicKey() const;
    CryptoPP::RSA::PrivateKey privateKey() const;

private:
    RsaManager() = default;

private:
    RsaKeyPair keys_;
};
