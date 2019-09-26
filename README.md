# XiboLinux
[![xibo-player](https://snapcraft.io/xibo-player/badge.svg)](https://snapcraft.io/xibo-player)

## Building from sources

### Building manually

NOTE: these libraries can require additional installations if you haven't such
- `cmake>=3.14` cross-platform software for managing the build process
- `gtk>=3.18` and `gtkmm>=3.18` GUI library and C++ bindings
- `glib>=2.56` and `glibmm>=2.56` low-level system library written in C and C++ bindings
- `gstreamer>=1.8`, `gstreamer-base-plugins>=1.8` multimedia framerwork with base plugins
- `gstreamer1.0-libav` needed for running video content
- `cryptopp>=8.1` for crypto utils (RSA, RC4 etc.)
- `zeromq>=4.3` distributed messaging 
- `webkitgtk>=2.4.10` web content rendering
- `Boost.System>=1.69`, `Boost.Date_Time>=1.69` and `Boost.Thread>=1.69`

### Building with Docker (only for Ubuntu 16.04)
- Install Docker
- Clone this repository
- Run `sudo docker build -t xibo-linux .` in the root of the repository
- `docker run -v /path/to/local/clone/build:/build xibo-linux`
- The Player binaries will be built and output in to your `build` directory
