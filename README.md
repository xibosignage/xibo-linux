# XiboLinux
[![xibo-player](https://snapcraft.io/xibo-player/badge.svg)](https://snapcraft.io/xibo-player)

## Building from sources

### Building manually

NOTE: These libraries can require additional installations if you haven't such. We have tested only with listed libraries versions, however, it can be possible to build with newer or older versions as well.
- `cmake>=3.14` cross-platform software for managing the build process
- `gtk>=3.22` and `gtkmm>=3.22` GUI library and C++ bindings
- `glib>=2.56` and `glibmm>=2.56` low-level system library written in C and C++ bindings
- `gstreamer, gstreamer-base-plugins, gstreamer-good-plugins, gstreamer-bad-plugins>=1.14` multimedia framerwork for audio/video playback with plugins
- `gstreamer1.0-libav`, `gstreamer1.0-gtk3`, `gstreamer1.0-gl`, `gstreamer1.0-vaapi`, `gstreamer1.0-alsa`, `gstreamer1.0-pulseaudio` needed for running/accelerating video content
- `cryptopp=8.1` for crypto utils (RSA, RC4 etc.)
- `cryptopp-pem`that is compatiable with `cryptopp` version needed for PEM key format
- `zeromq=4.3` distributed messaging
- `spdlog=1.4.1` logging library
- `gtest=1.8.1` for tests
- `webkit2gtk>=2.28.4` web content rendering
- `Boost.System, Boost.Filesystem, Boost.Date_Time, Boost.Thread=1.70` 

### Building with Docker (only for Ubuntu 18.04)
- Install Docker
- Clone this repository
- Run `sudo docker build -t xibo-linux .` in the root of the repository
- `docker run -v /path/to/local/clone/build:/build xibo-linux`
- The Player binaries will be built and output in to your `build` directory
