/*
  ==============================================================================

    LayerViewPortComponent.h
    Created: 9 Feb 2024 10:38:18am
    Author:  Klaas Klee

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LayersContainerComponent.h"

//==============================================================================
/*
*/
class LayerViewPortComponent  : public juce::Component
{
public:
    LayerViewPortComponent();
    ~LayerViewPortComponent() override;

    void paint(juce::Graphics&) override;

    void resized() override;

    LayersContainerComponent LayersContainer;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LayerViewPortComponent)
    
    juce::Viewport LayerViewport;
    
    // BG
    juce::Image imgBG;
};
