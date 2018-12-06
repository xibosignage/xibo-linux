#pragma once

#include "utils/FilePath.hpp"
#include "utils/IFileSystemAdaptor.hpp" // FIXME change to forward declaration

#include <boost/optional/optional_fwd.hpp>

class IMedia;

class MediaBuilder
{
public:
    MediaBuilder();
    virtual ~MediaBuilder() = default;

    std::unique_ptr<IMedia> build();

    MediaBuilder& id(int id);
    virtual MediaBuilder& path(const boost::optional<std::string>& path);
    virtual MediaBuilder& duration(int duration);

protected:
    virtual std::unique_ptr<IMedia> doBuild() = 0;
    virtual IFileSystemAdaptor& filesystem();

private:
    void checkPath(FilePath path);

protected:
    std::unique_ptr<IFileSystemAdaptor> m_filesystem;
    FilePath m_path;
    int m_id;
    int m_duration;


};
