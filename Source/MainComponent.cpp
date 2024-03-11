#include "MainComponent.h"
#include "Globals.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (1200, 800);
    
    addAndMakeVisible(ControlBar);
    addAndMakeVisible(LayersViewPort);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }

    
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    outBuffer.setSize(2, samplesPerBlockExpected);

    transportStateChanged(Stopped);

}

void MainComponent::transportStateChanged(TransportState newState)
{
    if (newState != state)
    {
        state = newState;
        
        switch (state) {
            case Stopped:
                setTransportPos(0.0);
                ControlBar.bPlay.setEnabled(true);
                ControlBar.bPlay.setToggleState(false, juce::NotificationType::dontSendNotification);
                ControlBar.bPause.setEnabled(false);
                ControlBar.bPause.setToggleState(false, juce::NotificationType::dontSendNotification);
                ControlBar.bStop.setEnabled(false);
                ControlBar.bStop.setToggleState(true, juce::NotificationType::dontSendNotification);
                DBG("state = stopped");
                break;
            case Playing:
                ControlBar.bPlay.setEnabled(false);
                ControlBar.bPlay.setToggleState(true, juce::NotificationType::dontSendNotification);
                ControlBar.bPause.setEnabled(true);
                ControlBar.bPause.setToggleState(false, juce::NotificationType::dontSendNotification);
                ControlBar.bStop.setEnabled(true);
                ControlBar.bStop.setToggleState(false, juce::NotificationType::dontSendNotification);
                break;
            case Starting:
                ControlBar.bPlay.setEnabled(false);
                ControlBar.bPlay.setToggleState(true, juce::NotificationType::dontSendNotification);
                ControlBar.bPause.setEnabled(true);
                ControlBar.bPause.setToggleState(false, juce::NotificationType::dontSendNotification);
                ControlBar.bStop.setEnabled(true);
                ControlBar.bStop.setToggleState(false, juce::NotificationType::dontSendNotification);
                setTransportStart();
                DBG("state = starting");
                break;
            case Stopping:
                ControlBar.bPlay.setEnabled(true);
                ControlBar.bPlay.setToggleState(false, juce::NotificationType::dontSendNotification);
                ControlBar.bPause.setEnabled(false);
                ControlBar.bPause.setToggleState(false, juce::NotificationType::dontSendNotification);
                ControlBar.bStop.setEnabled(false);
                ControlBar.bStop.setToggleState(true, juce::NotificationType::dontSendNotification);
                setTransportStop();
                DBG("state = stopping");
                break;
            case Pause:
                ControlBar.bPlay.setEnabled(true);
                ControlBar.bPlay.setToggleState(false, juce::NotificationType::dontSendNotification);
                ControlBar.bPause.setEnabled(false);
                ControlBar.bPause.setToggleState(true, juce::NotificationType::dontSendNotification);
                ControlBar.bStop.setEnabled(true);
                ControlBar.bStop.setToggleState(false, juce::NotificationType::dontSendNotification);
                setTransportStop();
                DBG("state = pause");
                break;
        }
    }
}

void MainComponent::setTransportPos(float pos)
{
    
}
void MainComponent::setTransportStart()
{
    
}
void MainComponent::setTransportStop()
{
    
}
void MainComponent::setTransportLoop(bool b)
{
    
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    
    auto numChannels = bufferToFill.buffer->getNumChannels();
    auto lengthInSamples = bufferToFill.numSamples;
    int nullInt = 0;
    if (LayersViewPort.LayersContainer.Layers[0].LayerWave.fileLoaded && LayersViewPort.LayersContainer.Layers[1].LayerWave.fileLoaded && LayersViewPort.LayersContainer.Layers[2].LayerWave.fileLoaded) {

        functionPointerType blendMode = getBlendModeFct(LayersViewPort.LayersContainer.Layers[2].LayerControl.selectedBlendMode);

        //todo: die smarte for schleife fuer alle Layer

        // todo: besseres handling von verschiedenen layer längen (zB nicht Abstürzen des Programms.)

        // todo: pause: als globale variable bool machen: in getNextAudioBlock abfragen, wenn true: noch einen Audio Block abspielen, aber am ende eine GainRamp machen. 

        blendModeAdd(LayersViewPort.LayersContainer.Layers[2].LayerWave.playBuffer, 
            LayersViewPort.LayersContainer.Layers[1].LayerWave.playBuffer, 
            outBuffer, 
            lengthInSamples, 
            LayersViewPort.LayersContainer.Layers[2].LayerWave.playPos, 
            LayersViewPort.LayersContainer.Layers[1].LayerWave.playPos);

        blendModeMult(outBuffer,
            LayersViewPort.LayersContainer.Layers[0].LayerWave.playBuffer,
            outBuffer,
            lengthInSamples,
            nullInt,
            LayersViewPort.LayersContainer.Layers[0].LayerWave.playPos);
    }


    if (LayersViewPort.LayersContainer.Layers[0].LayerWave.fileLoaded)
    {
        for (int ch = 0; ch < numChannels; ch++) {
            bufferToFill.buffer->copyFrom(ch,                                       //  destination buffer channel index
                0,                                                                  //  sample offset in output buffer
                outBuffer,                                                          //  source buffer
                ch % numChannels,                                                   //  channel of input buffer
                0,                                                                  //  start copy position in input buffer
                lengthInSamples);                                                   //  number of samples to copy
        }
    }
}




void MainComponent::blendModeAdd(juce::AudioSampleBuffer& layerA, juce::AudioSampleBuffer& layerB, juce::AudioSampleBuffer& outLayer, int numSamples, int& playPosA, int& playPosB)
{
    int numChannelsA = layerA.getNumChannels();
    int numChannelsB = layerB.getNumChannels();

    const float* readA; 
    const float* readB; 
    float* writeOut; 

    jassert (numChannelsA == numChannelsB);

    for (int ch = 0; ch < numChannelsA; ch++)
    {
        readA = layerA.getReadPointer(ch, playPosA);
        readB = layerB.getReadPointer(ch, playPosB);
        writeOut = outLayer.getWritePointer(ch);

        for (int i = 0; i < numSamples; i++) {
            *writeOut++ = *readA++ + *readB++;
        }
    }
    playPosA += numSamples;
    playPosB += numSamples; 
}

void MainComponent::blendModeMult(juce::AudioSampleBuffer& layerA, juce::AudioSampleBuffer& layerB, juce::AudioSampleBuffer& outLayer, int numSamples, int& playPosA, int& playPosB)
{
    int numChannelsA = layerA.getNumChannels();
    int numChannelsB = layerB.getNumChannels();

    const float* readA;
    const float* readB;
    float* writeOut;

    jassert(numChannelsA == numChannelsB);

    for (int ch = 0; ch < numChannelsA; ch++)
    {
        readA = layerA.getReadPointer(ch, playPosA);
        readB = layerB.getReadPointer(ch, playPosB);
        writeOut = outLayer.getWritePointer(ch);

        for (int i = 0; i < numSamples; i++) {
            *writeOut++ = *readA++ * *readB++;
        }
    }
    playPosA += numSamples;
    playPosB += numSamples;
}



void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll(GlobalColors::BG);
}

void MainComponent::resized()
{
    ControlBar.setBounds(0, 0, getWidth(), getHeight()/5);
    LayersViewPort.setBounds(0, getHeight()/5, getWidth(), getHeight() -  getHeight()/5);
}
