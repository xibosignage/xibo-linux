#pragma once

class FilePath;

class FileSystem
{
public:
    static bool isRegularFile(const FilePath& path);
    static bool exists(const FilePath& path);
    static bool createDirectory(const FilePath& path);

};
