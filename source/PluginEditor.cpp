#include "PluginEditor.h"

#include "BinaryData.h"

namespace
{
juce::Image loadEmbeddedPng (const char* data, int size)
{
    return juce::ImageFileFormat::loadFrom (data, static_cast<size_t> (size));
}
} // namespace

BackroomsMufflerAudioProcessorEditor::BackroomsMufflerAudioProcessorEditor (
    BackroomsMufflerAudioProcessor& p)
    : AudioProcessorEditor (&p),
      processor (p)
{
    setLookAndFeel (&lookAndFeel);
    setSize (480, 320);

    wallpaper.setWallpaperImage (loadEmbeddedPng (BinaryData::wallpaper_tile_png,
                                                  BinaryData::wallpaper_tile_pngSize));
    wallpaper.setCarpetImage (loadEmbeddedPng (BinaryData::carpet_tile_png,
                                               BinaryData::carpet_tile_pngSize));
    addAndMakeVisible (wallpaper);

    configureSlider (cornerSlider, "Corner");
    configureSlider (hallSlider, "Hall");
    configureSlider (depthSlider, "Depth");
    configureSlider (mixSlider, "Mix");

    auto setupLabel = [] (juce::Label& label, const juce::String& text)
    {
        label.setText (text, juce::dontSendNotification);
        label.setJustificationType (juce::Justification::centred);
        label.setInterceptsMouseClicks (false, false);
    };

    setupLabel (cornerLabel, "Corner");
    setupLabel (hallLabel, "Hall");
    setupLabel (depthLabel, "Depth");
    setupLabel (mixLabel, "Mix");

    titleLabel.setText ("BACKROOMS MUFFLER", juce::dontSendNotification);
    titleLabel.setJustificationType (juce::Justification::centred);
    titleLabel.setFont (juce::FontOptions (15.0f).withStyle ("Bold"));
    titleLabel.setColour (juce::Label::textColourId, BackroomsColours::labelText);
    titleLabel.setInterceptsMouseClicks (false, false);

    addAndMakeVisible (cornerLabel);
    addAndMakeVisible (hallLabel);
    addAndMakeVisible (depthLabel);
    addAndMakeVisible (mixLabel);
    addAndMakeVisible (titleLabel);

    auto& apvts = processor.getApvts();
    cornerAttachment = std::make_unique<SliderAttachment> (apvts, "corner", cornerSlider);
    hallAttachment = std::make_unique<SliderAttachment> (apvts, "hall", hallSlider);
    depthAttachment = std::make_unique<SliderAttachment> (apvts, "depth", depthSlider);
    mixAttachment = std::make_unique<SliderAttachment> (apvts, "mix", mixSlider);
}

void BackroomsMufflerAudioProcessorEditor::configureSlider (juce::Slider& slider,
                                                            const juce::String& name)
{
    juce::ignoreUnused (name);
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 56, 18);
    slider.setPopupDisplayEnabled (true, false, this);
    addAndMakeVisible (slider);
}

void BackroomsMufflerAudioProcessorEditor::paint (juce::Graphics& g)
{
    juce::ignoreUnused (g);
}

void BackroomsMufflerAudioProcessorEditor::resized()
{
    wallpaper.setBounds (getLocalBounds());

    auto layoutArea = getLocalBounds().reduced (16);
    const auto controlArea = layoutArea.withTrimmedTop (48).withTrimmedBottom (72);
    const auto knobWidth = controlArea.getWidth() / 4;

    auto placeKnob = [&] (juce::Slider& slider, juce::Label& label, int index)
    {
        auto slot = controlArea.withWidth (knobWidth).withX (controlArea.getX() + index * knobWidth);
        label.setBounds (slot.removeFromTop (20));
        slider.setBounds (slot.reduced (4));
    };

    placeKnob (cornerSlider, cornerLabel, 0);
    placeKnob (hallSlider, hallLabel, 1);
    placeKnob (depthSlider, depthLabel, 2);
    placeKnob (mixSlider, mixLabel, 3);

    titleLabel.setBounds (layoutArea.removeFromBottom (28));
}
