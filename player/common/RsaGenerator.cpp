#include "RsaGenerator.hpp"
#include "config.hpp"
#include "common/logger/Logging.hpp"

#include "pem.h"

const int RSA_KEY_LENGTH = 1024;

RsaKeyPair RsaGenerator::generate()
{
    auto publicKeyPath = ProjectResources::configDirectory() / "id_rsa.pub";
    auto privateKeyPath = ProjectResources::configDirectory() / "id_rsa";

    CryptoPP::AutoSeededRandomPool rng;

    CryptoPP::InvertibleRSAFunction params;
    params.GenerateRandomWithKeySize(rng, RSA_KEY_LENGTH);

    CryptoPP::RSA::PublicKey publicKey(params);
    CryptoPP::RSA::PrivateKey privateKey(params);

    CryptoPP::FileSink fs1(publicKeyPath.c_str());
    CryptoPP::FileSink fs2(privateKeyPath.c_str());

    PEM_Save(fs1, publicKey);
    PEM_Save(fs2, privateKey);

    return std::make_pair(publicKey, privateKey);
}
