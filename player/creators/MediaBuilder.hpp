#pragma once

#include "utils/Resources.hpp"
#include "utils/FileSystemAdaptor.hpp"
#include "parsers/MediaOptions.hpp"

#include <boost/optional/optional.hpp>

#include "utils/Logger.hpp"

template<typename Builder, typename MediaOptions>
class BaseMediaBuilder
{
public:
    BaseMediaBuilder()
    {
        m_filesystem = std::make_unique<FileSystemAdaptor>();
    }

    Builder& options(const MediaOptions& opts)
    {
        parseBaseOptions(opts);
        return mediaOptions(opts);
    }

protected:
    virtual IFileSystemAdaptor& filesystem()
    {
        return *m_filesystem;
    }

    virtual Builder& mediaOptions(const MediaOptions& opts) = 0;

    void parseBaseOptions(const ResourcesXlf::MediaOptions& opts)
    {
        m_id = id(opts.id());
        m_path = getPathOption(opts.path());
        m_duration = getDurationOption(opts.duration());
    }

    template<typename Media>
    void prepareCommonParams(Media& media)
    {
        media.setDuration(m_duration);
    }

    virtual int id(int id)
    {
        return id;
    }

    virtual FilePath getPathOption(const boost::optional<std::string>& pathOpt)
    {
        if(pathOpt)
        {
            auto fullPath = Resources::directory() / pathOpt.value();

            checkPath(fullPath);

            return fullPath;
        }
        return {};
    }

    virtual int getDurationOption(int duration)
    {
        return duration;
    }

private:
    void checkPath(FilePath path)
    {
        if(!filesystem().isRegularFile(path))
            throw std::runtime_error("Not valid path");
    }

protected:
    std::unique_ptr<IFileSystemAdaptor> m_filesystem;
    FilePath m_path;
    int m_id;
    int m_duration;

};
