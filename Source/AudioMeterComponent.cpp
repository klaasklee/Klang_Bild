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
    // set size of rmsValues
    rmsValues.setSize(2, rmsIMax);
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
    
    // solange in rmsValues samples reinpassen
        // bufferToFillSamples nach rmsValues
    // wenn voll
        // rms Calc
        // update GUI
    // resetAudioMeter()
}
void AudioMeterComponent::resetAudioMeter()
{
//    DBG("resetAudioMeter");
    rmsValues.clear();
}

void AudioMeterComponent::resized()
{
}
