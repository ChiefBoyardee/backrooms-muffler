# UI Guidelines

## Palette

| Element | Hex |
|---|---|
| Wallpaper base | `#D4B84A` |
| Wallpaper highlight | `#E8D174` |
| Wallpaper seam | `#B89A38` |
| Carpet base | `#B8A88A` |
| Carpet speckle | `#9A8B72` |
| Ceiling panel | `#E8E4D9` |
| Ceiling grid | `#8A8578` |
| Fluorescent | `#F5F8F2` |
| Fluorescent tint | `#D8E8D0` |
| Label text | `#3D3428` |
| Knob fill | `#F0EDE6` |
| Knob accent | `#C9A227` |

## Layout

- Window: 480×320 (fixed in v0.1.0)
- Ceiling bar: 36 px
- Carpet strip: 40 px
- Four rotary knobs: Corner, Hall, Depth, Mix

## Assets policy

- Use **original** seamless tiles in `assets/ui/`
- Do not import textures from Backrooms games, films, or other copyrighted works
- Procedural fallback exists in `WallpaperComponent` when images are unavailable

## Implementation

- `BackroomsLookAndFeel` — rotary knob styling
- `WallpaperComponent` — ceiling, tiled wallpaper, carpet
- Assets embedded via `juce_add_binary_data` → `BinaryData.h`
