#pragma once

#include "IMediaVisitor.hpp"
#include "parsers/Options.hpp"

#include "utils/Uri.hpp"

class MediaModel
{
public:
    MediaModel(const MediaOptions& options);
    virtual ~MediaModel() = default;

    virtual void apply(IMediaVisitor& visitor) = 0;

    void attachAudio(const std::shared_ptr<AudioModel>& media);
    std::shared_ptr<AudioModel> attachedAudio() const;

    int id() const;
    int duration() const;
    Uri uri() const;

private:
    int m_id;
    int m_duration;
    Uri m_uri;
    std::shared_ptr<AudioModel> m_attachedMedia;

};
