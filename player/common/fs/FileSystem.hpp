#pragma once

#include <memory>
#include <set>

struct StorageUsageInfo;
class FilePath;

class FileSystem
{
public:
    static bool isRegularFile(const FilePath& path);
    static bool isDirectory(const FilePath& path);
    static void move(const FilePath& source, const FilePath& dest);
    static void copy(const FilePath& source, const FilePath& dest);
    static bool exists(const FilePath& path);
    static bool empty(const FilePath& path);
    static bool remove(const FilePath& path);
    static bool removeAll(const FilePath& path);
    static bool createDirectory(const FilePath& path);
    static FilePath currentPath();
    static std::string readFromFile(const FilePath& path);
    static void writeToFile(const FilePath& path, const std::string& content);
    static StorageUsageInfo storageUsageFor(const FilePath& path);
};
