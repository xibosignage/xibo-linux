#pragma once

#include <boost/filesystem/path.hpp>

class FilePath : public boost::filesystem::path
{
public:
    using boost::filesystem::path::path;

    FilePath(const boost::filesystem::path& p) : boost::filesystem::path(p) {}
};
