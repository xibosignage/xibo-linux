# XiboLinux

## Building from sources
### Ubuntu 16.04/Linux Mint 18
- Install CMake at least 5.3.1 (`sudo apt install cmake`)
- Install dependencies (dev packages)
  - gtkmm at least 3.18 (`sudo apt install libgtkmm-3.0-dev`)
  - gstreamer and gstreamer base plugins at least 1.8 (`sudo apt install libgstreamer-1.0-dev libgstreamer-plugins-base1.0-dev`)
  - gstreamermm at least 1.8 (`sudo apt install libgstreamermm-1.0-dev`)
  - webkitgtk at least 2.4.11 (`sudo apt install libwebkitgtk-3.0-dev`)
  - boost modules (filesystem and program_options) at least 1.58 (`sudo apt install libboost-filesystem1.58-dev libbost-program-options1.58-dev`)
- Open top level source directory and run `cmake -H. -B_build`)
- `cd _build` and run `make && make install`

## Running Player
- `sudo apt install gstreamer1.0-libav` for running video examples (I hope to get rid of this installation)
- Just run `./Player` to get info about available command options
- `./Player --example-dir=/path-to-XiboExamples-folder/Examples` to run specified example
- `./Player --version` print current version (we need VERSIONING.md and CHANGELOG.md in future)

## Running tests
- Download `XiboExamples.tar.gz` from releases on Github and extract it to `source-dir/_build/Player`
- Open _build directory and run `ctests`

## Binary distribution
To run package that you've built on another machine you need to copy `Player` executable together with `libs` folder and `XiboExamples` folder from `source-dir/_build/Player`
