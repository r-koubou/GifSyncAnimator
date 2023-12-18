GifSyncAnimator
=======================

[日本語](README.ja.md)

GifSyncAnimator is a plugin for synchronizing GIF animations in a Digital Audio Workstation (DAW).

## Notes

### Regarding Mouse Operation Notation

> [!NOTE]
> - Mouse clicks and right-clicks mentioned in this document assume the default 'right-hand setting' of the OS.
> - The plugin is compatible with left-hand settings, so if your OS is set for left-handed use, please interpret left and right clicks inversely.

## How to Use

1. Load the plugin in your DAW, and click within the window to open a file selection dialog.
2. When you play on the DAW, the GIF animation will be synchronized and played.

## Speed Adjustment

1. After loading the GIF file, right-click within the window to open a popup menu.
2. You can adjust the playback speed by selecting options from the popup menu.

## Tested DAWs

- Studio One
- Logic Pro X

## To macOS Users

> [!IMPORTANT]
> Currently, distribution is not carried out on macOS due to security specifications of macOS. It is necessary to build on the Mac used for the DAW.
> Joining the Apple Developer Program and paying an annual fee of 99 US dollars allows for the distribution after receiving a signature from Apple, but there are no plans for this at present.

## Build Instructions

### Common

1. Clone this repository.

   ```bash
   git clone git@github.com:r-koubou/GifSyncAnimator.git
   ```

2. Clone external git submodules.

   ```bash
   git submodule update --init --recursive
   ```

### Windows

1. Install the following software:
   - Visual Studio 2022
   - CMake 3.15 or higher
     - Add the path to cmake.exe to the environment variable PATH.

2. Execute `build.win.release.bat` to perform the build.

### macOS

1. Install the following software:
   - Xcode
   - CMake 3.15 or higher
     - Installable via Homebrew:
       - `brew install cmake`

2. Execute `build.mac.release.sh` to perform the build.

### Location of the Built Plugin

The `build/GifSyncAnimator_artefacts/release` directory contains folders for each built plugin format.

## License

- GPL v3
