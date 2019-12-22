#include "RsaManager.hpp"

#include "common/fs/FileSystem.hpp"
#include "common/logger/Logging.hpp"
#include "config/AppConfig.hpp"

const unsigned int RsaKeyLength = 1024;

RsaManager& RsaManager::instance()
{
    static RsaManager rsa;
    return rsa;
}

void RsaManager::load()
{
    auto publicKeyPath = AppConfig::publicKeyPath();
    auto privateKeyPath = AppConfig::privateKeyPath();

    if (!FileSystem::exists(publicKeyPath) || !FileSystem::exists(privateKeyPath))
    {
        keys_ = CryptoUtils::generateRsaKeys(RsaKeyLength);
        CryptoUtils::saveRsaKeys(keys_, publicKeyPath, privateKeyPath);
    }
    else
    {
        keys_ = CryptoUtils::loadRsaKeys(publicKeyPath, privateKeyPath);
    }
}

CryptoPP::RSA::PublicKey RsaManager::publicKey() const
{
    return keys_.publicKey;
}

CryptoPP::RSA::PrivateKey RsaManager::privateKey() const
{
    return keys_.privateKey;
}
