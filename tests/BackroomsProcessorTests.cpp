#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "dsp/BackroomsProcessor.h"

namespace
{
juce::AudioBuffer<float> makeNoise (int numSamples = 2048)
{
    juce::AudioBuffer<float> buffer (2, numSamples);
    juce::Random rng (7);
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* data = buffer.getWritePointer (ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
            data[i] = rng.nextFloat() * 2.0f - 1.0f;
    }
    return buffer;
}

double measureHighFreqEnergy (const juce::AudioBuffer<float>& buf)
{
    double energy = 0.0;
    for (int ch = 0; ch < buf.getNumChannels(); ++ch)
    {
        const auto* data = buf.getReadPointer (ch);
        for (int i = 1; i < buf.getNumSamples(); ++i)
        {
            const auto diff = data[i] - data[i - 1];
            energy += diff * diff;
        }
    }
    return energy;
}


double measureDifferenceRms (const juce::AudioBuffer<float>& a, const juce::AudioBuffer<float>& b)
{
    double sum = 0.0;
    auto count = 0;
    for (int ch = 0; ch < a.getNumChannels(); ++ch)
    {
        const auto* aData = a.getReadPointer (ch);
        const auto* bData = b.getReadPointer (ch);
        for (int i = 0; i < a.getNumSamples(); ++i)
        {
            const auto diff = static_cast<double> (aData[i]) - static_cast<double> (bData[i]);
            sum += diff * diff;
            ++count;
        }
    }
    return std::sqrt (sum / static_cast<double> (count));
}
} // namespace

TEST_CASE ("Corner macro maps to expected cutoff range", "[dsp]")
{
    REQUIRE (BackroomsProcessor::mapCornerToCutoffHz (0.0f) == Catch::Approx (350.0f).margin (1.0f));
    REQUIRE (BackroomsProcessor::mapCornerToCutoffHz (1.0f) == Catch::Approx (12000.0f).margin (1.0f));
    REQUIRE (BackroomsProcessor::mapCornerToCutoffHz (0.5f) == Catch::Approx (6175.0f).margin (50.0f));
}

TEST_CASE ("Depth reduces effective cutoff", "[dsp]")
{
    REQUIRE (BackroomsProcessor::mapDepthToCutoffMultiplier (0.0f) == Catch::Approx (1.0f));
    REQUIRE (BackroomsProcessor::mapDepthToCutoffMultiplier (1.0f) == Catch::Approx (0.45f).margin (0.01f));
}

TEST_CASE ("Processor prepares and processes silence without NaNs", "[dsp]")
{
    BackroomsProcessor processor;
    juce::dsp::ProcessSpec spec { 48000.0, 512, 2 };
    processor.prepare (spec);
    processor.setParameters (10.0f, 50.0f, 80.0f, 100.0f);

    juce::AudioBuffer<float> buffer (2, 512);
    buffer.clear();

    processor.process (buffer);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        const auto* data = buffer.getReadPointer (ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
            REQUIRE_FALSE (std::isnan (data[i]));
    }
}

TEST_CASE ("High corner value yields brighter output than low corner on noise", "[dsp]")
{
    BackroomsProcessor muffled;
    BackroomsProcessor clear;
    juce::dsp::ProcessSpec spec { 48000.0, 2048, 2 };
    muffled.prepare (spec);
    clear.prepare (spec);

    auto muffledBuf = makeNoise();
    auto clearBuf = makeNoise();
    muffledBuf.makeCopyOf (clearBuf);

    muffled.setParameters (5.0f, 40.0f, 20.0f, 100.0f);
    clear.setParameters (95.0f, 40.0f, 20.0f, 100.0f);

    muffled.process (muffledBuf);
    clear.process (clearBuf);

    REQUIRE (measureHighFreqEnergy (clearBuf) > measureHighFreqEnergy (muffledBuf));
}

TEST_CASE ("Mix at 0 percent leaves input unchanged", "[dsp]")
{
    BackroomsProcessor processor;
    juce::dsp::ProcessSpec spec { 48000.0, 2048, 2 };
    processor.prepare (spec);

    auto input = makeNoise();
    auto output = makeNoise();
    output.makeCopyOf (input);

    processor.setParameters (20.0f, 50.0f, 40.0f, 0.0f);
    processor.process (output);

    REQUIRE (measureDifferenceRms (input, output) == Catch::Approx (0.0).margin (1.0e-5));
}

TEST_CASE ("Mix at 100 percent engages the wet path", "[dsp]")
{
    BackroomsProcessor processor;
    juce::dsp::ProcessSpec spec { 48000.0, 2048, 2 };
    processor.prepare (spec);

    auto input = makeNoise();
    auto output = makeNoise();
    output.makeCopyOf (input);

    processor.setParameters (20.0f, 50.0f, 40.0f, 100.0f);
    processor.process (output);

    REQUIRE (measureDifferenceRms (input, output) > 0.01);
}

TEST_CASE ("Mix increases wet contribution monotonically", "[dsp]")
{
    BackroomsProcessor dryMix;
    BackroomsProcessor midMix;
    BackroomsProcessor wetMix;
    juce::dsp::ProcessSpec spec { 48000.0, 2048, 2 };

    dryMix.prepare (spec);
    midMix.prepare (spec);
    wetMix.prepare (spec);

    auto input = makeNoise();
    auto dryBuf = makeNoise();
    auto midBuf = makeNoise();
    auto wetBuf = makeNoise();
    dryBuf.makeCopyOf (input);
    midBuf.makeCopyOf (input);
    wetBuf.makeCopyOf (input);

    dryMix.setParameters (25.0f, 60.0f, 50.0f, 0.0f);
    midMix.setParameters (25.0f, 60.0f, 50.0f, 50.0f);
    wetMix.setParameters (25.0f, 60.0f, 50.0f, 100.0f);

    dryMix.process (dryBuf);
    midMix.process (midBuf);
    wetMix.process (wetBuf);

    const auto dryDeviation = measureDifferenceRms (input, dryBuf);
    const auto midDeviation = measureDifferenceRms (input, midBuf);
    const auto wetDeviation = measureDifferenceRms (input, wetBuf);

    REQUIRE (wetDeviation > midDeviation);
    REQUIRE (midDeviation > dryDeviation);
    REQUIRE (dryDeviation == Catch::Approx (0.0).margin (1.0e-5));
}
