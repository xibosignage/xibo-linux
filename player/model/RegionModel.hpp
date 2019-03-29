#pragma once

#include "MediaModel.hpp"
#include "parsers/Options.hpp"

#include <vector>
#include <memory>

using AllMedia = std::vector<std::shared_ptr<MediaModel>>;

class RegionModel
{
public:
    enum class Loop
    {
        Disable,
        Enable
    };

    RegionModel(const RegionOptions& options);

    int id() const;
    Loop looped() const;

    const AllMedia& media() const;
    void addMedia(const std::shared_ptr<MediaModel>& media);

    int width() const;
    int height() const;

    int left() const;
    int top() const;
    void setZindex(int zindex);
    int zindex() const;

private:
    int m_id;
    int m_width;
    int m_height;
    int m_left;
    int m_top;
    int m_zindex;
    Loop m_looped;
    AllMedia m_media;

};
