/*
  ==============================================================================

    PlayHeadComponent.cpp
    Created: 15 Mar 2024 5:11:47pm
    Author:  Klaas Klee

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlayHeadComponent.h"
#include "Globals.h"

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
//    g.fillAll(juce::Colours::white);
    
    juce::Path triangle;
    triangle.addTriangle(0, 10, getWidth(), 10, getWidth()/2, 20);
    
    g.setColour(GlobalColors::white);
    g.drawLine(getWidth()/2, 18, getWidth()/2, getHeight(), 2);
    g.fillPath(triangle);
}

void PlayHeadComponent::resized()
{
}
