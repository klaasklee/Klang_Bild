/*
  ==============================================================================

    KeyListenerComponent.cpp
    Created: 22 Mar 2024 4:30:13pm
    Author:  Klaas Klee

  ==============================================================================
*/

#include <JuceHeader.h>
#include "KeyListenerComponent.h"
#include "MainComponent.h"
#include "Globals.h"

//==============================================================================
KeyListenerComponent::KeyListenerComponent()
{
}

KeyListenerComponent::~KeyListenerComponent()
{
}

bool KeyListenerComponent::keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent)
{
    if (key.getTextCharacter() == ' ') {
//        DBG("The space key was pressed");
        findParentComponentOfClass<MainComponent>()->toggleTransportPlayPause();
    }
    
    return true; // Return true to indicate the event was handled
}

void KeyListenerComponent::paint (juce::Graphics& g)
{
}

void KeyListenerComponent::resized()
{
}
