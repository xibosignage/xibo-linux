# XiboLinux

## Building from sources

NOTE: these packages can require additional installations if you haven't such
- `cmake>=3.5.1` cross-platform software for managing the build process
- `gtk>=3.18` and `gtkmm>=3.18` GUI library and C++ bindings
- `gstreamer>=1.8`, `gstreamer-base-plugins>=1.8` and g`streamermm>=1.8` multimedia framerwork, base plugins and C++ bindings
- `webkitgtk>=2.4.11` web content rendering
- `boost_filesystem>=1.58` and `boost_program_options>=1.58`
  
### General steps to build player
- Download sources from GitHub or run `git clone https://github.com/Stivius/XiboLinuxStack`
- Install CMake
- Install platform specific dependencies
- Install spdlog `git clone https://github.com/gabime/spdlog` and `cd spdlog && cmake . && make && sudo make install`
- Open top level source directory and run `cmake -H. -B_build`)
- `cd _build` and run `make && make install`

### Platofrm specific dependencies
  #### Ubuntu 16.04/Linux Mint 18
  Dev packages `sudo apt install cmake libgtkmm-3.0-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libwebkitgtk-3.0-dev libboost-filesystem1.58-dev libbost-program-options1.58-dev`)
  
  ##### Known issues on Ubuntu 16.04/Linux Mint 18
  gstreamermm is 1.4.3 in xenial repos. You need to install 1.8.0 manually from [sources](https://github.com/GNOME/gstreamermm/releases/tag/1.8.0) after installing [`mm-common>=0.9.10`](https://github.com/GNOME/mm-common/releases/tag/0.9.10) and [`glibmm-2.4>=2.47.6`](https://github.com/GNOME/glibmm/releases/tag/2.47.6) because 1.8 version requires them to be built
  
  #### Manjaro 17
  Packages `sudo pacman -Syy cmake gtk3 gtkmm3 gstreamermm boost`
  
  ##### Known issues on Manjaro 17
  WebkitGTK is available in AUR and can be built from source but this installation will require a lot of time because the package is huge. To speed up process you can add additional repositiory as described [here](https://github.com/archlinuxcn/repo/blob/master/README.md) and install it like usual package `sudo pacman -Syy webkitgtk`

## Running Player
- `sudo apt install gstreamer1.0-libav` for running video examples (I hope to get rid of this installation)
- Just run `./Player` to get info about available command options
- `./Player --example-dir=/path-to-XiboExamples-folder/Examples` to run specified example
- `./Player --version` print current version (we need VERSIONING.md and CHANGELOG.md in future)

## Running tests
- Download `XiboExamples.tar.gz` from releases on Github and extract it to `source-dir/_build/Player`
- Open _build directory and run `ctests`

## Binary distribution
To run on another machine the package that you've built you need to copy `Player` executable together with `libs` folder and `XiboExamples` folder from `source-dir/_build/Player`
