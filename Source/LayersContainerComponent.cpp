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
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    for (int i = 0; i < numOfLayers; i++) {
        addAndMakeVisible(Layers[i]);
    }
}

LayersContainerComponent::~LayersContainerComponent()
{
}

void LayersContainerComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    juce::FlexBox layersFB;
    layersFB.flexDirection = juce::FlexBox::Direction::row;
    layersFB.alignContent = juce::FlexBox::AlignContent::flexStart;
    layersFB.flexWrap = juce::FlexBox::Wrap::wrap;
    
    for (int i = 0; i < numOfLayers; i++) {
        layersFB.items.add(juce::FlexItem(Layers[i]).withHeight(200).withWidth(getLocalBounds().getWidth()));
    }
   
    layersFB.performLayout(getLocalBounds());
}
