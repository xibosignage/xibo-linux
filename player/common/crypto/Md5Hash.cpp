#include "Md5Hash.hpp"

#include "common/fs/FileSystem.hpp"

#include <boost/format.hpp>
#include <openssl/md5.h>
#include <sstream>

Md5Hash Md5Hash::fromString(std::string_view data)
{
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(data.data()), data.size(), result);

    std::stringstream stream;
    for (unsigned char byte : result)
    {
        stream << boost::format("%02x") % static_cast<short>(byte);
    }
    return Md5Hash{stream.str()};
}

Md5Hash Md5Hash::fromFile(const FilePath& path)
{
    auto fileContent = FileSystem::readFromFile(path);

    return Md5Hash::fromString(fileContent);
}

bool operator==(const Md5Hash& first, const Md5Hash& second)
{
    return static_cast<std::string>(first) == static_cast<std::string>(second);
}

bool operator!=(const Md5Hash& first, const Md5Hash& second)
{
    return !(first == second);
}

std::ostream& operator<<(std::ostream& out, const Md5Hash& hash)
{
    return out << static_cast<std::string>(hash);
}
