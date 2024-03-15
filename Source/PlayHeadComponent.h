/*
  ==============================================================================

    PlayHeadComponent.h
    Created: 15 Mar 2024 5:11:47pm
    Author:  Klaas Klee

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PlayHeadComponent  : public juce::Component
{
public:
    PlayHeadComponent();
    ~PlayHeadComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayHeadComponent)
};
