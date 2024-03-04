/*
  ==============================================================================

    LayerWaveComponent.h
    Created: 9 Feb 2024 10:40:04am
    Author:  Klaas Klee

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Globals.h"

//==============================================================================
/*
*/
class LayerWaveComponent  : public juce::Component
{
public:
    LayerWaveComponent();
    ~LayerWaveComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LayerWaveComponent)
    
    juce::TextButton openButton;
    void openButtonClicked();
    
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> playSource;
    
    float duration;
    
};
