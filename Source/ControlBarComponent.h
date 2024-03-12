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
#include "MainComponent.h"

//==============================================================================

class ControlBarComponent  : public juce::Component,
                             public juce::Slider::Listener
{
public:
    ControlBarComponent();
    ~ControlBarComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged (juce::Slider* slider) override;
    
    //Volume
    juce::Slider sVolumeDial;
    //State Buttons
    juce::ToggleButton bPlay;
    juce::ToggleButton bPause;
    juce::ToggleButton bStop;

    juce::ToggleButton bLoop;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlBarComponent)
    
    void bPlayClicked();
    void bStopClicked();
    void bPauseClicked();
    
    void bLoopClicked();
    
    //items

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

