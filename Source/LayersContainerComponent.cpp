/*
  ==============================================================================

    LayersContainerComponent.cpp
    Created: 9 Feb 2024 10:38:35am
    Author:  Klaas Klee

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LayersContainerComponent.h"
#include "Globals.h"

//==============================================================================
LayersContainerComponent::LayersContainerComponent()
{
    int index = numOfLayers;
    for (int i = 0; i < numOfLayers; i++) {
        addAndMakeVisible(Layers[i]);
        Layers[i].layerIndex = index;
        index--;
        
        // sets lowest Layer as baseLayer
        // removes options for BlendModes because they will have no effect anyway
        if (i == numOfLayers-1)
        {
            Layers[i].LayerControl.removeChildComponent(&Layers[i].LayerControl.BlendModeDropdown);
        }
    }
}

LayersContainerComponent::~LayersContainerComponent()
{
}

void LayersContainerComponent::resized()
{
    juce::FlexBox layersFB;
    layersFB.flexDirection = juce::FlexBox::Direction::row;
    layersFB.alignContent = juce::FlexBox::AlignContent::flexStart;
    layersFB.flexWrap = juce::FlexBox::Wrap::wrap;
    
    for (int i = 0; i < numOfLayers; i++) {
        layersFB.items.add(juce::FlexItem(Layers[i]).withHeight(200).withWidth(getLocalBounds().getWidth()));
    }
   
    layersFB.performLayout(getLocalBounds());

}
