#pragma once

class Image;
class Audio;
class Video;
class WebView;

class MediaVisitor
{
public:
    virtual ~MediaVisitor() = default;

    virtual void visit(Image& image) = 0;
    virtual void visit(Audio& audio) = 0;
    virtual void visit(Video& video) = 0;
    virtual void visit(WebView& webView) = 0;

};
