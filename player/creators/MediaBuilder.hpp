#pragma once

#include "utils/Resources.hpp"
#include "utils/FileSystemAdaptor.hpp"
#include "parsers/MediaOptions.hpp"

#include <boost/optional/optional.hpp>

template<typename Builder>
struct BuilderTraits;

template<typename Builder>
class BaseMediaBuilder
{
public:
    BaseMediaBuilder()
    {
        m_filesystem = std::make_unique<FileSystemAdaptor>();
    }

    std::unique_ptr<typename BuilderTraits<Builder>::Media> build()
    {
        auto media = create();
        media->setDuration(m_duration);
        doSetup(*media);
        return media;
    }

    Builder& options(const typename BuilderTraits<Builder>::Options& opts)
    {
        parseBaseOptions(opts);
        return mediaOptions(opts);
    }

protected:
    virtual Builder& mediaOptions(const typename BuilderTraits<Builder>::Options& opts) = 0;
    virtual std::unique_ptr<typename BuilderTraits<Builder>::Media> create() = 0;
    virtual std::unique_ptr<typename BuilderTraits<Builder>::MediaHandler> createHandler() = 0;

    virtual void doSetup(typename BuilderTraits<Builder>::Media&)
    {
    }

    virtual IFileSystemAdaptor& filesystem()
    {
        return *m_filesystem;
    }

    virtual int getIdOption(int id)
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
    void parseBaseOptions(const ResourcesXlf::MediaOptions& opts)
    {
        m_id = getIdOption(opts.id());
        m_path = getPathOption(opts.path());
        m_duration = getDurationOption(opts.duration());
    }

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
