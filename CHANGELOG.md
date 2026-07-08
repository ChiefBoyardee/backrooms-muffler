# Changelog

All notable changes to Backrooms Muffler will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.3.0] - 2026-07-08

### Added

- **Muffle** control — post-mix pillow-over-speaker chain (LPF, mid peak, soft clip, compressor) on full output
- Ring-style knobs with frosted dark control panel and header title strip
- Muffle unit tests (bypass, HF reduction, output engagement)

### Changed

- Editor resized to 560×360 with five knobs (Corner, Hall, Depth, Mix, Muffle)
- Value readout boxes use dark background with cream text for readability on frosted panel
- Muffle knob uses dusty gold-rose accent (`#C4785A`) to distinguish from spatial controls

## [0.2.0] - 2026-07-08

### Added

- Hall pre-delay (15–70 ms) and post-reverb HF shelf for hallway distance character
- Cascaded 24 dB/oct Corner LPF (exponential 180 Hz–10 kHz curve)
- CC0 Backrooms-style wallpaper, carpet, and ceiling tiles with attribution
- UI control backing plate, label pills, and improved value readout contrast

### Changed

- Corner mid-boost reduced to +2.5 dB max with faster fade at high Corner
- Hall reverb shape retuned (roomSize, damping, stereo width)
- UI palette warmed to match photographic wallpaper tiles

## [0.1.0] - 2026-07-07

### Added

- Initial VST3/AU/Standalone release
- **Corner**, **Hall**, **Depth**, and **Mix** controls for liminal room-muffle effect
- Backrooms-themed UI with yellow wallpaper, fluorescent ceiling bar, and carpet strip
- Catch2 unit tests for DSP parameter mapping and Mix behavior
- GitHub Actions CI with pluginval validation on Windows and macOS

### Fixed

- Mix now behaves as documented global dry/wet (100% = full wet path)
- Corner reverb amount applied inside wet chain instead of limiting output blend

### Changed

- Filter and reverb parameter updates subsampled every 16 samples for reduced zipper noise
- Per-sample smoothed Mix blend; Hall room size and damping smoothed

[Unreleased]: https://github.com/ChiefBoyardee/backrooms-muffler/compare/v0.3.0...HEAD
[0.3.0]: https://github.com/ChiefBoyardee/backrooms-muffler/releases/tag/v0.3.0
[0.2.0]: https://github.com/ChiefBoyardee/backrooms-muffler/releases/tag/v0.2.0
[0.1.0]: https://github.com/ChiefBoyardee/backrooms-muffler/releases/tag/v0.1.0
