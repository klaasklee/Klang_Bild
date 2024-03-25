/*
  ==============================================================================

    LayerComponent.h
    Created: 9 Feb 2024 10:39:21am
    Author:  Klaas Klee

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LayerControlComponent.h"
#include "LayerWaveComponent.h"
#include "LayerBlendmodeControlComponent.h"

//==============================================================================
/*
*/
class LayerComponent  : public juce::Component
{
public:
    LayerComponent();
    ~LayerComponent() override;

    void paint (juce::Graphics&) override;

    
    void resized() override;

    int layerIndex;
        
    LayerControlComponent LayerControl;
    LayerWaveComponent LayerWave;

    LayerBlendmodeControlComponent LayerBlendmodeControl;
    
    juce::TextButton bToggleShowBlendPara;


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LayerComponent)
    
    juce::ShapeButton layerUp;
    juce::ShapeButton layerDown;
    
    void moveLayerUp();
    void moveLayerDown();
    
    void bToggleShowBlendParaClicked();
    
    LookAndFeel001 LookAndFeel001;
    
};
