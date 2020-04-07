#include "ScreenShoter.hpp"

#include <boost/beast/core/detail/base64.hpp>

ScreenShoter::ScreenShoter(Xibo::Window& window) : window_(window) {}

void ScreenShoter::takeBase64(const ScreenShotTaken& callback)
{
    takeScreenshotNative(nativeWindow(), [callback = std::move(callback)](const ImageBuffer& buffer) {
        auto base64 = boost::beast::detail::base64_encode(buffer.data(), buffer.size());
        callback(base64);
    });
}

NativeWindow ScreenShoter::nativeWindow() const
{
    return window_.nativeWindow();
}
