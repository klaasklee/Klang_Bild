/*
  ==============================================================================

    LayersContainerComponent.h
    Created: 9 Feb 2024 10:38:35am
    Author:  Klaas Klee

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LayerComponent.h"
#include "Globals.h"

//==============================================================================
/*
*/
class LayersContainerComponent  : public juce::Component
{
public:
    LayersContainerComponent();
    ~LayersContainerComponent() override;

    int getLayerContainerHeight(){
        return numOfLayers*layerHeight;
    }
    
    void resized() override;

    LayerComponent Layers[numOfLayers];

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LayersContainerComponent)

};
