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
    auto svgData = juce::MemoryInputStream(BinaryData::LevelMeterGrill_svg, BinaryData::LevelMeterGrill_svgSize, false);
    grill = juce::Drawable::createFromImageDataStream(svgData);
}

AudioMeterComponent::~AudioMeterComponent()
{
}

void AudioMeterComponent::paint (juce::Graphics& g)
{
    // R and L label
    juce::Rectangle<int> labelL;
    juce::Rectangle<int> labelR;
    labelL.setBounds(42, 15, 17, 17);
    labelR.setBounds(42, 33, 17, 17);
    
    g.setColour(GlobalColors::black);
    g.fillRect(labelL);
    g.fillRect(labelR);
    g.setColour(GlobalColors::white);
    g.drawText("L", labelL, juce::Justification::centred);
    g.drawText("R", labelR, juce::Justification::centred);
    
    // draw meter
    auto mainBound = getLocalBounds().reduced(60, 12);
    auto boundsL = mainBound.toFloat().removeFromTop(mainBound.getHeight()/2).reduced(3.f, 3.f);
    auto boundsR = mainBound.toFloat().removeFromBottom(mainBound.getHeight()/2).reduced(3.f, 3.f);
    
    juce::ColourGradient gradient = juce::ColourGradient{ juce::Colours::green, boundsL.getTopLeft(), juce::Colours::red, boundsL.getTopRight(), false };
    
    gradient.addColour(0.5, juce::Colours::yellow);
    
    g.setColour(juce::Colours::black);
    g.fillRect(boundsL);
    g.fillRect(boundsR);

    g.setGradientFill(gradient);
    float scaledXL = juce::jmap(static_cast<float>(rmsCurrentValueL), 0.0f, 1.f, 0.0f, static_cast<float>(getWidth()));
    float scaledXR = juce::jmap(static_cast<float>(rmsCurrentValueR), 0.0f, 1.f, 0.0f, static_cast<float>(getWidth()));
//    DBG(scaledXL);
//    DBG(scaledXR);
    g.fillRect(boundsL.removeFromLeft(scaledXL));
    g.fillRect(boundsR.removeFromLeft(scaledXR));
}
void AudioMeterComponent::paintOverChildren(::juce::Graphics& g)
{
    auto mainBound = getLocalBounds().reduced(60, 15);
    auto boundsL = mainBound.toFloat().removeFromTop(mainBound.getHeight()/2);
    auto boundsR = mainBound.toFloat().removeFromBottom(mainBound.getHeight()/2);
    
    grill->drawWithin(g, boundsL, juce::RectanglePlacement::stretchToFit, 1.0f);
    grill->drawWithin(g, boundsR, juce::RectanglePlacement::stretchToFit, 1.0f);
}

// this is not a loudness Meter
void AudioMeterComponent::updateAudioMeter(const juce::AudioSourceChannelInfo& bufferToFill)
{
    int numBufferSamples = bufferToFill.buffer->getNumSamples();
    int numChannels = bufferToFill.buffer->getNumChannels();

    //    // Iterate over each channel in the buffer
        for (int channel = 0; channel < numChannels; ++channel)
        {
            const float* read = bufferToFill.buffer->getReadPointer(channel);

                // Iterate over each sample in the channel
                for (int sample = 0; sample < numBufferSamples; ++sample)
                {
                    float sampleValue = read[sample];
                    if (channel == 0)
                    {
                        rmsSumOfSquaresL += sampleValue * sampleValue;
                    }
                    if (channel == 1)
                    {
                        rmsSumOfSquaresR += sampleValue * sampleValue;
                    }
                    rmsNumOfCurrentValues ++;
                }
        }
    
    // do some calculations goint from rmsCurrentValue to rmsTargetValue
    if (rmsCurrentValueL > rmsTargetValueL)
    {
        rmsCurrentValueL = rmsTargetValueL - rmsTargetValueL * 0.2;
    }
    if (rmsCurrentValueR > rmsTargetValueR)
    {
        rmsCurrentValueR = rmsTargetValueR - rmsTargetValueR * 0.2;
    }
    
    if (rmsNumOfCurrentValues >= rmsMaxNumOfValues)
    {
        // Calculate the mean of squares
        double meanOfSquaresL = rmsSumOfSquaresL / rmsNumOfCurrentValues;
        double meanOfSquaresR = rmsSumOfSquaresR / rmsNumOfCurrentValues;
        
        // Calculate RMS by taking the square root of the mean of squares
        double newRmsL = std::sqrt(meanOfSquaresL);
        double newRmsR = std::sqrt(meanOfSquaresR);
        
        // update GUI
        // smoothing out the decline but not the transients
        if (newRmsL < rmsCurrentValueL)
        {
            rmsTargetValueL = newRmsL;
        }
        else
        {
            rmsCurrentValueL = newRmsL;
            rmsTargetValueL = newRmsL;
        }
        if (newRmsR < rmsCurrentValueR)
        {
            rmsTargetValueR = newRmsR;
        }
        else
        {
            rmsCurrentValueR = newRmsR;
            rmsTargetValueR = newRmsR;
        }
        
        
        // reset rms Values
        rmsNumOfCurrentValues = 0;
        rmsSumOfSquaresL = 0;
        rmsSumOfSquaresR = 0;
    }
    
}

void AudioMeterComponent::resized()
{
}
