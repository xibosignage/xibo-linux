#pragma once

#include "CryptoUtils.hpp"

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
    RsaKeyPair m_keys;
};
