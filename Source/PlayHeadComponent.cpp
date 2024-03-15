/*
  ==============================================================================

    PlayHeadComponent.cpp
    Created: 15 Mar 2024 5:11:47pm
    Author:  Klaas Klee

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlayHeadComponent.h"

//==============================================================================
PlayHeadComponent::PlayHeadComponent()
{

}

PlayHeadComponent::~PlayHeadComponent()
{
}

void PlayHeadComponent::paint (juce::Graphics& g)
{
    // drawPlayHead
    g.fillAll(juce::Colours::red);
}

void PlayHeadComponent::resized()
{
}
