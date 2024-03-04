/*
  ==============================================================================

    ControlBarComponent.h
    Created: 9 Feb 2024 10:37:57am
    Author:  Klaas Klee

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Globals.h"

//==============================================================================
/*
*/

//styling - LookAndFeel
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

//==============================================================================
//==============================================================================


class ControlBarComponent  : public juce::Component,
                             public juce::Slider::Listener
{
public:
    ControlBarComponent();
    ~ControlBarComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged (juce::Slider* slider) override
    {
        if (slider == &sVolumeDial)
            std::cout << "volume: " << sVolumeDial.getValue() << std::endl;
    }
    
    //State Buttons
    juce::ToggleButton bPlay;
    juce::ToggleButton bPause;
    juce::ToggleButton bStop;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlBarComponent)
    
    void bPlayClicked();
    void bStopClicked();
    
    //items
    
    //Volume
    juce::Slider sVolumeDial;
    //Tools
    juce::ToggleButton bEins;
    juce::ToggleButton bZwei;
    juce::ToggleButton bDrei;
    juce::ToggleButton bVier;
    juce::ToggleButton bFünf;
    juce::ToggleButton bSechs;
    //MainControls
    juce::Label lAppTitle;
    //Info
    juce::Label lMeter;
    juce::Label lTimeCode;
    
    //styling - lookAndFeel
    LookAndFeel001 LookAndFeel001;
    
    
};
