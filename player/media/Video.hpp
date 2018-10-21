#pragma once

#include "Media.hpp"
#include "adaptors/IVideoHandler.hpp"
#include "wrapper/GstFwd.hpp"

#include <gtkmm/drawingarea.h>
#include <spdlog/spdlog.h>
#include <boost/format.hpp>

class XiboVideoSink;

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
