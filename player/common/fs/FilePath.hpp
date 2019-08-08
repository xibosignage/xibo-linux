#pragma once

#include <filesystem>

class FilePath : public std::filesystem::path
{
public:
    using std::filesystem::path::path;

    FilePath(const std::filesystem::path& p) : std::filesystem::path(p) { }
};
