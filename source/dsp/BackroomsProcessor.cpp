#include "BackroomsProcessor.h"

#include <cmath>

namespace
{
constexpr float kMinCutoffHz = 180.0f;
constexpr float kMaxCutoffHz = 10000.0f;
constexpr float kHpfHz = 80.0f;
constexpr float kMidPeakHz = 500.0f;
constexpr float kMidPeakQ = 0.9f;
constexpr float kMaxMidBoostDb = 2.5f;
constexpr float kMaxReverbWet = 0.45f;
constexpr float kMinReverbWet = 0.08f;
constexpr float kMinDepthGain = 0.55f;
constexpr float kMinPredelayMs = 15.0f;
constexpr float kMaxPredelayMs = 70.0f;
constexpr float kMinHallHfCutoffHz = 2500.0f;
constexpr float kMaxHallHfCutoffHz = 7000.0f;

float lerp (float a, float b, float t) noexcept
{
    return a + (b - a) * t;
}
} // namespace

float BackroomsProcessor::mapCornerToCutoffHz (float cornerNorm)
{
    const auto t = juce::jlimit (0.0f, 1.0f, cornerNorm);
    return kMinCutoffHz * std::pow (kMaxCutoffHz / kMinCutoffHz, t);
}

float BackroomsProcessor::mapDepthToCutoffMultiplier (float depthNorm)
{
    const auto t = juce::jlimit (0.0f, 1.0f, depthNorm);
    return lerp (1.0f, 0.45f, t);
}

void BackroomsProcessor::prepare (const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    numChannels = static_cast<int> (spec.numChannels);

    highPass.prepare (spec);
    lowPass.prepare (spec);
    lowPass2.prepare (spec);
    midPeak.prepare (spec);
    hallHighShelf.prepare (spec);
    predelay.prepare (spec);
    reverb.prepare (spec);
    compressor.prepare (spec);

    const auto maxPredelaySamples = static_cast<int> (std::ceil (sampleRate * kMaxPredelayMs * 0.001));
    predelay.setMaximumDelayInSamples (maxPredelaySamples);

    const auto smoothTime = 0.05;
    smoothedCutoff.reset (sampleRate, smoothTime);
    smoothedMidGainDb.reset (sampleRate, smoothTime);
    smoothedReverbWet.reset (sampleRate, smoothTime);
    smoothedCompThreshold.reset (sampleRate, smoothTime);
    smoothedDepthGain.reset (sampleRate, smoothTime);
    smoothedRoomSize.reset (sampleRate, smoothTime);
    smoothedDamping.reset (sampleRate, smoothTime);
    smoothedReverbWidth.reset (sampleRate, smoothTime);
    smoothedPredelaySamples.reset (sampleRate, smoothTime);
    smoothedHallHfCutoff.reset (sampleRate, smoothTime);
    smoothedMix.reset (sampleRate, smoothTime);

    wetBuffer.setSize (numChannels, static_cast<int> (spec.maximumBlockSize));

    *highPass.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, kHpfHz);

    compressor.setRatio (2.0f);
    compressor.setAttack (20.0f);
    compressor.setRelease (120.0f);

    reset();
    setParameters (corner, hall, depth, mix);
}

void BackroomsProcessor::reset()
{
    highPass.reset();
    lowPass.reset();
    lowPass2.reset();
    midPeak.reset();
    hallHighShelf.reset();
    predelay.reset();
    reverb.reset();
    compressor.reset();
}

void BackroomsProcessor::setParameters (float cornerPercent,
                                        float hallPercent,
                                        float depthPercent,
                                        float mixPercent)
{
    corner = juce::jlimit (0.0f, 100.0f, cornerPercent);
    hall = juce::jlimit (0.0f, 100.0f, hallPercent);
    depth = juce::jlimit (0.0f, 100.0f, depthPercent);
    mix = juce::jlimit (0.0f, 100.0f, mixPercent);

    const auto cornerNorm = corner * 0.01f;
    const auto depthNorm = depth * 0.01f;
    const auto hallNorm = hall * 0.01f;

    const auto cutoff = mapCornerToCutoffHz (cornerNorm) * mapDepthToCutoffMultiplier (depthNorm);
    smoothedCutoff.setTargetValue (juce::jlimit (kMinCutoffHz, kMaxCutoffHz, cutoff));
    smoothedMidGainDb.setTargetValue (lerp (kMaxMidBoostDb, 0.0f, std::pow (cornerNorm, 0.7f)));
    smoothedReverbWet.setTargetValue (lerp (kMaxReverbWet, kMinReverbWet, cornerNorm));
    smoothedCompThreshold.setTargetValue (lerp (-24.0f, 0.0f, cornerNorm));
    smoothedDepthGain.setTargetValue (lerp (kMinDepthGain, 1.0f, 1.0f - depthNorm));
    smoothedRoomSize.setTargetValue (lerp (0.18f, 0.72f, hallNorm));
    smoothedDamping.setTargetValue (lerp (0.82f, 0.45f, hallNorm));
    smoothedReverbWidth.setTargetValue (lerp (0.55f, 0.85f, hallNorm));
    smoothedPredelaySamples.setTargetValue (lerp (kMinPredelayMs, kMaxPredelayMs, hallNorm)
                                            * static_cast<float> (sampleRate) * 0.001f);
    smoothedHallHfCutoff.setTargetValue (lerp (kMaxHallHfCutoffHz, kMinHallHfCutoffHz, hallNorm));
    smoothedMix.setTargetValue (mix * 0.01f);

    updateCoefficients();
    updateReverbParameters();
}

void BackroomsProcessor::advanceSmoothedWetTargets (int numSteps)
{
    for (int i = 0; i < numSteps; ++i)
    {
        smoothedCutoff.getNextValue();
        smoothedMidGainDb.getNextValue();
        smoothedReverbWet.getNextValue();
        smoothedCompThreshold.getNextValue();
        smoothedDepthGain.getNextValue();
        smoothedRoomSize.getNextValue();
        smoothedDamping.getNextValue();
        smoothedReverbWidth.getNextValue();
        smoothedPredelaySamples.getNextValue();
        smoothedHallHfCutoff.getNextValue();
    }
}

void BackroomsProcessor::updateCoefficients()
{
    const auto cutoff = smoothedCutoff.getCurrentValue();
    const auto lpfCoeffs = juce::dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, cutoff);
    *lowPass.state = *lpfCoeffs;
    *lowPass2.state = *lpfCoeffs;
    *midPeak.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (
        sampleRate, kMidPeakHz, kMidPeakQ, juce::Decibels::decibelsToGain (smoothedMidGainDb.getCurrentValue()));
    *hallHighShelf.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass (
        sampleRate, smoothedHallHfCutoff.getCurrentValue());
}

void BackroomsProcessor::updateReverbParameters()
{
    juce::dsp::Reverb::Parameters reverbParams;
    const auto reverbWet = smoothedReverbWet.getCurrentValue();
    reverbParams.roomSize = smoothedRoomSize.getCurrentValue();
    reverbParams.damping = smoothedDamping.getCurrentValue();
    reverbParams.wetLevel = reverbWet;
    reverbParams.dryLevel = 1.0f - reverbWet;
    reverbParams.width = smoothedReverbWidth.getCurrentValue();
    reverbParams.freezeMode = 0.0f;
    reverb.setParameters (reverbParams);
}

void BackroomsProcessor::applyPredelay (juce::dsp::AudioBlock<float>& block)
{
    const auto delaySamples = smoothedPredelaySamples.getCurrentValue();
    predelay.setDelay (delaySamples);

    for (size_t ch = 0; ch < block.getNumChannels(); ++ch)
    {
        auto* data = block.getChannelPointer (ch);
        for (size_t i = 0; i < block.getNumSamples(); ++i)
        {
            predelay.pushSample (static_cast<int> (ch), data[i]);
            data[i] = predelay.popSample (static_cast<int> (ch));
        }
    }
}

void BackroomsProcessor::process (juce::AudioBuffer<float>& buffer)
{
    const auto numSamples = buffer.getNumSamples();
    if (numSamples == 0)
        return;

    wetBuffer.makeCopyOf (buffer, true);

    for (int offset = 0; offset < numSamples; offset += kSmoothUpdateInterval)
    {
        const auto chunkSize = juce::jmin (kSmoothUpdateInterval, numSamples - offset);

        advanceSmoothedWetTargets (chunkSize);
        updateCoefficients();
        updateReverbParameters();
        compressor.setThreshold (smoothedCompThreshold.getCurrentValue());

        juce::dsp::AudioBlock<float> fullBlock (wetBuffer);
        auto subBlock = fullBlock.getSubBlock (static_cast<size_t> (offset), static_cast<size_t> (chunkSize));
        juce::dsp::ProcessContextReplacing<float> wetContext (subBlock);

        highPass.process (wetContext);
        lowPass.process (wetContext);
        lowPass2.process (wetContext);
        midPeak.process (wetContext);
        applyPredelay (subBlock);
        reverb.process (wetContext);
        hallHighShelf.process (wetContext);
        compressor.process (wetContext);

        subBlock.multiplyBy (smoothedDepthGain.getCurrentValue());
    }

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* dry = buffer.getWritePointer (ch);
        const auto* wet = wetBuffer.getReadPointer (ch);

        for (int i = 0; i < numSamples; ++i)
        {
            const auto wetAmount = smoothedMix.getNextValue();
            const auto dryAmount = 1.0f - wetAmount;
            dry[i] = dry[i] * dryAmount + wet[i] * wetAmount;
        }
    }
}
