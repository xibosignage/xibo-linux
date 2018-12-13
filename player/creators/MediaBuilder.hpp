#pragma once

#include "utils/Resources.hpp"
#include "utils/FileSystemAdaptor.hpp"
#include "utils/FilePath.hpp"

#include <boost/optional/optional.hpp>

template<typename Builder>
class BaseMediaBuilder
{
public:
    BaseMediaBuilder()
    {
        m_filesystem = std::make_unique<FileSystemAdaptor>();
    }

    Builder& id(int id)
    {
        m_id = id;
        return static_cast<Builder&>(*this);
    }

    virtual Builder& path(const boost::optional<std::string>& path)
    {
        if(path)
        {
            auto fullPath = Resources::directory() / path.value();

            checkPath(fullPath);

            m_path = fullPath;
        }
        return static_cast<Builder&>(*this);
    }

    virtual Builder& duration(int duration)
    {
        m_duration = duration;
        return static_cast<Builder&>(*this);
    }

protected:
    virtual IFileSystemAdaptor& filesystem()
    {
        return *m_filesystem;
    }

    template<typename Media>
    void prepareCommonParams(Media& media)
    {
        media.setDuration(m_duration);
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
