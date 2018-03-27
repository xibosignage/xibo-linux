# XiboLinux

| `develop` | `master`|
|--------|--------|
| [![Build Status](https://travis-ci.org/Stivius/XiboLinuxStack.svg?branch=develop)](https://travis-ci.org/Stivius/XiboLinuxStack) | [![Build Status](https://travis-ci.org/Stivius/XiboLinuxStack.svg?branch=master)](https://travis-ci.org/Stivius/XiboLinuxStack)

## Building from sources

### Required dependencies
NOTE: these packages can require additional installations if you haven't such
- `cmake>=3.5.1` cross-platform software for managing the build process
- `gtk>=3.18` and `gtkmm>=3.18` GUI library and C++ bindings
- `gstreamer>=1.8`, `gstreamer-base-plugins>=1.8` and `gstreamermm>=1.8` multimedia framerwork, base plugins and C++ bindings
- `webkitgtk>=2.4.11` web content rendering
- `boost_filesystem>=1.58` and `boost_program_options>=1.58`

### Building with Docker for Ubuntu 16.04
- Install Docker
- Clone this repository
- Pull the docker image from hub: `docker pull stivius/xibo-linux`
- From the root of the repository: `docker build --cache-from stivius/xibo-linux .`
- `docker run -v /path/to/local/clone/output:/output stivius/xibo-linux`
- The Player binaries will be build and output in to your `output` directory

## Running Player
- `sudo apt install gstreamer1.0-libav` for running video examples (I hope to get rid of this installation)
- Just run `./Player` to get info about available command options
- `./Player --example-dir=/path-to-XiboExamples-folder/Examples` to run specified example
- `./Player --version` print current version (check VERSIONING.md and CHANGELOG.md to know more about your player version)

## Running tests
- Download `XiboExamples.tar.gz` from releases on Github and extract it to `source-dir/_build/Player`
- Open _build directory and run `ctests`

## Binary distribution
To run on another machine the package that you've built you need to copy `Player` executable together with `libs` folder and `XiboExamples` folder from `source-dir/_build/Player`
