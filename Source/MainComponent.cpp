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
    
    transportStateChanged(Stop);

}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (state == Play) {
        
        bufferToFill.clearActiveBufferRegion();
        
        auto numChannels = bufferToFill.buffer->getNumChannels();
        auto lengthInSamples = bufferToFill.numSamples;
        int nullInt = 0;
        int numActiveLayers = 0;
        int activeLayerIndexes[numOfLayers];
        for (int i = 0; i < numOfLayers; i++)
        {
            activeLayerIndexes[i] = -1;
        }
        
        // find the last active layer, and the number of active layers
        for (int layerCounter = numOfLayers - 1; layerCounter >= 0; layerCounter--) {
            if (LayersViewPort.LayersContainer.Layers[layerCounter].LayerWave.fileLoaded) {
                activeLayerIndexes[numActiveLayers] = layerCounter;
                numActiveLayers += 1;
            }
        }
        
        
        if (numActiveLayers > 0) {
            
            // always copy the last active layer to the outBuffer.
            for (int ch = 0; ch < numChannels; ch++) {
                outBuffer.copyFrom(ch,                                                              //  destination buffer channel index
                                   0,                                                                              //  sample offset in output buffer
                                   LayersViewPort.LayersContainer.Layers[activeLayerIndexes[0]].LayerWave.playBuffer,       //  source buffer
                                   ch % numChannels,                                                               //  channel of input buffer
                                   LayersViewPort.LayersContainer.Layers[activeLayerIndexes[0]].LayerWave.playPos,          //  start copy position in input buffer
                                   lengthInSamples);                                                               //  number of samples to copy
            }
            // maoves the playPosition one bufferlenght forward
            LayersViewPort.LayersContainer.Layers[activeLayerIndexes[0]].LayerWave.playPos += lengthInSamples;
            
            
            // calculate blend modes, if there are at least two active layers
            // for only one active layer, the loop will be skipped
            for (int i = 1; i < numActiveLayers; i++) {
                functionPointerType calcBlendMode = getBlendModeFct(LayersViewPort.LayersContainer.Layers[activeLayerIndexes[i]].LayerControl.selectedBlendMode); // get the blend mode of the second to last layer, the blend mode of the last layer is always ignored
                calcBlendMode(outBuffer,
                             LayersViewPort.LayersContainer.Layers[activeLayerIndexes[i]].LayerWave.playBuffer,
                             outBuffer,
                             lengthInSamples,
                             nullInt,
                             LayersViewPort.LayersContainer.Layers[activeLayerIndexes[i]].LayerWave.playPos);
            }
            
            //copy the calculation result to output (bufferToFill)
            for (int ch = 0; ch < numChannels; ch++) {
                bufferToFill.buffer->copyFrom(ch,                                       //  destination buffer channel index
                                              0,                                                                  //  sample offset in output buffer
                                              outBuffer,                                                          //  source buffer
                                              ch % numChannels,                                                   //  channel of input buffer
                                              0,                                                                  //  start copy position in input buffer
                                              lengthInSamples);                                                   //  number of samples to copy
                bufferToFill.buffer->applyGain(0, lengthInSamples, globalVolume);
            }
        }
    }
    
    
    if (state == Stop)
    {
        // find the last active layer, and the number of active layers
        for (int layerCounter = numOfLayers - 1; layerCounter >= 0; layerCounter--)
        {
            LayersViewPort.LayersContainer.Layers[layerCounter].LayerWave.playPos = LayersViewPort.LayersContainer.Layers[layerCounter].LayerWave.playOffset;
        }
    }
    
    // if state = pause nothing hapens
}

#
    //if (numActiveLayers == 1) { // only one layer active, no calculations to be done.
    //    for (int ch = 0; ch < numChannels; ch++) {
    //        outBuffer.copyFrom(ch,                                                              //  destination buffer channel index
    //            0,                                                                              //  sample offset in output buffer
    //            LayersViewPort.LayersContainer.Layers[activeLayerIndexes[0]].LayerWave.playBuffer,       //  source buffer
    //            ch % numChannels,                                                               //  channel of input buffer
    //            LayersViewPort.LayersContainer.Layers[activeLayerIndexes[0]].LayerWave.playPos,          //  start copy position in input buffer
    //            lengthInSamples);                                                               //  number of samples to copy                
    //    }
    //    LayersViewPort.LayersContainer.Layers[activeLayerIndexes[0]].LayerWave.playPos += lengthInSamples;
    //}
    //if(0) {}
    //else {

    //}


    //if (LayersViewPort.LayersContainer.Layers[0].LayerWave.fileLoaded && LayersViewPort.LayersContainer.Layers[1].LayerWave.fileLoaded && LayersViewPort.LayersContainer.Layers[2].LayerWave.fileLoaded) {

    //    functionPointerType blendMode = getBlendModeFct(LayersViewPort.LayersContainer.Layers[2].LayerControl.selectedBlendMode);

    //    //todo: die smarte for schleife fuer alle Layer

    //    // todo: besseres handling von verschiedenen layer längen (zB nicht Abstürzen des Programms.)

    //    // todo: pause: als globale variable bool machen: in getNextAudioBlock abfragen, wenn true: noch einen Audio Block abspielen, aber am ende eine GainRamp machen. 

    //    blendModeAdd(LayersViewPort.LayersContainer.Layers[2].LayerWave.playBuffer, 
    //        LayersViewPort.LayersContainer.Layers[1].LayerWave.playBuffer, 
    //        outBuffer, 
    //        lengthInSamples, 
    //        LayersViewPort.LayersContainer.Layers[2].LayerWave.playPos, 
    //        LayersViewPort.LayersContainer.Layers[1].LayerWave.playPos);

    //    blendModeMult(outBuffer,
    //        LayersViewPort.LayersContainer.Layers[0].LayerWave.playBuffer,
    //        outBuffer,
    //        lengthInSamples,
    //        nullInt,
    //        LayersViewPort.LayersContainer.Layers[0].LayerWave.playPos);
    //}





void MainComponent::blendModeAdd(juce::AudioSampleBuffer& layerA, juce::AudioSampleBuffer& layerB, juce::AudioSampleBuffer& outLayer, int numSamples, int playPosA, int& playPosB)
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
    // do not increment playPosA as it is always the outBuffer! 
    //playPosA += numSamples;
    playPosB += numSamples; 
}

void MainComponent::blendModeMult(juce::AudioSampleBuffer& layerA, juce::AudioSampleBuffer& layerB, juce::AudioSampleBuffer& outLayer, int numSamples, int playPosA, int& playPosB)
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
    //playPosA += numSamples;
    playPosB += numSamples;
}

void MainComponent::transportStateChanged(TransportState newState)
{
    if (newState != state)
    {
        state = newState;
        
        switch (state) {
            case Stop:
                ControlBar.bPlay.setEnabled(true);
                ControlBar.bPlay.setToggleState(false, juce::NotificationType::dontSendNotification);
                ControlBar.bPause.setEnabled(false);
                ControlBar.bPause.setToggleState(false, juce::NotificationType::dontSendNotification);
                ControlBar.bStop.setEnabled(false);
                ControlBar.bStop.setToggleState(true, juce::NotificationType::dontSendNotification);
                DBG("state = stop");
                break;
            case Play:
                ControlBar.bPlay.setEnabled(false);
                ControlBar.bPlay.setToggleState(true, juce::NotificationType::dontSendNotification);
                ControlBar.bPause.setEnabled(true);
                ControlBar.bPause.setToggleState(false, juce::NotificationType::dontSendNotification);
                ControlBar.bStop.setEnabled(true);
                ControlBar.bStop.setToggleState(false, juce::NotificationType::dontSendNotification);
                DBG("state = play");
                break;
            case Pause:
                ControlBar.bPlay.setEnabled(true);
                ControlBar.bPlay.setToggleState(false, juce::NotificationType::dontSendNotification);
                ControlBar.bPause.setEnabled(false);
                ControlBar.bPause.setToggleState(true, juce::NotificationType::dontSendNotification);
                ControlBar.bStop.setEnabled(true);
                ControlBar.bStop.setToggleState(false, juce::NotificationType::dontSendNotification);
                DBG("state = pause");
                break;
        }
    }
}

void MainComponent::setVolume(float volume)
{
    globalVolume = volume;
}

void MainComponent::setTransportLoop(bool b)
{
    globalLoop = b;
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
