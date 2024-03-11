/*
  ==============================================================================

    LayerComponent.cpp
    Created: 9 Feb 2024 10:39:21am
    Author:  Klaas Klee

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LayerComponent.h"

//==============================================================================
LayerComponent::LayerComponent()
{
    addAndMakeVisible(LayerControl);
    addAndMakeVisible(LayerWave);
}

LayerComponent::~LayerComponent()
{
}

void LayerComponent::resized()
{
    juce::FlexBox layerFB;
    layerFB.flexDirection = juce::FlexBox::Direction::row;
    layerFB.flexWrap = juce::FlexBox::Wrap::wrap;
    
        layerFB.items.add(juce::FlexItem(LayerControl).withWidth(250));
        layerFB.items.add(juce::FlexItem(LayerWave).withFlex(1));
   
    layerFB.performLayout(getLocalBounds());
}
