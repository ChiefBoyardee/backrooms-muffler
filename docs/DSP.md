# DSP Notes

## Signal chain

1. Dry/wet split
2. Wet path: HPF (~80 Hz) → LPF (Corner + Depth) → mid peak (~500 Hz) → hall reverb (Corner wet/dry) → soft compressor → depth gain
3. Mix knob blends dry input with fully processed wet path

## Parameter smoothing

All modulated targets use **50 ms linear** smoothing (`juce::SmoothedValue`).

- Filter coefficients and reverb/Hall parameters update every **16 samples** using advanced smoothed values (reduces zipper noise on fast automation).
- Mix dry/wet blend is applied **per sample** with smoothed Mix.

## Parameter mapping

### Corner (0–100%)

Master macro for muffled ↔ clear. Default: **35%**.

| Corner | LPF cutoff | Mid boost | Reverb wet (in wet path) | Compressor |
|---:|---:|---:|---:|---|
| 0% | ~350 Hz | +4 dB | 35% | threshold −24 dB |
| 100% | ~12 kHz | 0 dB | 5% | threshold 0 dB |

The **Reverb wet** column is the internal `juce::dsp::Reverb` wet/dry mix inside the wet chain — not the output dry/wet (that is **Mix**).

### Hall (0–100%)

Controls `juce::dsp::Reverb` room size and damping (smoothed).

- Small hall: roomSize ~0.25, high damping
- Large hall: roomSize ~0.85, lower damping

### Depth (0–100%)

- Multiplies LPF cutoff (1.0 → 0.45×)
- Applies wet-path gain (1.0 → 0.55)

### Mix (0–100%)

Global dry/wet blend between unaffected input and the fully processed wet path.

- **0%** = bypass (dry input only)
- **100%** = full wet chain output

## Tuning log

- v0.1.0: Initial curves chosen from door-muffle research (LPF 350 Hz–12 kHz, short hall reverb, drywall mid peak at 500 Hz).
- v0.1.0: Mix fixed to true global dry/wet; Corner reverb amount moved inside wet path; subsampled filter/Hall smoothing added.

## Tuning critique (prioritized)

### P0 — addressed in v0.1.0

- Mix semantics aligned with documentation (100% Mix = full wet path).
- Filter coefficient and Hall/reverb parameter updates subsampled every 16 samples.
- Per-sample smoothed Mix blend.

### P1 — v0.2 candidates

- Factory presets (e.g. “Around the Corner”, “Deep Hall”, “Drywall”).
- Mono/stereo-safe reverb width handling.
- True compressor bypass at Corner 100% (currently threshold → 0 dB).

### P2 — nice-to-have

- Fluorescent ceiling bar flicker in UI.
- Custom font from `assets/fonts/`.
- Resizable editor.
- Sine-sweep regression tests for LPF rolloff.

### Ear-test checklist

Before tagging a release, verify in a DAW:

1. Corner sweep at Mix 100% — muffled (0%) vs clear (100%).
2. Hall size at Depth 0% and 100%.
3. Fast automation on Corner and Hall — listen for zipper artifacts.
4. Mix 0% / 50% / 100% at fixed Corner/Hall/Depth.
