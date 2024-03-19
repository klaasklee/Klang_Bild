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

void LayerComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
    g.setColour(juce::Colours::white);
    g.setFont(20);
    g.drawText(juce::String(layerIndex), 20, 20, 20, 20, juce::Justification::centred);
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
