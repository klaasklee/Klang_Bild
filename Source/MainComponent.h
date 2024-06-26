#pragma once

#include <JuceHeader.h>
#include "ControlBarComponent.h"
#include "LayerViewPortComponent.h"
#include "AlertWindowComponent.h"
#include "PlayHeadComponent.h"
#include "PlayHeadRulerComponent.h"
#include "CrosssynthProcessor.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  :  public juce::AudioAppComponent,
                        public juce::KeyListener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    bool keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent) override;
    
    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

    void setVolume(float volume);
    
    int timeLineSize = 0;
    
    enum TransportState
    {
        Stop,
        Play,
        Pause,
        Export
    };
    static TransportState state;
        
    void transportStateChanged(TransportState newState);
    
    void toggleTransportPlayPause();
    
    void setTransportLoop(bool b);
    
    void toggleExportState();
    
    juce::AudioSampleBuffer exportBuffer;
    void prepareAudioExport();
    void exportAudioToFile(juce::AudioBuffer<float> &buffer, juce::String fileName);
    
    void killExportAlertWindow();
    void killSetTimeLineSizeAlertWindow();
    AlertWindowComponent SetTimeLineSizeAlertWindow;
    
    void setTimeLineSize(int i);
    
    // position of playHead if state == stop
    float playHeadStartPos = 0;
    
    // playHead position in pixel X
    float playHeadPos = 0;
    
    // play position in samples
    int playPosInSamples = 0;
    
    // this function is for setting the playHead with a mouseEvent
    // in GUI and changes playHeadPos as well as playPosInSamples
    void setPlayHeadPos(int pos);
    
    int activeLayerIndexes[numOfLayers];
    int numActiveLayers = 0;
    void calcActiveLayerIndices();
    int getLayerArrayIndexFromLayerIndex(int layerIndex);
    
private: 
    //==============================================================================
    // Your private member variables go here...

    //ControlBar
    float globalVolume;
    bool globalLoop = false;
    
    //Export
    std::unique_ptr<juce::AlertWindow> alertWindow;
    juce::TextEditor alertTextEditor;
    long samplesWritten;
    
    //BlendModes
    static void blendModeAdd(juce::AudioSampleBuffer& layerA, LayerComponent& layerB, juce::AudioSampleBuffer& outLayer, int numSamples, int playPosA, int playPosB);
    static void blendModeMult(juce::AudioSampleBuffer& layerA, LayerComponent& layerB, juce::AudioSampleBuffer& outLayer, int numSamples, int playPosA, int playPosB);
    static void blendModeDuck(juce::AudioSampleBuffer& layerA, LayerComponent& layerB, juce::AudioSampleBuffer& outLayer, int numSamples, int playPosA, int playPosB);
    static void blendModeBinary(juce::AudioSampleBuffer& layerA, LayerComponent& layerB, juce::AudioSampleBuffer& outLayer, int numSamples, int playPosA, int playPosB);
    static void blendModeVariableFilter(juce::AudioSampleBuffer& layerA, LayerComponent& layerB, juce::AudioSampleBuffer& outLayer, int numSamples, int playPosA, int playPosB);
    static void blendModeCrossSynth(juce::AudioSampleBuffer& layerA, LayerComponent& layerB, juce::AudioSampleBuffer& outLayer, int numSamples, int playPosA, int playPosB);
    static void blendModeFreqFill(juce::AudioSampleBuffer& layerA, LayerComponent& layerB, juce::AudioSampleBuffer& outLayer, int numSamples, int playPosA, int playPosB);
    static void blendModePitchShift(juce::AudioSampleBuffer& layerA, LayerComponent& layerB, juce::AudioSampleBuffer& outLayer, int numSamples, int playPosA, int playPosB);

    
    typedef void (*functionPointerType)(juce::AudioSampleBuffer& layerA, LayerComponent& layerB, juce::AudioSampleBuffer& outLayer, int numSamples, int playPosA, int playPosB);

    static functionPointerType getBlendModeFct(BlendModes blendMode) {
        switch (blendMode) {
        case BlendModes::Normal:
            return &blendModeAdd;
        case BlendModes::Multiply:
            return &blendModeMult;
        case BlendModes::Duck:
            return &blendModeDuck;
        case BlendModes::Binary:
            return &blendModeBinary;
        case BlendModes::VariableFilter:
            return &blendModeVariableFilter;
        case BlendModes::CrossSynth:
            return &blendModeCrossSynth;
        case BlendModes::FreqFill:
            return &blendModeFreqFill;
        case BlendModes::PitchShift:
            return &blendModePitchShift;
        default:
            return NULL;
        }
    }

    enum class FilterType
    {
        Lowpass,
        Bandpass,
        Highpass,
    };

    FilterType filterType{ FilterType::Lowpass };
    static juce::dsp::StateVariableTPTFilter<float> filter;

    //Components
    ControlBarComponent ControlBar;
    PlayHeadRulerComponent PlayHeadRuler;
    LayerViewPortComponent LayersViewPort;
    
    PlayHeadComponent PlayHead;
    
    AlertWindowComponent ExportAlertWindow;
    
    juce::AudioSampleBuffer outBuffer;
        
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)

};
