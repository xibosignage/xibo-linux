#include "RsaManager.hpp"

#include "config.hpp"
#include "FileSystem.hpp"
#include "common/logger/Logging.hpp"



#include "pem.h"

RsaManager& RsaManager::instance()
{
    static RsaManager rsa;
    return rsa;
}

void RsaManager::load()
{
    auto publicKeyPath = ProjectResources::configDirectory() / "id_rsa.pub";
    auto privateKeyPath = ProjectResources::configDirectory() / "id_rsa";

    if(!FileSystem::exists(publicKeyPath) || !FileSystem::exists(privateKeyPath))
    {
        auto[pub, priv] = RsaGenerator::generate();
        m_publicKey = pub;
        m_privateKey = priv;
    }
    else
    {
        CryptoPP::FileSource fs1(publicKeyPath.c_str(), true);
        CryptoPP::FileSource fs2(privateKeyPath.c_str(), true);

        PEM_Load(fs1, m_publicKey);
        PEM_Load(fs2, m_privateKey);
    }
}

std::string RsaManager::publicKeyStr() const
{
    std::string publicKeyStr;
    CryptoPP::StringSink s1{publicKeyStr};

    PEM_Save(s1, m_publicKey);

    return publicKeyStr;
}

std::string RsaManager::privateKeyStr() const
{
    std::string privateKeyStr;
    CryptoPP::StringSink s1{privateKeyStr};

    PEM_Save(s1, m_privateKey);

    return privateKeyStr;
}

CryptoPP::RSA::PublicKey RsaManager::publicKey() const
{
    return m_publicKey;
}

CryptoPP::RSA::PrivateKey RsaManager::privateKey() const
{
    return m_privateKey;
}
