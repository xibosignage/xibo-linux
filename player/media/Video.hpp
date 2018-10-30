#pragma once

#include "Media.hpp"
#include "IVideoHandler.hpp"

class Video : public Media<IVisibleMedia>
{
public:
    Video(int id, int width, int height, const FilePath& path, std::unique_ptr<IVideoHandler>&& handler);

    void scale(double, double) override;
    int width() const override;
    int height() const override;

    void setLooped(bool looped);
    void setMuted(bool muted);

    IWidgetAdaptor& handler() override;
    void apply(MediaVisitor& visitor) override;

protected:
    void doStop() override;
    void doStart() override;

private:
    void onVideoFinished();

private:
    std::unique_ptr<IVideoHandler> m_handler;
    bool m_looped = false;

};
