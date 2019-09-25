#include "Utils.hpp"
#include "fs/FilePath.hpp"

#include <boost/beast/core/detail/base64.hpp>
#include <boost/format.hpp>
#include <fstream>
#include <openssl/md5.h>

std::string Utils::md5hash(std::string_view data)
{
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(data.data()), data.size(), result);

    std::stringstream stream;
    for (unsigned char byte : result)
    {
        stream << boost::format("%02x") % static_cast<short>(byte);
    }
    return stream.str();
}

std::string Utils::md5hashFromFile(const FilePath& path)
{
    std::ifstream in{path};
    std::string fileContent{(std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()};

    return md5hash(fileContent.data());
}

std::string Utils::toBase64(const std::string& text)
{
    return boost::beast::detail::base64_encode(text);
}

std::string Utils::fromBase64(const std::string& text)
{
    return boost::beast::detail::base64_decode(text);
}
