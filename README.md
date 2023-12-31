GifSyncAnimator
=======================

GifSyncAnimator is a plugin for synchronizing GIF animations with music in a DAW (Digital Audio Workstation).

![demo](./assets/docs/images/demo.gif)
gif file from [Giphy](https://giphy.com/gifs/party-pizza-pizza58e2a1c2e81a0281819863-Ub8XEam5vXbMY)

## Supported OS

| OS                            | VST3 | AU  |
| ----------------------------- | ---- | --- |
| Windows (Intel x86_64)        | ✔   |     |
| macOS (AppleSilicon arm64) *1 | ✔   | ✔  |
| macOS (Intel x86_64) *1       | ✔   | ✔  |

*1 On macOS, the plugin itself is not currently distributed.
You will need to build it on a Mac using DAW. For more details, please refer to [For macOS users](#for_mac_users).

## Notes

### About Mouse Operation Notation

> [!NOTE]
> - Mouse clicks and right-clicks mentioned in this document assume the default 'right-hand setting' of the OS.
> - The plugin is compatible with left-hand settings, so if your OS is set for left-handed use, please interpret left and right clicks inversely.

## How to Use

1. Load the plugin in your DAW, and click within the window to open the file selection dialog.
2. When you play in the DAW, the GIF animation will play in sync.

## Adjusting Animation Speed

By default, the animation performs one loop per measure. If this is too fast or too slow, you can adjust the animation speed.

1. After loading the GIF file, right-click within the window to open a popup menu.
2. You can adjust the playback speed by selecting options from the popup menu.

![popup_menu](./assets/docs/images/popup_animation.jpg)

## DAWs Tested

- Studio One
- Logic Pro X

<a id="for_mac_users"></a>

## For macOS Users

> [!IMPORTANT]
> On macOS, due to the security specifications of macOS and the requirements of the Apple Developer Program, we are not currently distributing it. You will need to build it on your own Mac.
> I could distribute it by joining the Apple Developer Program and using the signature issued by Apple, but there are no plans to do so at the current time.

## Build from source code

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

- Debug Build: `build/GifSyncAnimator_artefacts/debug`
- Release Build: `build/GifSyncAnimator_artefacts/release`

## License

- GPL v3
