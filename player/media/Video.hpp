#pragma once

#include "Media.hpp"
#include "IVideoHandler.hpp"

class Video : public Media<IVisibleMedia>
{
public:
    Video(bool looped, std::unique_ptr<IVideoHandler>&& handler);

    void scale(double, double) override;
    int width() const override;
    int height() const override;

    IWidgetAdaptor& handler() override;
    void apply(MediaVisitor& visitor) override;

protected:
    void doStop() override;
    void doStart() override;

private:
    void onVideoFinished();

private:
    std::unique_ptr<IVideoHandler> m_handler;
    bool m_looped;

};
