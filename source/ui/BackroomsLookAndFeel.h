#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace BackroomsColours
{
inline const juce::Colour wallpaperBase { 0xffc9a83a };
inline const juce::Colour wallpaperHighlight { 0xffe2c45a };
inline const juce::Colour wallpaperSeam { 0xffa8872e };
inline const juce::Colour carpetBase { 0xff8f7d62 };
inline const juce::Colour carpetSpeckle { 0xff6e5f4c };
inline const juce::Colour ceilingPanel { 0xffe4e0d4 };
inline const juce::Colour ceilingGrid { 0xff6e6a60 };
inline const juce::Colour fluorescent { 0xfff8faf4 };
inline const juce::Colour fluorescentTint { 0xffd4e6cc };
inline const juce::Colour labelText { 0xff2a2418 };
inline const juce::Colour knobFill { 0xfff5f1e8 };
inline const juce::Colour knobAccent { 0xffb8921e };
inline const juce::Colour controlPlate { 0xfff0ebe0 };
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
        setColour (juce::Slider::textBoxBackgroundColourId, BackroomsColours::knobFill);
        setColour (juce::Slider::textBoxOutlineColourId, BackroomsColours::ceilingGrid);
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

        g.setColour (BackroomsColours::ceilingGrid.withAlpha (0.45f));
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

    void drawLabel (juce::Graphics& g, juce::Label& label) override
    {
        const auto textArea = label.getLocalBounds().toFloat().reduced (2.0f, 1.0f);
        g.setColour (BackroomsColours::knobFill.withAlpha (0.92f));
        g.fillRoundedRectangle (textArea, 4.0f);
        g.setColour (BackroomsColours::ceilingGrid.withAlpha (0.35f));
        g.drawRoundedRectangle (textArea, 4.0f, 1.0f);

        g.setColour (label.findColour (juce::Label::textColourId));
        g.setFont (getLabelFont (label));
        g.drawFittedText (label.getText(), textArea.toNearestInt(), label.getJustificationType(), 1);
    }

    juce::Font getLabelFont (juce::Label&) override
    {
        return juce::FontOptions (14.0f).withStyle ("Bold");
    }

    juce::Font getSliderPopupFont (juce::Slider&) override
    {
        return juce::FontOptions (14.0f).withStyle ("Bold");
    }
};

class WallpaperComponent : public juce::Component
{
public:
    void setWallpaperImage (const juce::Image& image) { wallpaper = image; repaint(); }
    void setCarpetImage (const juce::Image& image) { carpet = image; repaint(); }
    void setCeilingImage (const juce::Image& image) { ceiling = image; repaint(); }

    void setControlPlateBounds (juce::Rectangle<int> bounds)
    {
        controlPlateBounds = bounds;
        repaint();
    }

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

        if (controlPlateBounds.getWidth() > 0)
        {
            g.setColour (juce::Colours::black.withAlpha (0.15f));
            g.fillRoundedRectangle (controlPlateBounds.toFloat(), 8.0f);
            g.setColour (BackroomsColours::controlPlate.withAlpha (0.88f));
            g.fillRoundedRectangle (controlPlateBounds.toFloat(), 8.0f);
            g.setColour (BackroomsColours::ceilingGrid.withAlpha (0.4f));
            g.drawRoundedRectangle (controlPlateBounds.toFloat().reduced (0.5f), 8.0f, 1.0f);
        }

        const auto carpetArea = bounds.removeFromBottom (carpetHeight);
        if (carpet.isValid())
            paintTiledImage (g, carpetArea, carpet);
        else
            paintProceduralCarpet (g, carpetArea);
    }

private:
    juce::Image wallpaper;
    juce::Image carpet;
    juce::Image ceiling;
    juce::Rectangle<int> controlPlateBounds;

    static void paintTiledImage (juce::Graphics& g, juce::Rectangle<int> area, const juce::Image& image)
    {
        const auto tileW = image.getWidth();
        const auto tileH = image.getHeight();

        for (int y = area.getY(); y < area.getBottom(); y += tileH)
        {
            for (int x = area.getX(); x < area.getRight(); x += tileW)
                g.drawImageAt (image, x, y);
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

    void paintCeiling (juce::Graphics& g, juce::Rectangle<int> area)
    {
        if (ceiling.isValid())
        {
            paintTiledImage (g, area, ceiling);

            g.setColour (BackroomsColours::fluorescent.withAlpha (0.85f));
            const auto light = area.reduced (area.getWidth() / 4, 10).withHeight (8);
            g.fillRoundedRectangle (light.toFloat(), 2.0f);
            return;
        }

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
