#pragma once

#include "AbstractBuilder.hpp"

#include "utils/Resources.hpp"
#include "utils/FileSystemAdaptor.hpp"
#include "parsers/MediaOptions.hpp"

#include <boost/optional/optional.hpp>

template<typename Builder>
class AbstractMediaBuilder : public AbstractBuilder<Builder>
{
public:
    using Options = typename AbstractBuilder<Builder>::Options;
    using Component = typename AbstractBuilder<Builder>::Component;
    using Handler = typename AbstractBuilder<Builder>::Handler;

    AbstractMediaBuilder()
    {
        m_filesystem = std::make_unique<FileSystemAdaptor>();
    }

    Builder& filesystem(std::unique_ptr<IFileSystemAdaptor>&& filesystem)
    {
        m_filesystem = std::move(filesystem);
        return static_cast<Builder&>(*this);
    }

protected:
    virtual Builder& retrieveMediaOptions(const typename BuilderTraits<Builder>::Options& opts) = 0;
    virtual void doMediaSetup(typename BuilderTraits<Builder>::Component&) { }

    IFileSystemAdaptor& filesystem()
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
    void doSetup(typename BuilderTraits<Builder>::Component& media) final
    {
        media.setDuration(m_duration);
        doMediaSetup(media);
    }

    Builder& retrieveOptions(const typename BuilderTraits<Builder>::Options& opts) final
    {
        parseBaseOptions(opts);
        return retrieveMediaOptions(opts);
    }

    void parseBaseOptions(const MediaOptions& opts)
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
    std::unique_ptr<Handler> m_adaptor;
    std::unique_ptr<IFileSystemAdaptor> m_filesystem;
    FilePath m_path;
    int m_id;
    int m_duration;

};
