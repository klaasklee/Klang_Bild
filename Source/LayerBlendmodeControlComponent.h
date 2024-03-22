/*
  ==============================================================================

    LayerBlendmodeControlComponent.h
    Created: 22 Mar 2024 2:34:55pm
    Author:  Klaas Klee

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LayerBlendmodeControlComponent  : public juce::Component
{
public:
    LayerBlendmodeControlComponent();
    ~LayerBlendmodeControlComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    float fPara1;
    float fPara2;
    bool boPara1;
    bool boPara2;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LayerBlendmodeControlComponent)
};
