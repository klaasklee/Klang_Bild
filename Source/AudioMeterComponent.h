/*
  ==============================================================================

    AudioMeterComponent.h
    Created: 20 Mar 2024 1:00:13pm
    Author:  Klaas Klee

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class AudioMeterComponent  : public juce::Component
{
public:
    AudioMeterComponent();
    ~AudioMeterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioMeterComponent)
};
