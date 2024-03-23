/*
  ==============================================================================

    LayerBlendmodeControlComponent.h
    Created: 22 Mar 2024 2:34:55pm
    Author:  Klaas Klee

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Globals.h"

//==============================================================================
/*
*/
class LayerBlendmodeControlComponent  : public juce::Component,
                                        public juce::Slider::Listener
{
public:
    LayerBlendmodeControlComponent();
    ~LayerBlendmodeControlComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged (juce::Slider* slider) override;
    
    float fPara1;
    float fPara2;
    bool boPara1;
    bool boPara2;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LayerBlendmodeControlComponent)
    
    juce::Slider sPara1;
    juce::Slider sPara2;
    
    LookAndFeel001 LookAndFeel001;
    
    juce::ToggleButton bPara1;
    juce::ToggleButton bPara2;
    
    void bPara1Clicked();
    void bPara2Clicked();
};
