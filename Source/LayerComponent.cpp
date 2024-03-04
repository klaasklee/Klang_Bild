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
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    addAndMakeVisible(LayerControl);
    addAndMakeVisible(LayerWave);
}

LayerComponent::~LayerComponent()
{
}

void LayerComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    juce::FlexBox layerFB;
    layerFB.flexDirection = juce::FlexBox::Direction::row;
    layerFB.flexWrap = juce::FlexBox::Wrap::wrap;
    
        layerFB.items.add(juce::FlexItem(LayerControl).withWidth(250));
        layerFB.items.add(juce::FlexItem(LayerWave).withFlex(1));
   
    layerFB.performLayout(getLocalBounds());
}
