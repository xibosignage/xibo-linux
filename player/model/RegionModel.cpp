#include "RegionModel.hpp"

RegionModel::RegionModel(const RegionOptions& options)
{
    m_id = options.id;
    m_looped = static_cast<Loop>(options.loop);
    m_width = options.width;
    m_height = options.height;
    m_left = options.left;
    m_top = options.top;
    m_zindex = options.zindex;
}

int RegionModel::id() const
{
    return m_id;
}

const AllMedia& RegionModel::media() const
{
    return m_media;
}

void RegionModel::addMedia(const std::shared_ptr<MediaModel>& media)
{
    m_media.emplace_back(media);
}

RegionModel::Loop RegionModel::looped() const
{
    return m_looped;
}

int RegionModel::width() const
{
    return m_width;
}

int RegionModel::height() const
{
    return m_height;
}

int RegionModel::left() const
{
    return m_left;
}

int RegionModel::top() const
{
    return m_top;
}

int RegionModel::zindex() const
{
    return m_zindex;
}
