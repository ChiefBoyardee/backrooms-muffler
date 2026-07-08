# UI Guidelines

## Palette (v0.2.0)

| Element | Hex |
|---|---|
| Wallpaper base | `#C9A83A` |
| Wallpaper highlight | `#E2C45A` |
| Wallpaper seam | `#A8872E` |
| Carpet base | `#8F7D62` |
| Carpet speckle | `#6E5F4C` |
| Ceiling panel | `#E4E0D4` |
| Ceiling grid | `#6E6A60` |
| Fluorescent | `#F8FAF4` |
| Fluorescent tint | `#D4E6CC` |
| Label text | `#2A2418` |
| Knob fill | `#F5F1E8` |
| Knob accent | `#B8921E` |
| Control plate | `#F0EBE0` |

## Layout

- Window: 480×320 (fixed in v0.2.0)
- Ceiling bar: 36 px (tiled `ceiling_tile.png` when embedded)
- Carpet strip: 40 px
- Control plate: rounded rect behind knob row (~88% opacity)
- Four rotary knobs: Corner, Hall, Depth, Mix
- Label pills: 24 px height, light background
- Value text boxes: 60×20 px, opaque fill + outline

## Assets policy

- Primary tiles live in `assets/ui/` and are embedded via CMake `juce_add_binary_data`
- CC0 third-party tiles are permitted with attribution in [`assets/ui/ATTRIBUTION.md`](../assets/ui/ATTRIBUTION.md)
- Do **not** import textures from Backrooms games, Kane Pixels films, or other copyrighted works without license
- Procedural fallback exists in `WallpaperComponent` when images are unavailable

### Current assets (v0.2.0)

| File | Source |
|---|---|
| `wallpaper_tile.png` | [amini-allight/backrooms-textures](https://github.com/amini-allight/backrooms-textures) (CC0), resized 512×512 |
| `carpet_tile.png` | same, resized 384×384 |
| `ceiling_tile.png` | same, resized 512×128 |

## Implementation

- `BackroomsLookAndFeel` — rotary knob styling, label pills, slider text box colours
- `WallpaperComponent` — ceiling tile, tiled wallpaper, control plate, carpet
- Assets embedded via `juce_add_binary_data` → `BinaryData.h`
