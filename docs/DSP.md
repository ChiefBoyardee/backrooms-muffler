# DSP Notes

## Signal chain

1. Dry/wet split
2. Wet path: HPF (~80 Hz) → **cascaded LPF ×2** (Corner + Depth, 24 dB/oct) → mid peak (~500 Hz) → **Hall pre-delay** → hall reverb (Corner wet/dry) → **Hall HF shelf** → soft compressor → depth gain
3. Mix knob blends dry input with fully processed wet path

## Parameter smoothing

All modulated targets use **50 ms linear** smoothing (`juce::SmoothedValue`).

- Filter coefficients and reverb/Hall parameters update every **16 samples** using advanced smoothed values (reduces zipper noise on fast automation).
- Mix dry/wet blend is applied **per sample** with smoothed Mix.

## Parameter mapping

### Corner (0–100%)

Master macro for muffled ↔ clear. Default: **35%**. LPF cutoff uses an **exponential** curve.

| Corner | LPF cutoff | Mid boost | Reverb wet (in wet path) | Compressor |
|---:|---:|---:|---:|---|
| 0% | ~180 Hz | +2.5 dB | 45% | threshold −24 dB |
| 50% | ~1.3 kHz | ~+0.9 dB | ~26% | threshold −12 dB |
| 100% | ~10 kHz | 0 dB | 8% | threshold 0 dB |

The **Reverb wet** column is the internal `juce::dsp::Reverb` wet/dry mix inside the wet chain — not the output dry/wet (that is **Mix**).

Mid boost fades faster at higher Corner: `lerp(+2.5 dB, 0, corner^0.7)`.

### Hall (0–100%)

Hallway distance and space — not just reverb amount.

| Hall | Pre-delay | HF cutoff | roomSize | damping | width |
|---:|---:|---:|---:|---:|---:|
| 0% | ~15 ms | ~7 kHz | 0.18 | 0.82 | 0.55 |
| 100% | ~70 ms | ~2.5 kHz | 0.72 | 0.45 | 0.85 |

- **Pre-delay** — signal delay before reverb (hallway distance)
- **HF cutoff** — post-reverb low-pass (air absorption down the hall)
- **roomSize / damping / width** — JUCE `Reverb` shape (narrower at low Hall)

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
- v0.2.0: Exponential Corner curve (180 Hz–10 kHz), cascaded LPF, Hall pre-delay + HF shelf, reverb shape retune.

## Tuning critique (prioritized)

### P0 — addressed in v0.2.0

- Aggressive Corner muffling (exponential curve + 24 dB/oct LPF).
- Hall hallway character (pre-delay, HF absorption, narrower low-Hall reverb).

### P1 — v0.3 candidates

- Factory presets (e.g. “Around the Corner”, “Deep Hall”, “Drywall”).
- True compressor bypass at Corner 100% (currently threshold → 0 dB).

### P2 — nice-to-have

- Fluorescent ceiling bar flicker in UI.
- Custom font from `assets/fonts/`.
- Resizable editor.
- Sine-sweep regression tests for LPF rolloff.

### Ear-test checklist

Before tagging a release, verify in a DAW:

1. Corner 15 / Hall 40 / Depth 50 / Mix 100 — clearly muffled vs dry.
2. Corner 80 / same — noticeably clearer.
3. Hall 0 vs 100 at fixed Corner 20 — longer hall feels distant, not just more reverb wash.
4. Mix 0% / 50% / 100% at fixed Corner/Hall/Depth.
