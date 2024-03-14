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

    void setVolume(float volume);
    
    enum TransportState
    {
        Stop,
        Play,
        Pause,
        Export
    };
    TransportState state;
    void transportStateChanged(TransportState newState);
    
    void setTransportLoop(bool b);
    
    void toggleExportState();

private:
    //==============================================================================
    // Your private member variables go here...

    float globalVolume;
    bool globalLoop;
    
    std::unique_ptr<juce::AlertWindow> alertWindow; 
    juce::TextEditor alertTextEditor;
    
    void prepareAudioExport(juce::AudioBuffer<float> buffer);
    void setExportFileName(juce::AudioBuffer<float> buffer, juce::String header, juce::String info);
    void setExportFileNameString(juce::AudioBuffer<float> buffer, juce::String string);
    void exportAudioToFile(juce::AudioBuffer<float> buffer, juce::String fileName);
    
    static void blendModeAdd(juce::AudioSampleBuffer& layerA, juce::AudioSampleBuffer& layerB, juce::AudioSampleBuffer& outLayer, int numSamples, int playPosA, int& playPosB);
    static void blendModeMult(juce::AudioSampleBuffer& layerA, juce::AudioSampleBuffer& layerB, juce::AudioSampleBuffer& outLayer, int numSamples, int playPosA, int& playPosB);

    typedef void (*functionPointerType)(juce::AudioSampleBuffer& layerA, juce::AudioSampleBuffer& layerB, juce::AudioSampleBuffer& outLayer, int numSamples, int playPosA, int& playPosB);

    static functionPointerType getBlendModeFct(BlendModes blendMode) {
        switch (blendMode) {
        case BlendModes::Normal:
            return &blendModeAdd;
        case BlendModes::Multiply:
            return &blendModeMult;
        default:
            return NULL;
        }
    }

    void applyGainForAllTracks(int lengthInSamples, int numActiveLayers, int activeLayerIndexes[], bool applyRevert);

    ControlBarComponent ControlBar;
    LayerViewPortComponent LayersViewPort;

    juce::AudioSampleBuffer outBuffer;
    
    juce::AudioSampleBuffer exportBuffer;
    long samplesWritten;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)

};
