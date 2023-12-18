
GifSyncAnimator
=======================

[日本語](README.ja.md)

GifSyncAnimator is a plugin that synchronizes GIF animations playback in a DAW (Digital Audio Workstation).

## Notes

### Regarding Mouse Operation Notation

> [!NOTE]
> - Mouse clicks and right-clicks in this document assume the default `right-hand setting` for the operating system.
> - The plugin is compatible with left-hand settings. If your OS is set for left-hand use, please interpret left and right clicks inversely.

## How to Use

1. Load the plugin on your DAW, and click within the window to open a file selection dialog.
2. When you play on the DAW, the GIF animation will play in sync.

## Adjusting Speed

1. After loading a GIF file, right-click within the window to open a popup menu.
2. Select an option in the popup menu to adjust the playback speed.

## DAWs Tested for Compatibility

- Studio One
- Logic Pro X

## For macOS Users

> [!IMPORTANT]
> Currently, there is no distribution for macOS due to the security specifications of macOS. It is necessary to build on the Mac using the DAW. We can distribute it by joining the Apple Developer Program and paying an annual fee of 99 US dollars, allowing Apple to issue a signature. However, there are no plans to do so at present.

## How to Build

### Common Steps

1. Clone this repository.

```bash
git clone git@github.com:r-koubou/GifSyncAnimator.git
```

2. Clone the external git submodules.

```bash
git submodule update --init --recursive
```

### Windows

1. Install the following software:
- Visual Studio 2022
- CMake 3.15 or higher
  - Add the path to cmake.exe to your environment variable PATH.

2. Execute `build.win.release.bat` to perform the build.

### macOS

1. Install the following software:
- Xcode
- CMake 3.15 or higher
  - Can be installed via Homebrew
    - `brew install cmake`

2. Execute `build.mac.release.sh` to perform the build.

### Location of the Built Plugin

The `build/GifSyncAnimator_artefacts/release` directory contains folders for each plugin format built.

## License

- GPL v3
