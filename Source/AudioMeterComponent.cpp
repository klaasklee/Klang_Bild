/*
  ==============================================================================

    AudioMeterComponent.cpp
    Created: 20 Mar 2024 1:00:13pm
    Author:  Klaas Klee

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AudioMeterComponent.h"
//==============================================================================
AudioMeterComponent::AudioMeterComponent()
{
}

AudioMeterComponent::~AudioMeterComponent()
{
}

void AudioMeterComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("AudioMeterComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void AudioMeterComponent::updateAudioMeter(const juce::AudioSourceChannelInfo& bufferToFill)
{
//    DBG("updateAudioMeter");
    
    int numBufferSamples = bufferToFill.numSamples;
    const float* readL = bufferToFill.buffer->getReadPointer(0);
    const float* readR = bufferToFill.buffer->getReadPointer(1);
    
    // solange rmsMaxNumOfValues nicht überschritten oder buffer leer
    for (int i = 0; i < numBufferSamples; i++)
    {
        if (rmsMaxNumOfValues < rmsNumOfCurrentValues)
        {
            // nächstes bufferToFill Sample auslesen beider channels
            
            // rmsSumOfCurrentValuesSquare = rmsSumOfCurrentValuesSquare + bufferToFillSample^2
            
            // increment rmsNumOfCurrentValues
            rmsNumOfCurrentValues ++;
        }
        else
        {
            return;
        }
    }
    
    // wenn voll
        // rms Calc von beiden channels
            // all values^2
            // devidet by rmsValues (besser eigene int dafür (akkurater))
            // root of all
        // update GUI von beiden channels
    // resetAudioMeter()
}
void AudioMeterComponent::resetAudioMeter()
{
//    DBG("resetAudioMeter");
    rmsSumOfCurrentValuesSquare = 0;
}

void AudioMeterComponent::resized()
{
}
