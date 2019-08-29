#pragma once

#include <memory>
#include <set>

class FilePath;

class FileSystem
{
public:
    static bool isRegularFile(const FilePath& path);
    static bool isDirectory(const FilePath& path);
    static void copyRecursive(const FilePath& source, const FilePath& dest, const std::set<FilePath>& exceptions = {});
    static void copy(const FilePath& source, const FilePath& dest);
    static bool exists(const FilePath& path);
    static bool empty(const FilePath& path);
    static bool remove(const FilePath& path);
    static bool removeAll(const FilePath& path);
    static bool createDirectory(const FilePath& path);
    static FilePath currentPath();
    static void writeToFile(const FilePath& path, const std::string& content);
};
