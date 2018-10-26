#include "MediaBuilder.hpp"

class Video;
class IVideoHandler;

class VideoBuilder : public MediaBuilder
{
public:
    VideoBuilder& width(int width);
    VideoBuilder& height(int height);
    VideoBuilder& muted(const boost::optional<bool>& muted);
    VideoBuilder& looped(const boost::optional<bool>& looped);

protected:
    std::unique_ptr<IMedia> doBuild() final;

    virtual std::unique_ptr<Video> createVideo();
    virtual std::unique_ptr<IVideoHandler> createHandler();

private:
    int m_width;
    int m_height;
    bool m_looped;
    bool m_muted;

};
