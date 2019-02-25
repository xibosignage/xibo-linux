#pragma once

#include "AbstractBuilder.hpp"

#include "utils/Resources.hpp"
#include "utils/FileSystemAdaptor.hpp"
#include "utils/Uri.hpp"

#include "options/MediaOptions.hpp"

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
    virtual void retrieveMediaOptions(const Options& opts) = 0;
    virtual void doMediaSetup(Component&) { }

    IFileSystemAdaptor& filesystem()
    {
        return *m_filesystem;
    }

    virtual int getIdOption(int id)
    {
        return id;
    }

    virtual Uri getUriOption(const boost::optional<std::string>& pathOpt)
    {
        if(pathOpt)
        {
            auto fullPath = Resources::directory() / pathOpt.value();

            checkPath(fullPath);

            return Uri{Uri::Scheme::File, fullPath};
        }
        return {};
    }

    virtual int getDurationOption(int duration)
    {
        return duration;
    }

private:
    void doSetup(Component& media) final
    {
        media.setDuration(m_duration);
        doMediaSetup(media);
    }

    void retrieveOptions(const Options& opts) final
    {
        parseBaseOptions(opts);
        retrieveMediaOptions(opts);
    }

    void parseBaseOptions(const MediaOptions& opts)
    {
        m_id = getIdOption(opts.id());
        m_uri = getUriOption(opts.uri());
        m_duration = getDurationOption(opts.duration());
    }

    void checkPath(const FilePath& path)
    {
        if(!filesystem().isRegularFile(path))
            throw std::runtime_error("Not valid path");
    }

protected: // FIXME
    std::unique_ptr<IFileSystemAdaptor> m_filesystem;
    Uri m_uri;
    int m_id;
    int m_duration;

};
