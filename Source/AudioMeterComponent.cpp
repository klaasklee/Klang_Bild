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
    // draw meter
    auto mainBound = getLocalBounds().reduced(60, 12);
    auto boundsL = mainBound.toFloat().removeFromTop(mainBound.getHeight()/2).reduced(3.f);
    auto boundsR = mainBound.toFloat().removeFromBottom(mainBound.getHeight()/2).reduced(3.f);
    
    g.setColour(juce::Colours::black);
    g.fillRect(boundsL);
    g.fillRect(boundsR);

    juce::ColourGradient gradient = juce::ColourGradient{ juce::Colours::green, boundsL.getTopLeft(), juce::Colours::red, boundsL.getTopRight(), false };
    gradient.addColour(0.5, juce::Colours::yellow);
    
    g.setGradientFill(gradient);
    float scaledXL = juce::jmap(static_cast<float>(rmsCurrentValueL), 0.0f, 1.f, 0.0f, static_cast<float>(getWidth()));
    float scaledXR = juce::jmap(static_cast<float>(rmsCurrentValueR), 0.0f, 1.f, 0.0f, static_cast<float>(getWidth()));
//    DBG(scaledXL);
//    DBG(scaledXR);
    g.fillRect(boundsL.removeFromLeft(scaledXL));
    g.fillRect(boundsR.removeFromLeft(scaledXR));
    
    
    // R and L label
    auto labelBoundsL = mainBound.toFloat().removeFromTop(mainBound.getHeight()/2).reduced(4.f);
    auto labelBoundsR = mainBound.toFloat().removeFromBottom(mainBound.getHeight()/2).reduced(4.f);
    juce::Rectangle<float> labelL;
    juce::Rectangle<float> labelR;
    labelL.setBounds(labelBoundsL.getTopLeft().getX()-(labelBoundsL.getHeight()+3)*1.4, labelBoundsL.getTopLeft().getY()-3, labelBoundsL.getHeight()+6, labelBoundsL.getHeight()+6);
    labelR.setBounds(labelBoundsR.getTopLeft().getX()-(labelBoundsR.getHeight()+3)*1.4, labelBoundsR.getTopLeft().getY()-3, labelBoundsR.getHeight()+6, labelBoundsR.getHeight()+6);
    
    g.setColour(GlobalColors::black);
    g.fillRect(labelL);
    g.fillRect(labelR);
    g.setColour(GlobalColors::white);
    g.drawText("L", labelL, juce::Justification::centred);
    g.drawText("R", labelR, juce::Justification::centred);
}
void AudioMeterComponent::paintOverChildren(::juce::Graphics& g)
{
    auto mainBound = getLocalBounds().reduced(60, 12);
    auto boundsL = mainBound.toFloat().removeFromTop(mainBound.getHeight()/2).reduced(1.f);
    auto boundsR = mainBound.toFloat().removeFromBottom(mainBound.getHeight()/2).reduced(1.f);
    
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
void AudioMeterComponent::resetAudioMeter()
{
    rmsNumOfCurrentValues = 0;
    rmsSumOfSquaresL = 0;
    rmsSumOfSquaresR = 0;
    rmsCurrentValueL = 0;
    rmsCurrentValueR = 0;
    rmsTargetValueL = 0;
    rmsTargetValueR = 0;
}

void AudioMeterComponent::resized()
{
}
