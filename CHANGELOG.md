### 0.1.1 Aqua

**31.03.2018 - `(unstable)`**

- **User Part**
  - Duration fixed for `html` rendering media
  - WebView page reloading fixed
  - Added logging level in command line options
  - Binaries can be uploaded from `releases` on Github
  - Updated README, added CHANGELOG and VERSIONING
- **Developer Part**
  - CommandLineParser added
  - XlfParser refactored
  - Added factory for media, layout and region
  - Build process moved to CMake with Docker
  - Added C++17 support
  - Added continuous integration (Travis CI)
  - Removed `gstreamermm`, `gstreamer` is used now

### 0.1.0 Aqua

**09.03.2018 - `(unstable)`**

- Initial Player release
- Supported media: video, webview, images
- Video
  - can be looped
  - can be muted
- WebView
  - can render `html` files
  - supports `js` scripts
- Layout
  - support for background image
  - support for background color
- Simple parsing of `.xlf` layout file
- Z-ordering for regions
- Timer for changing media after some interval
- Logging in console
- Some options for main window that are avaialble from source code but not from command line:
  - run Player always on top
  - run in Full Screen mode
  - optionally disable mouse pointer
  - no windowing/no "chrome" mode
- Version can be known using `--version` command line option
- Example can be run using `--example-dir=/path/to/dir` option
  - Folder should contain all media files
  - Folder also should contatin .XLF file
