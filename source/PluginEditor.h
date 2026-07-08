#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "PluginProcessor.h"
#include "ui/BackroomsLookAndFeel.h"

class BackroomsMufflerAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit BackroomsMufflerAudioProcessorEditor (BackroomsMufflerAudioProcessor&);
    ~BackroomsMufflerAudioProcessorEditor() override = default;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    void configureSlider (juce::Slider& slider, const juce::String& name);

    BackroomsMufflerAudioProcessor& processor;
    BackroomsLookAndFeel lookAndFeel;
    WallpaperComponent wallpaper;

    juce::Slider cornerSlider;
    juce::Slider hallSlider;
    juce::Slider depthSlider;
    juce::Slider mixSlider;
    juce::Slider muffleSlider;

    juce::Label cornerLabel;
    juce::Label hallLabel;
    juce::Label depthLabel;
    juce::Label mixLabel;
    juce::Label muffleLabel;
    juce::Label headerLabel;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> cornerAttachment;
    std::unique_ptr<SliderAttachment> hallAttachment;
    std::unique_ptr<SliderAttachment> depthAttachment;
    std::unique_ptr<SliderAttachment> mixAttachment;
    std::unique_ptr<SliderAttachment> muffleAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BackroomsMufflerAudioProcessorEditor)
};
