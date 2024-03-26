/*
  ==============================================================================

    ControlBarComponent.h
    Created: 9 Feb 2024 10:37:57am
    Author:  Klaas Klee

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "AudioMeterComponent.h"
#include "Globals.h"
#include "LogoComponent.h"

//==============================================================================

class ControlBarComponent  : public juce::Component,
                             public juce::Slider::Listener
{
public:
    ControlBarComponent();
    ~ControlBarComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged(juce::Slider* slider) override;
    
    //Volume
    juce::Slider sVolumeDial;
    //State Buttons
    juce::TextButton bPlay;
    juce::TextButton bPause;
    juce::TextButton bStop;

    juce::TextButton bLoop;
    void toggleLoop();
    
    juce::TextButton recordButton;
    
    //Info
//    juce::Label lMeter;
    juce::Label lTimeCode;
    AudioMeterComponent AudioMeter;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlBarComponent)
    
    void bPlayClicked();
    void bStopClicked();
    void bPauseClicked();
    
    void bLoopClicked();
    
    void recordButtonClicked();
    
    //items

    //Tools
    juce::ToggleButton bEins;
    juce::ToggleButton bZwei;
    juce::ToggleButton bDrei;
    juce::ToggleButton bVier;
    juce::ToggleButton bFünf;
    juce::ToggleButton bSechs;
    //MainControls
    LogoComponent   Logo;
    
    //styling - lookAndFeel
    LookAndFeel001 LookAndFeel001;
    LookAndFeel003 LookAndFeel003;
    
    // BG
    juce::Image imgBG;
    
};

