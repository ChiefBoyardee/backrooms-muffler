#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace BackroomsColours
{
inline const juce::Colour wallpaperBase { 0xffd4b84a };
inline const juce::Colour wallpaperHighlight { 0xffe8d174 };
inline const juce::Colour wallpaperSeam { 0xffb89a38 };
inline const juce::Colour carpetBase { 0xffb8a88a };
inline const juce::Colour carpetSpeckle { 0xff9a8b72 };
inline const juce::Colour ceilingPanel { 0xffe8e4d9 };
inline const juce::Colour ceilingGrid { 0xff8a8578 };
inline const juce::Colour fluorescent { 0xfff5f8f2 };
inline const juce::Colour fluorescentTint { 0xffd8e8d0 };
inline const juce::Colour labelText { 0xff3d3428 };
inline const juce::Colour knobFill { 0xfff0ede6 };
inline const juce::Colour knobAccent { 0xffc9a227 };
} // namespace BackroomsColours

class BackroomsLookAndFeel : public juce::LookAndFeel_V4
{
public:
    BackroomsLookAndFeel()
    {
        setColour (juce::Slider::rotarySliderFillColourId, BackroomsColours::knobAccent);
        setColour (juce::Slider::rotarySliderOutlineColourId, BackroomsColours::ceilingGrid);
        setColour (juce::Slider::thumbColourId, BackroomsColours::fluorescent);
        setColour (juce::Slider::textBoxTextColourId, BackroomsColours::labelText);
        setColour (juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
        setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        setColour (juce::Label::textColourId, BackroomsColours::labelText);
    }

    void drawRotarySlider (juce::Graphics& g,
                           int x,
                           int y,
                           int width,
                           int height,
                           float sliderPosProportional,
                           float rotaryStartAngle,
                           float rotaryEndAngle,
                           juce::Slider& slider) override
    {
        juce::ignoreUnused (slider);

        const auto bounds = juce::Rectangle<float> (static_cast<float> (x),
                                                    static_cast<float> (y),
                                                    static_cast<float> (width),
                                                    static_cast<float> (height))
                                .reduced (8.0f);
        const auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.5f;
        const auto centre = bounds.getCentre();
        const auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        g.setColour (BackroomsColours::knobFill);
        g.fillEllipse (centre.x - radius, centre.y - radius, radius * 2.0f, radius * 2.0f);

        g.setColour (BackroomsColours::ceilingGrid.withAlpha (0.35f));
        g.drawEllipse (centre.x - radius, centre.y - radius, radius * 2.0f, radius * 2.0f, 1.5f);

        juce::Path arc;
        arc.addCentredArc (centre.x, centre.y, radius * 0.78f, radius * 0.78f, 0.0f,
                           rotaryStartAngle, angle, true);
        g.setColour (BackroomsColours::knobAccent);
        g.strokePath (arc, juce::PathStrokeType (3.0f));

        juce::Path pointer;
        pointer.addRectangle (-1.5f, -radius * 0.72f, 3.0f, radius * 0.28f);
        g.setColour (BackroomsColours::labelText);
        g.fillPath (pointer, juce::AffineTransform::rotation (angle).translated (centre.x, centre.y));
    }

    juce::Font getLabelFont (juce::Label&) override
    {
        return juce::FontOptions (13.0f).withStyle ("Bold");
    }
};

class WallpaperComponent : public juce::Component
{
public:
    void setWallpaperImage (const juce::Image& image) { wallpaper = image; repaint(); }
    void setCarpetImage (const juce::Image& image) { carpet = image; repaint(); }

    void paint (juce::Graphics& g) override
    {
        auto bounds = getLocalBounds();
        const auto ceilingHeight = 36;
        const auto carpetHeight = 40;
        const auto wallArea = bounds.withTrimmedTop (ceilingHeight).withTrimmedBottom (carpetHeight);

        paintCeiling (g, bounds.removeFromTop (ceilingHeight));

        if (wallpaper.isValid())
            paintTiledImage (g, wallArea, wallpaper);
        else
            paintProceduralWallpaper (g, wallArea);

        const auto carpetArea = bounds.removeFromBottom (carpetHeight);
        if (carpet.isValid())
            paintTiledImage (g, carpetArea, carpet);
        else
            paintProceduralCarpet (g, carpetArea);
    }

private:
    juce::Image wallpaper;
    juce::Image carpet;

    static void paintTiledImage (juce::Graphics& g, juce::Rectangle<int> area, const juce::Image& image)
    {
        const auto tileW = image.getWidth();
        const auto tileH = image.getHeight();

        for (int y = area.getY(); y < area.getBottom(); y += tileH)
        {
            for (int x = area.getX(); x < area.getRight(); x += tileW)
            {
                g.drawImageAt (image, x, y);
            }
        }
    }

    static void paintProceduralWallpaper (juce::Graphics& g, juce::Rectangle<int> area)
    {
        g.setGradientFill (juce::ColourGradient (
            BackroomsColours::wallpaperHighlight,
            static_cast<float> (area.getX()),
            static_cast<float> (area.getY()),
            BackroomsColours::wallpaperBase,
            static_cast<float> (area.getRight()),
            static_cast<float> (area.getBottom()),
            false));
        g.fillRect (area);

        g.setColour (BackroomsColours::wallpaperSeam.withAlpha (0.35f));
        for (int x = area.getX() + 64; x < area.getRight(); x += 128)
            g.fillRect (x, area.getY(), 2, area.getHeight());
    }

    static void paintProceduralCarpet (juce::Graphics& g, juce::Rectangle<int> area)
    {
        g.setColour (BackroomsColours::carpetBase);
        g.fillRect (area);

        juce::Random rng (42);
        for (int i = 0; i < area.getWidth() * area.getHeight() / 120; ++i)
        {
            const auto px = area.getX() + rng.nextInt (area.getWidth());
            const auto py = area.getY() + rng.nextInt (area.getHeight());
            g.setColour (BackroomsColours::carpetSpeckle.withAlpha (0.25f));
            g.fillRect (px, py, 2, 2);
        }
    }

    static void paintCeiling (juce::Graphics& g, juce::Rectangle<int> area)
    {
        g.setColour (BackroomsColours::ceilingPanel);
        g.fillRect (area);

        g.setColour (BackroomsColours::ceilingGrid);
        for (int x = area.getX(); x < area.getRight(); x += 48)
            g.drawVerticalLine (x, static_cast<float> (area.getY()), static_cast<float> (area.getBottom()));

        g.setColour (BackroomsColours::fluorescent);
        const auto light = area.reduced (area.getWidth() / 4, 10).withHeight (8);
        g.fillRoundedRectangle (light.toFloat(), 2.0f);

        g.setColour (BackroomsColours::fluorescentTint.withAlpha (0.5f));
        g.fillRoundedRectangle (light.translated (0, 10).toFloat(), 2.0f);
    }
};
