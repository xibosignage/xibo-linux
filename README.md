# XiboLinux

| `dev (v0.7-alpha)` | `master (v0.6-alpha)`|
|--------|--------|
| [![CircleCI](https://circleci.com/gh/Stivius/XiboLinuxStack/tree/v0.7-alpha.svg?style=svg)](https://circleci.com/gh/Stivius/XiboLinuxStack/tree/v0.7-alpha) | [![CircleCI](https://circleci.com/gh/Stivius/XiboLinuxStack/tree/master.svg?style=svg)](https://circleci.com/gh/Stivius/XiboLinuxStack/tree/master)

## Building from sources

### Building manually
NOTE: these libraries can require additional installations if you haven't such
- `cmake>=3.12` cross-platform software for managing the build process
- `gtk>=3.18` and `gtkmm>=3.18` GUI library and C++ bindings
- `gstreamer>=1.8`, `gstreamer-base-plugins>=1.8` multimedia framerwork with base plugins
- `webkitgtk>=2.4.10` web content rendering
- `Boost.Filesystem>=1.66` and `Boost.Program_options>=1.66`
- Also `gstreamer1.0-libav` needed for running video content (I hope to get rid of this addition requirement)

### Building with Docker for Ubuntu 16.04
- Install Docker
- Clone this repository
- Run `docket build -t xibo-linux .` in the root of the repository
- `docker run -v /path/to/local/clone/build:/build xibo-linux`
- The Player binaries will be build and output in to your `build` directory

## Running Player
- Just run `./player` to get info about available command options
- `./player --host=your_cms_host --server-key=key_in_cms --hardware-key=type_random_symbols`
- `./player --version` print current version (check VERSIONING.md and CHANGELOG.md to know more about your player version)

## Binary distribution
To run on another machine the package that you've built you need to copy `player` executable together with `libs` folder from `source-dir/_build/player`
