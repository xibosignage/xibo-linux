#pragma once

#include <memory>

class ImageModel;
class WebViewModel;
class AudioModel;
class VideoModel;

class IMediaVisitor
{
public:
    virtual ~IMediaVisitor() = default;
    virtual void visit(const std::shared_ptr<ImageModel>&) = 0;
    virtual void visit(const std::shared_ptr<WebViewModel>&) = 0;
    virtual void visit(const std::shared_ptr<AudioModel>&) = 0;
    virtual void visit(const std::shared_ptr<VideoModel>&) = 0;
};
