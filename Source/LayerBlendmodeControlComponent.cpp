/*
  ==============================================================================

    LayerBlendmodeControlComponent.cpp
    Created: 22 Mar 2024 2:34:55pm
    Author:  Klaas Klee

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LayerBlendmodeControlComponent.h"

//==============================================================================
LayerBlendmodeControlComponent::LayerBlendmodeControlComponent()
{
}

LayerBlendmodeControlComponent::~LayerBlendmodeControlComponent()
{
}

void LayerBlendmodeControlComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::darkgrey);
}

void LayerBlendmodeControlComponent::resized()
{
}
