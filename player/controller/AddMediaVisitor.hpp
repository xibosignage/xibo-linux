#pragma once

#include "model/IMediaVisitor.hpp"

#include "controller/RegionController.hpp"

class AddMediaVisitor : public IMediaVisitor
{
public:
    AddMediaVisitor(FixedLayout& view, RegionController& controller);

    void visit(const std::shared_ptr<ImageModel>& image) override;
    void visit(const std::shared_ptr<WebViewModel>& webview) override;
    void visit(const std::shared_ptr<AudioModel>& audio) override;
    void visit(const std::shared_ptr<VideoModel>& video) override;

private:
    void addMediaController(std::unique_ptr<MediaController>&& controller);

private:
    FixedLayout& m_view;
    RegionController& m_controller;

};
