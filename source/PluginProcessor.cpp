#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace ParamIDs
{
constexpr auto corner = "corner";
constexpr auto hall = "hall";
constexpr auto depth = "depth";
constexpr auto mix = "mix";
constexpr auto muffle = "muffle";
} // namespace ParamIDs

juce::AudioProcessorValueTreeState::ParameterLayout BackroomsMufflerAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::corner, 1 },
        "Corner",
        juce::NormalisableRange<float> { 0.0f, 100.0f, 0.1f },
        35.0f,
        juce::AudioParameterFloatAttributes().withLabel ("%")));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::hall, 1 },
        "Hall",
        juce::NormalisableRange<float> { 0.0f, 100.0f, 0.1f },
        40.0f,
        juce::AudioParameterFloatAttributes().withLabel ("%")));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::depth, 1 },
        "Depth",
        juce::NormalisableRange<float> { 0.0f, 100.0f, 0.1f },
        30.0f,
        juce::AudioParameterFloatAttributes().withLabel ("%")));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::mix, 1 },
        "Mix",
        juce::NormalisableRange<float> { 0.0f, 100.0f, 0.1f },
        50.0f,
        juce::AudioParameterFloatAttributes().withLabel ("%")));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::muffle, 1 },
        "Muffle",
        juce::NormalisableRange<float> { 0.0f, 100.0f, 0.1f },
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel ("%")));

    return layout;
}

BackroomsMufflerAudioProcessor::BackroomsMufflerAudioProcessor()
    : AudioProcessor (BusesProperties()
                          .withInput ("Input", juce::AudioChannelSet::stereo(), true)
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

void BackroomsMufflerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32> (samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32> (getTotalNumOutputChannels());

    backroomsProcessor.prepare (spec);
}

void BackroomsMufflerAudioProcessor::releaseResources()
{
    backroomsProcessor.reset();
}

void BackroomsMufflerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                                   juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    for (auto i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    backroomsProcessor.setParameters (
        apvts.getRawParameterValue (ParamIDs::corner)->load(),
        apvts.getRawParameterValue (ParamIDs::hall)->load(),
        apvts.getRawParameterValue (ParamIDs::depth)->load(),
        apvts.getRawParameterValue (ParamIDs::mix)->load(),
        apvts.getRawParameterValue (ParamIDs::muffle)->load());

    backroomsProcessor.process (buffer);
}

juce::AudioProcessorEditor* BackroomsMufflerAudioProcessor::createEditor()
{
    return new BackroomsMufflerAudioProcessorEditor (*this);
}

void BackroomsMufflerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto state = apvts.copyState().createXml())
        copyXmlToBinary (*state, destData);
}

void BackroomsMufflerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
        apvts.replaceState (juce::ValueTree::fromXml (*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BackroomsMufflerAudioProcessor();
}
