#pragma once

#include "constants.hpp"
#include "utils/IFileSystemAdaptor.hpp"

#include <boost/optional/optional.hpp>

class IMedia;

class MediaBuilder
{
public:
    MediaBuilder();
    virtual ~MediaBuilder() = default;

    std::unique_ptr<IMedia> build();

    MediaBuilder& id(int id);
    MediaBuilder& audio(std::unique_ptr<IMedia>&& audio);

    virtual MediaBuilder& path(const boost::optional<std::string>& path);
    virtual MediaBuilder& duration(int duration);

protected:
    virtual std::unique_ptr<IMedia> doBuild() = 0;
    virtual IFileSystemAdaptor& filesystem();

private:
    void checkPath(FilePath path);

protected:
    std::unique_ptr<IFileSystemAdaptor> m_filesystem;
    int m_id;
    std::unique_ptr<IMedia> m_audio;
    FilePath m_path;
    int m_duration;


};
