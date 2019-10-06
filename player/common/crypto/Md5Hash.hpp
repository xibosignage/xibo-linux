#pragma once

#include "common/types/internal/StrongType.hpp"

#include <string>

class FilePath;

struct Md5Hash : StrongType<std::string>
{
    using StrongType::StrongType;

    static Md5Hash fromString(std::string_view data);
    static Md5Hash fromFile(const FilePath& path);
};

bool operator==(const Md5Hash& first, const Md5Hash& second);
bool operator!=(const Md5Hash& first, const Md5Hash& second);
std::ostream& operator<<(std::ostream& out, const Md5Hash& hash);
