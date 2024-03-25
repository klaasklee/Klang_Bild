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
    void paintOverChildren(::juce::Graphics& g) override;
    void resized() override;
    
    void updateAudioMeter(const juce::AudioSourceChannelInfo& bufferToFill);
    void resetAudioMeter();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioMeterComponent)
    
    // amount of samples I need to update the levelMeter about every 12 seconds
    const int rmsMaxNumOfValues = globalSampleRate/24;
    
    int rmsNumOfCurrentValues = 0;
    float rmsSumOfSquaresL;
    float rmsSumOfSquaresR;
    double rmsCurrentValueL = 0;
    double rmsCurrentValueR = 0;
    double rmsTargetValueL = 0;
    double rmsTargetValueR = 0;
    
    std::unique_ptr<juce::Drawable> grill;
};
