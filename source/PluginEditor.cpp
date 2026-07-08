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
    setSize (560, 360);

    wallpaper.setWallpaperImage (loadEmbeddedPng (BinaryData::wallpaper_tile_png,
                                                  BinaryData::wallpaper_tile_pngSize));
    wallpaper.setCarpetImage (loadEmbeddedPng (BinaryData::carpet_tile_png,
                                               BinaryData::carpet_tile_pngSize));
    wallpaper.setCeilingImage (loadEmbeddedPng (BinaryData::ceiling_tile_png,
                                                BinaryData::ceiling_tile_pngSize));
    addAndMakeVisible (wallpaper);

    configureSlider (cornerSlider, "Corner");
    configureSlider (hallSlider, "Hall");
    configureSlider (depthSlider, "Depth");
    configureSlider (mixSlider, "Mix");
    configureSlider (muffleSlider, "Muffle");

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
    setupLabel (muffleLabel, "Muffle");

    headerLabel.setText ("BACKROOMS MUFFLER", juce::dontSendNotification);
    headerLabel.setComponentID ("header");
    headerLabel.setJustificationType (juce::Justification::centredLeft);
    headerLabel.setFont (juce::FontOptions (13.0f).withStyle ("Bold"));
    headerLabel.setColour (juce::Label::textColourId, BackroomsColours::headerText);
    headerLabel.setInterceptsMouseClicks (false, false);

    addAndMakeVisible (cornerLabel);
    addAndMakeVisible (hallLabel);
    addAndMakeVisible (depthLabel);
    addAndMakeVisible (mixLabel);
    addAndMakeVisible (muffleLabel);
    addAndMakeVisible (headerLabel);

    auto& apvts = processor.getApvts();
    cornerAttachment = std::make_unique<SliderAttachment> (apvts, "corner", cornerSlider);
    hallAttachment = std::make_unique<SliderAttachment> (apvts, "hall", hallSlider);
    depthAttachment = std::make_unique<SliderAttachment> (apvts, "depth", depthSlider);
    mixAttachment = std::make_unique<SliderAttachment> (apvts, "mix", mixSlider);
    muffleAttachment = std::make_unique<SliderAttachment> (apvts, "muffle", muffleSlider);
}

void BackroomsMufflerAudioProcessorEditor::configureSlider (juce::Slider& slider,
                                                            const juce::String& name)
{
    slider.setName (name);
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 64, 22);
    slider.setColour (juce::Slider::textBoxTextColourId, BackroomsColours::valueText);
    slider.setColour (juce::Slider::textBoxBackgroundColourId, BackroomsColours::valueBackground);
    slider.setColour (juce::Slider::textBoxOutlineColourId, BackroomsColours::valueOutline);
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

    auto layoutArea = getLocalBounds().reduced (14);
    headerLabel.setBounds (layoutArea.removeFromTop (40).reduced (8, 8));

    const auto controlArea = layoutArea.withTrimmedTop (4).withTrimmedBottom (44);
    const auto knobWidth = controlArea.getWidth() / 5;

    wallpaper.setControlPlateBounds (controlArea.expanded (6, 8));

    auto placeKnob = [&] (juce::Slider& slider, juce::Label& label, int index)
    {
        auto slot = controlArea.withWidth (knobWidth).withX (controlArea.getX() + index * knobWidth);
        label.setBounds (slot.removeFromTop (24));
        slider.setBounds (slot.reduced (2));
    };

    placeKnob (cornerSlider, cornerLabel, 0);
    placeKnob (hallSlider, hallLabel, 1);
    placeKnob (depthSlider, depthLabel, 2);
    placeKnob (mixSlider, mixLabel, 3);
    placeKnob (muffleSlider, muffleLabel, 4);
}
