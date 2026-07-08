# Build

## Prerequisites

### Windows

- Visual Studio 2022 with **Desktop development with C++**
- CMake 3.22+
- Git

### macOS

- Xcode command-line tools
- CMake 3.22+

## Clone

```bash
git clone --recurse-submodules https://github.com/ChiefBoyardee/backrooms-muffler.git
cd backrooms-muffler
```

If you already cloned without submodules:

```bash
git submodule update --init --recursive
```

## Configure and build (Windows)

```powershell
cmake --preset vs2022
cmake --build --preset vs2022-release
```

## Configure and build (macOS)

```bash
cmake --preset ninja-release
cmake --build --preset ninja-release
```

## Output paths

| Platform | VST3 |
|---|---|
| Windows | `build/BackroomsMuffler_artefacts/Release/VST3/Backrooms Muffler.vst3` |
| macOS | `build/BackroomsMuffler_artefacts/Release/VST3/Backrooms Muffler.vst3` |

## Install for Ableton Live 12 (Windows)

1. Copy `Backrooms Muffler.vst3` to `C:\Program Files\Common Files\VST3\`
2. Open Ableton Live 12 → **Preferences → Plug-ins**
3. Enable **Use VST3 Plug-in System Folders**
4. Click **Rescan** (hold **Alt** while clicking for a deep rescan)
5. Find **Backrooms Muffler** under Plug-ins → VST3

## Run tests

```powershell
cmake --build build --config Release --target BackroomsMufflerTests
ctest --test-dir build -C Release --output-on-failure
```

## Validate with pluginval

Download [pluginval](https://github.com/Tracktion/pluginval/releases) and run:

```powershell
pluginval.exe --strictness-level 5 --skip-gui-tests --validate "build\BackroomsMuffler_artefacts\Release\VST3\Backrooms Muffler.vst3"
```

## Troubleshooting

- **Plugin not in browser:** confirm 64-bit VST3 path, separate from VST2 folder
- **Build fails on JUCE:** run `git submodule update --init --recursive`
- **Denormals / clicks:** report an issue with DAW sample rate and buffer size
