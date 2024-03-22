/*
  ==============================================================================

    KeyListenerComponent.h
    Created: 22 Mar 2024 4:30:13pm
    Author:  Klaas Klee

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class KeyListenerComponent  :   public juce::Component,
                                public juce::KeyListener
{
public:
    KeyListenerComponent();
    ~KeyListenerComponent() override;

    bool keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent) override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyListenerComponent)
};
