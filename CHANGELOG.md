### 0.2.1-alpha Navy (hotfix)

**18.04.2018 - `(unstable)`**

- **User Part**
  - Fixed media duration handling
  

### 0.2.0-alpha Navy

**15.04.2018 - `(unstable)`**

- **User Part**
  - Layout scaling (If the Player is 1920x1080 also, then a 1:1 rendering can occur, but if the player is 1280x720 or even 1080x1920, scaling must occur)
  - Audio module
     - Independent media
     - Additional module for other media
  - Duration handling was improved in media
  - Image rendering options (hortizontal align, vertical align, stretching/preserving aspect ration)
  - Fixed video blinking during scaling
- **Developer Part**
  - Media handling refactoring
  - Parsers refactoring
  - Added classes-creators (factory and builders)
  

### 0.1.1-alpha Aqua

**31.03.2018 - `(unstable)`**

- **User Part**
  - Duration fixed for `html` rendering media
  - WebView page reloading fixed
  - Added logging level in command line options
  - Binaries can be downloaded from `releases` on Github
  - Updated README, added CHANGELOG and VERSIONING
- **Developer Part**
  - CommandLineParser added
  - XlfParser refactored
  - Added factory for media, layout and region
  - Build process moved to CMake with Docker
  - Added C++17 support
  - Added continuous integration (Travis CI)
  - Removed `gstreamermm`, `gstreamer` is used now

### 0.1.0-alpha Aqua

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
