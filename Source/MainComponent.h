#pragma once

#include <JuceHeader.h>
#include "ControlBarComponent.h"
#include "LayerViewPortComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

    
    enum TransportState
    {
        Stopped,
        Starting,
        Stopping,
        Playing,
        Pause
    };
    TransportState state;
    void transportStateChanged(TransportState newState);
    
    void setTransportLoop(bool b);
    


private:
    //==============================================================================
    // Your private member variables go here...
    
    void setTransportPos(float pos);
    void setTransportStart();
    void setTransportStop();

    static void blendModeAdd(juce::AudioSampleBuffer& layerA, juce::AudioSampleBuffer& layerB, juce::AudioSampleBuffer& outLayer, int numSamples, int playPosA, int& playPosB);
    static void blendModeMult(juce::AudioSampleBuffer& layerA, juce::AudioSampleBuffer& layerB, juce::AudioSampleBuffer& outLayer, int numSamples, int playPosA, int& playPosB);

    typedef void (*functionPointerType)(juce::AudioSampleBuffer& layerA, juce::AudioSampleBuffer& layerB, juce::AudioSampleBuffer& outLayer, int numSamples, int playPosA, int& playPosB);

    static functionPointerType getBlendModeFct(BlendModes blendMode) {
        switch (1) {
        case BlendModes::Normal:
            return &blendModeAdd;
        case BlendModes::Multiply:
            return &blendModeMult;
        default:
            return NULL;
        }
    }

    ControlBarComponent ControlBar;
    LayerViewPortComponent LayersViewPort;

    juce::AudioSampleBuffer outBuffer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)

};
