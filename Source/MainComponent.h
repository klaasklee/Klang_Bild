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
//                        ,public juce::ChangeListener
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

    
    
    // if you add a Blendmode
    // add BlendModeName in LayerControlComponent.h
    // change last Blendmode in LayerControlComponent.cpp line 20

    enum TransportState
    {
        Stopped,
        Starting,
        Stopping,
        Playing,
        Pause
    };
    TransportState state;
    
    
    void transportStateChanged(TransportState nweState);
    juce::AudioTransportSource transport;
    
private:
    //==============================================================================
    // Your private member variables go here...
    
//    void changeListenerCallback(juce::ChangeBroadcaster *source) override;
    
    ControlBarComponent ControlBar;
    LayerViewPortComponent LayersViewPort;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
