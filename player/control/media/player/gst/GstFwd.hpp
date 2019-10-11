#pragma once

#include <memory>

namespace Gst
{
    class AudioConvert;
    class AutoAudioSink;
    class Caps;
    class Capsfilter;
    class Decodebin;
    class Element;
    class UriSrc;
    class Message;
    class Pad;
    class Pipeline;
    class Queue;
    class VideoConvert;
    class VideoScale;
    class Volume;

    template <typename T>
    using RefPtr = std::shared_ptr<T>;
}
