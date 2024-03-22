/*
  ==============================================================================

    KeyListener.h
    Created: 22 Mar 2024 3:03:25pm
    Author:  Klaas Klee

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class KeyListener : public juce::KeyListener
{
public:
    bool keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent) override
    {
        // Handle key press event
        if (key.getTextCharacter() == ' ') {
            // Do something when 'a' key is pressed
            juce::Logger::writeToLog("The space key was pressed");
        }
        
        return true; // Return true to indicate the event was handled
    }
};
