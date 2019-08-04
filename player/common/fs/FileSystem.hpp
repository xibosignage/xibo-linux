#pragma once

#include <memory>

class FilePath;

class FileSystem
{
public:
    static bool isRegularFile(const FilePath& path);
    static bool exists(const FilePath& path);
    static bool remove(const FilePath& path);
    static bool createDirectory(const FilePath& path);
    static FilePath currentPath();
    static void writeToFile(const FilePath& path, const std::string& content);
};
