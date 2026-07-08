# Backrooms Muffler

A liminal room-muffle audio effect plugin. Place your audio inside the endless yellow halls and turn the **Corner** knob to hear it grow muffled and distant — as if the source is deeper in the Backrooms while you stay in a clearer stretch of hallway.

## Features

- **Corner** — how far around the bend the source sits (muffled ↔ clear)
- **Hall** — fluorescent corridor size and decay
- **Depth** — how deep into the Backrooms the source lives
- **Mix** — reality vs liminal bleed

## Formats

- VST3 (Windows, macOS)
- AU (macOS)
- Standalone

## Build

See [docs/BUILD.md](docs/BUILD.md).

```powershell
cmake --preset vs2022
cmake --build --preset vs2022-release
```

Windows VST3 output:

`build/BackroomsMuffler_artefacts/Release/VST3/Backrooms Muffler.vst3`

## Ableton Live 12

1. Copy the `.vst3` bundle to `C:\Program Files\Common Files\VST3\`
2. Live **Preferences → Plug-ins** → enable **Use VST3 Plug-in System Folders**
3. Rescan plug-ins (Alt+click **Rescan** for a deep scan)
4. Load **Backrooms Muffler** from the VST3 browser on an audio track

## License

This project is licensed under the **GNU Affero General Public License v3.0** (AGPL-3.0). See [LICENSE](LICENSE).

JUCE is dual-licensed under AGPLv3 and a commercial license. This project uses JUCE under AGPLv3.

## Visual assets

UI patterns are **original artwork** inspired by a liminal aesthetic. Do not redistribute assets ripped from Backrooms games, films, or other copyrighted sources.
