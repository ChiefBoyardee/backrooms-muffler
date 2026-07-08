#pragma once

#include <juce_dsp/juce_dsp.h>

class BackroomsProcessor
{
public:
    void prepare (const juce::dsp::ProcessSpec& spec);
    void reset();
    void setParameters (float cornerPercent, float hallPercent, float depthPercent, float mixPercent);
    void process (juce::AudioBuffer<float>& buffer);

    static float mapCornerToCutoffHz (float cornerNorm);
    static float mapDepthToCutoffMultiplier (float depthNorm);

private:
    static constexpr int kSmoothUpdateInterval = 16;

    void advanceSmoothedWetTargets (int numSteps);
    void updateCoefficients();
    void updateReverbParameters();

    double sampleRate = 44100.0;
    int numChannels = 2;

    float corner = 35.0f;
    float hall = 40.0f;
    float depth = 30.0f;
    float mix = 50.0f;

    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedCutoff;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedMidGainDb;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedReverbWet;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedCompThreshold;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedDepthGain;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedRoomSize;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedDamping;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedMix;

    using Filter = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
                                                  juce::dsp::IIR::Coefficients<float>>;

    Filter highPass;
    Filter lowPass;
    Filter midPeak;

    juce::dsp::Reverb reverb;
    juce::dsp::Compressor<float> compressor;

    juce::AudioBuffer<float> wetBuffer;
};
