#pragma once

#include <juce_dsp/juce_dsp.h>

class BackroomsProcessor
{
public:
    void prepare (const juce::dsp::ProcessSpec& spec);
    void reset();
    void setParameters (float cornerPercent,
                        float hallPercent,
                        float depthPercent,
                        float mixPercent,
                        float mufflePercent);
    void process (juce::AudioBuffer<float>& buffer);

    static float mapCornerToCutoffHz (float cornerNorm);
    static float mapDepthToCutoffMultiplier (float depthNorm);

private:
    static constexpr int kSmoothUpdateInterval = 16;

    void advanceSmoothedWetTargets (int numSteps);
    void advanceSmoothedMuffleTargets (int numSteps);
    void updateCoefficients();
    void updateReverbParameters();
    void updateMuffleCoefficients();
    void applyPredelay (juce::dsp::AudioBlock<float>& block);
    void applyMuffleSoftClip (juce::dsp::AudioBlock<float>& block, float drive);
    void processMuffleChain (juce::dsp::AudioBlock<float>& block);

    double sampleRate = 44100.0;
    int numChannels = 2;

    float corner = 35.0f;
    float hall = 40.0f;
    float depth = 30.0f;
    float mix = 50.0f;
    float muffle = 0.0f;

    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedCutoff;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedMidGainDb;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedReverbWet;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedCompThreshold;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedDepthGain;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedRoomSize;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedDamping;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedReverbWidth;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedPredelaySamples;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedHallHfCutoff;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedMix;

    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedMuffle;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedMuffleCutoff;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedMuffleMidDb;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedMuffleDrive;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedMuffleThreshold;

    using Filter = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
                                                  juce::dsp::IIR::Coefficients<float>>;

    using DelayLine = juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear>;

    Filter highPass;
    Filter lowPass;
    Filter lowPass2;
    Filter midPeak;
    Filter hallHighShelf;

    Filter muffleLpf1;
    Filter muffleLpf2;
    Filter muffleMidPeak;

    DelayLine predelay;
    juce::dsp::Reverb reverb;
    juce::dsp::Compressor<float> compressor;
    juce::dsp::Compressor<float> muffleCompressor;

    juce::AudioBuffer<float> wetBuffer;
    juce::AudioBuffer<float> muffleBuffer;
};
