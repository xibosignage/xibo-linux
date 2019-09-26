#include "RsaManager.hpp"

#include "common/logger/Logging.hpp"
#include "config.hpp"
#include "fs/FileSystem.hpp"

const unsigned int RsaKeyLength = 1024;

RsaManager& RsaManager::instance()
{
    static RsaManager rsa;
    return rsa;
}

void RsaManager::load()
{
    auto publicKeyPath = ProjectResources::publicKeyPath();
    auto privateKeyPath = ProjectResources::privateKeyPath();

    if (!FileSystem::exists(publicKeyPath) || !FileSystem::exists(privateKeyPath))
    {
        m_keys = CryptoUtils::generateRsaKeys(RsaKeyLength);
        CryptoUtils::saveRsaKeys(m_keys, publicKeyPath, privateKeyPath);
    }
    else
    {
        m_keys = CryptoUtils::loadRsaKeys(publicKeyPath, privateKeyPath);
    }
}

CryptoPP::RSA::PublicKey RsaManager::publicKey() const
{
    return m_keys.publicKey;
}

CryptoPP::RSA::PrivateKey RsaManager::privateKey() const
{
    return m_keys.privateKey;
}
