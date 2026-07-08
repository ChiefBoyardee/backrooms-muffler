# Changelog

All notable changes to Backrooms Muffler will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

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

[Unreleased]: https://github.com/ChiefBoyardee/backrooms-muffler/compare/v0.1.0...HEAD
[0.1.0]: https://github.com/ChiefBoyardee/backrooms-muffler/releases/tag/v0.1.0
