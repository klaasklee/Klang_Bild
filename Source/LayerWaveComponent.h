/*
  ==============================================================================

    LayerWaveComponent.h
    Created: 9 Feb 2024 10:40:04am
    Author:  Klaas Klee

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Globals.h"

//==============================================================================
/*
*/
class LayerWaveComponent  : public juce::Component
{
public:
    LayerWaveComponent();
    ~LayerWaveComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    juce::AudioSampleBuffer fileBuffer; // contains the original file
    juce::AudioSampleBuffer resampledBuffer; // contains the resampled file
    juce::AudioSampleBuffer playBuffer; // contains the portion of the resampled file after shifting etc
    // @klaas: eventuell ist es doch besser, wenn wir das mit dem shiften nicht mit den zwei Buffern loesen, sondern mit irgendwelchen integern, die dann die "startposition" angeben? Damit hat man weniger redundanz und ballert den Arbeitsspeicher nicht so voll
    // trotzdem mache ich das erstmal wie besprochen, weil das dann einfacher ist :) 
    int playPos;
    bool fileLoaded;
    int playOffset;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LayerWaveComponent)
    
    juce::TextButton openButton;
    void openButtonClicked();
    
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> playSource;
    
    float duration;
    
    void resampleAudioBuffer(juce::AudioBuffer<float>& srcBuffer, juce::AudioBuffer<float>& destBuffer, float srcSampleRate,  float destSampleRate);
    
    //drawing Waveforms
    bool updateWaveform = false;
    juce::Path p;
    int waveBorder = 15;
    std::vector<float> audioPoints;
    
};
