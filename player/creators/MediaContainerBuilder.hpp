#pragma once

#include <memory>
#include <vector>
#include <boost/optional/optional_fwd.hpp>

class IMediaContainer;
class IMedia;

struct MediaStruct
{
    std::unique_ptr<IMedia> media;
    std::string type;
    int x;
    int y;
};

class MediaContainerBuilder
{
public:
    template<typename MediaContainerFactory>
    std::unique_ptr<IMediaContainer> build()
    {
        auto container = MediaContainerFactory().create(m_zorder);

        prepareContainer(*container);

        return container;
    }

    MediaContainerBuilder& setWidth(int width);
    MediaContainerBuilder& setHeight(int height);
    MediaContainerBuilder& setZorder(const boost::optional<int>& zorder);
    MediaContainerBuilder& setLoop(const boost::optional<bool>& loop);
    MediaContainerBuilder& setMedia(std::vector<MediaStruct>&& media);

private:
    void prepareContainer(IMediaContainer& container);

private:
    int m_width;
    int m_height;
    int m_zorder;
    bool m_loop;
    std::vector<MediaStruct> m_media;

};
