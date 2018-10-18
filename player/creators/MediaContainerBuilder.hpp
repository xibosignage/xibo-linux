#pragma once

#include <memory>
#include <vector>
#include <boost/optional/optional.hpp>

#include "control/IMediaContainer.hpp"
#include "media/IMedia.hpp"
#include "adaptors/IFixedLayoutAdaptor.hpp"

struct MediaWithPos
{
    std::unique_ptr<IMedia> media;
    int x;
    int y;
};

const int DEFAULT_ZORDER = 0;
const bool DEFAULT_LOOP = false;

class MediaContainerBuilder
{
public:
    std::unique_ptr<IMediaContainer> build();

    MediaContainerBuilder& adaptor(std::unique_ptr<IFixedLayoutAdaptor>&& adaptor);
    MediaContainerBuilder& width(int width);
    MediaContainerBuilder& height(int height);
    MediaContainerBuilder& zorder(const boost::optional<int>& zorder);
    MediaContainerBuilder& loop(const boost::optional<bool>& loop);
    MediaContainerBuilder& visibleMedia(std::vector<MediaWithPos>&& visibleMedia);
    MediaContainerBuilder& invisibleMedia(std::vector<std::unique_ptr<IMedia>>&& visibleMedia);

protected:
    virtual std::unique_ptr<IMediaContainer> createContainer(int zorder, bool loop);

private:
    void addAllMedia(IMediaContainer& container);
    void checkWidth(int width);
    void checkHeight(int height);

private:
    std::unique_ptr<IFixedLayoutAdaptor> m_adaptor;
    int m_width;
    int m_height;
    int m_zorder;
    bool m_loop;
    std::vector<MediaWithPos> m_visibleMedia;
    std::vector<std::unique_ptr<IMedia>> m_invisibleMedia;

};
