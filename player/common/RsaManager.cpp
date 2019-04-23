#include "RsaManager.hpp"

#include "config.hpp"
#include "FileSystem.hpp"
#include "common/logger/Logging.hpp"

const unsigned int RSA_KEY_LENGTH = 1024;

RsaManager& RsaManager::instance()
{
    static RsaManager rsa;
    return rsa;
}

void RsaManager::load()
{
    auto publicKeyPath = ProjectResources::publicKeyFile();
    auto privateKeyPath = ProjectResources::privateKeyFile();

    if(!FileSystem::exists(publicKeyPath) || !FileSystem::exists(privateKeyPath))
    {
        m_keys = CryptoUtils::generateRsaKeys(RSA_KEY_LENGTH);
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
