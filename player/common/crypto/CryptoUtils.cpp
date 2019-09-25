#include "CryptoUtils.hpp"
#include "fs/FilePath.hpp"

#include <cryptopp/arc4.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>

RsaKeyPair CryptoUtils::generateRsaKeys(unsigned int keyLength)
{
    CryptoPP::AutoSeededRandomPool rng;

    CryptoPP::InvertibleRSAFunction params;
    params.GenerateRandomWithKeySize(rng, keyLength);

    CryptoPP::RSA::PublicKey publicKey{params};
    CryptoPP::RSA::PrivateKey privateKey{params};

    return {publicKey, privateKey};
}

RsaKeyPair CryptoUtils::loadRsaKeys(const FilePath& publicKeyPath, const FilePath& privateKeyPath)
{
    CryptoPP::FileSource fs1{publicKeyPath.c_str(), true};
    CryptoPP::FileSource fs2{privateKeyPath.c_str(), true};

    RsaKeyPair keys;
    CryptoPP::PEM_Load(fs1, keys.publicKey);
    CryptoPP::PEM_Load(fs2, keys.privateKey);

    return keys;
}

void CryptoUtils::saveRsaKeys(const RsaKeyPair& keys, const FilePath& publicKeyPath, const FilePath& privateKeyPath)
{
    CryptoPP::FileSink fs1{publicKeyPath.c_str()};
    CryptoPP::FileSink fs2{privateKeyPath.c_str()};

    CryptoPP::PEM_Save(fs1, keys.publicKey);
    CryptoPP::PEM_Save(fs2, keys.privateKey);
}

std::string CryptoUtils::decryptPrivateKeyPkcs(const std::string& message, const CryptoPP::RSA::PrivateKey& key)
{
    std::string dectypedMessage;
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::RSAES_PKCS1v15_Decryptor decryptor{key};
    CryptoPP::StringSource{message, true,
                           new CryptoPP::PK_DecryptorFilter{rng, decryptor, new CryptoPP::StringSink{dectypedMessage}}};

    return dectypedMessage;
}

std::string CryptoUtils::decryptRc4(const std::string& message, const std::string& key)
{
    auto messageRaw = reinterpret_cast<unsigned char*>(const_cast<char*>(message.c_str()));
    auto keyRaw = reinterpret_cast<unsigned char*>(const_cast<char*>(key.c_str()));

    CryptoPP::ARC4 rc4{keyRaw, key.size()};
    rc4.ProcessString(messageRaw, message.size());

    return message;
}
