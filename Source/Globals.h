/*
  ==============================================================================

    Globals.h
    Created: 9 Feb 2024 10:42:05am
    Author:  Klaas Klee

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>



const int numOfLayers = 5;
const int layerHeight = 200;
const float globalSampleRate = 48000.0;

float globalVolume;

// if you add a Blendmode
// add BlendModeName in LayerControlComponent.h
// change last Blendmode in LayerControlComponent.cpp line 20

enum BlendModes
{
    Normal,
    Subdivide,
    Blend,
    Multiply,
    Duck
};


//==============================================================================
//Styling COLORS
//==============================================================================

namespace GlobalColors {

    const juce::Colour black = juce::Colour(0xff000000);
    const juce::Colour white = juce::Colour(0xffffffff);

    const juce::Colour brightText = juce::Colour(0xffffffff);
    const juce::Colour darkText = juce::Colour(0xff000000);

    const juce::Colour BG = juce::Colour(0xffCFCFCF);

    const juce::Colour controlBarBG = juce::Colour(0xff9E9E9E);

    const juce::Colour toolButtonActive = juce::Colour(0xffbd3a31);
    const juce::Colour toolButtonNotActive = juce::Colour(0xff000000);

    const juce::Colour controlBarPlayButtonActive = juce::Colour(0xff47FF37);
    //const juce::Colour controlBarPlayButtonNotActive = juce::Colour(0xff47FF37);
    //const juce::Colour controlBarPauseButtonActive = juce::Colour(0xffA16C2E);
    const juce::Colour controlBarPauseButtonNotActive = juce::Colour(0xffA16C2E);
    //const juce::Colour controlBarStopButtonActive = juce::Colour(0xff824242);
    const juce::Colour controlBarStopButtonNotActive = juce::Colour(0xff824242);

    const juce::Colour controlBarTimeStampBG = juce::Colour(0xff8D8D8D);

}

namespace GlobalFonts {
const juce::Font titleFont = juce::Font("Eastman Roman Alt", "Regular", 40.0f);
const juce::Font mainFont = juce::Font("Eastman Roman Alt", "Regular", 20.0f);
}

//==============================================================================
//Styling - LOOKANDFEEL
//==============================================================================

class LookAndFeel001 : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider (juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider)
    {
        float diameter = fmin(width, height);
        float radius = diameter/2;
        float centerX = x + width/2;
        float centerY = y + height/2;
        float angle = rotaryStartAngle + (sliderPos* (rotaryEndAngle - rotaryStartAngle));
        juce::Rectangle<float> dialArea (centerX-radius,centerY-radius,diameter,diameter);

        g.setColour(GlobalColors::black);
        g.fillEllipse(dialArea);
        
        g.setColour(GlobalColors::white);
        juce::Path dialHandle;
        dialHandle.addEllipse(0-radius*0.28/2, -radius*0.82, radius*0.28, radius*0.26);
        g.fillPath(dialHandle, juce::AffineTransform::rotation(angle).translated(centerX, centerY));
    }
    void drawToggleButton (juce::Graphics &g, juce::ToggleButton &button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
    {
        auto fontSize = fmin (15.0f, (float) button.getHeight() * 0.75f);
        auto tickWidth = fontSize * 1.1f;

        drawTickBox (g, button, 4.0f, ((float) button.getHeight() - tickWidth) * 0.5f,
                     tickWidth, tickWidth,
                     button.getToggleState(),
                     button.isEnabled(),
                     shouldDrawButtonAsHighlighted,
                     shouldDrawButtonAsDown);

        g.setColour (button.findColour (juce::ToggleButton::textColourId));
        g.setFont (fontSize);

        if (! button.isEnabled())
            g.setOpacity (0.5f);

        g.drawFittedText (button.getButtonText(),
                          button.getLocalBounds().withTrimmedLeft (juce::roundToInt (tickWidth) + 10)
                                                 .withTrimmedRight (2),
                          juce::Justification::centredLeft, 10);
    }
    void drawTickBox (juce::Graphics& g, juce::Component& component,
                                      float x, float y, float w, float h,
                                      const bool ticked,
                                      [[maybe_unused]] const bool isEnabled,
                                      [[maybe_unused]] const bool shouldDrawButtonAsHighlighted,
                                      [[maybe_unused]] const bool shouldDrawButtonAsDown)
    {
        juce::Rectangle<float> tickBounds (x, y, w, h);

        g.setColour (GlobalColors::toolButtonNotActive);
        g.fillRect (tickBounds);

        if (ticked)
        {
            g.setColour (GlobalColors::toolButtonActive);
            g.fillRect (tickBounds);
        }
    }
};
