/*
  ==============================================================================

    AudioMeterComponent.h
    Created: 20 Mar 2024 1:00:13pm
    Author:  Klaas Klee

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Globals.h"

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
    
    void updateAudioMeter(const juce::AudioSourceChannelInfo& bufferToFill);
    void resetAudioMeter();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioMeterComponent)
    
    // amount of samples I need to update the levelMeter about every 24 seconds
    const int rmsIMax = globalSampleRate/24;
    juce::AudioSampleBuffer rmsValues;
};
