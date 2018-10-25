#pragma once

#include "constants.hpp"
#include <boost/optional/optional.hpp>

class IMedia;

class MediaBuilder
{
public:
    virtual ~MediaBuilder() = default;

    std::unique_ptr<IMedia> build();

    MediaBuilder& id(int id);
    virtual MediaBuilder& path(const boost::optional<std::string>& path);
    virtual MediaBuilder& duration(int duration);

protected:
    virtual std::unique_ptr<IMedia> doBuild() = 0;

protected:
    FilePath m_path;
    int m_id;
    int m_duration;

};
