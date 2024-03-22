/*
  ==============================================================================

    AlertWindowComponent.h
    Created: 14 Mar 2024 9:13:36pm
    Author:  Klaas Klee

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class AlertWindowComponent  : public juce::Component
{
public:
    AlertWindowComponent(juce::String b1Txt, juce::String b2Txt, juce::String headerTxt, juce::String infoTxt);
    ~AlertWindowComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlertWindowComponent)
    juce::String headerText;
    juce::String infoText;
    
    juce::TextButton b1;
    void b1Clicked();
    juce::TextButton b2;
    void b2Clicked();
    juce::TextEditor textEditor;
    
};
