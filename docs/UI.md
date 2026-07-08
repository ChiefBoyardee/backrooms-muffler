# UI Guidelines

## Palette (v0.3.0)

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
| Label text | `#F8F4EA` |
| Knob ring | `#F5F1E8` |
| Knob accent | `#B8921E` |
| Muffle accent | `#C4785A` |
| Control plate | `#1E1A14` @ 72% alpha |
| Control overlay | black @ 40% (dims wallpaper behind plate) |
| Plate border | `#B8921E` 1 px |
| Value background | `#2A2418` |
| Value text | `#F8F4EA` |
| Value outline | `#B8921E` |

## Layout

- Window: **560×360** (fixed in v0.3.0)
- Ceiling bar: 36 px (tiled `ceiling_tile.png` when embedded); **header title** left-aligned in strip
- Carpet strip: 40 px (no bottom title pill)
- Control zone: 40% dark overlay on wallpaper, then frosted dark control plate
- Five ring knobs: Corner, Hall, Depth, Mix, **Muffle**
- Knob style: hollow ring with arc + dot indicator (FabFilter-inspired)
- Label pills: 24 px height, light text on dark plate
- Value text boxes: 64×22 px, dark background with cream text and gold outline

## Assets policy

- Primary tiles live in `assets/ui/` and are embedded via CMake `juce_add_binary_data`
- CC0 third-party tiles are permitted with attribution in [`assets/ui/ATTRIBUTION.md`](../assets/ui/ATTRIBUTION.md)
- Do **not** import textures from Backrooms games, Kane Pixels films, or other copyrighted works without license
- Procedural fallback exists in `WallpaperComponent` when images are unavailable

### Current assets (v0.2.0+)

| File | Source |
|---|---|
| `wallpaper_tile.png` | [amini-allight/backrooms-textures](https://github.com/amini-allight/backrooms-textures) (CC0), resized 512×512 |
| `carpet_tile.png` | same, resized 384×384 |
| `ceiling_tile.png` | same, resized 512×128 |

## Implementation

- `BackroomsLookAndFeel` — ring rotary knobs, frosted panel, label pills, slider text box colours, Muffle accent ring
- `WallpaperComponent` — ceiling tile, tiled wallpaper, control overlay + plate, carpet
- Assets embedded via `juce_add_binary_data` → `BinaryData.h`
