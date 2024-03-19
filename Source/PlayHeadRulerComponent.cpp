/*
  ==============================================================================

    PlayHeadRulerComponent.cpp
    Created: 19 Mar 2024 8:51:20pm
    Author:  Klaas Klee

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlayHeadRulerComponent.h"
#include "Globals.h"
#include "MainComponent.h"

//==============================================================================
PlayHeadRulerComponent::PlayHeadRulerComponent()
{

}


PlayHeadRulerComponent::~PlayHeadRulerComponent()
{
}

void PlayHeadRulerComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::white);   // clear the background
    g.setColour(juce::Colours::black);
    
    int ratio = ((globalSampleRate*timeLineSize)/(getWidth()-layerControlW-waveBorder*2));
    
    for (int sample = 0; sample <= timeLineSize*globalSampleRate; sample += (globalSampleRate*10))
    {
        DBG(sample);
        g.drawLine(250+waveBorder +1 + (sample / ratio), 5, 250+waveBorder +1 + (sample / ratio), 30, 3);
    }

}

void PlayHeadRulerComponent::mouseDown(const juce::MouseEvent& event)
{
//    DBG("PlayHeadRulerComponent - MouseDown");
    findParentComponentOfClass<MainComponent>()->setPlayHeadPos(event.getMouseDownX()-layerControlW);
}

void PlayHeadRulerComponent::resized()
{

}
