#include "MainComponent.h"
#include "Globals.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
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
    
    for (int i = 0; i < numOfLayers; i++)
    {
        LayersViewPort.LayersContainer.Layers[i].LayerWave.transport.addChangeListener(this);
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
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    outBuffer.setSize(2, samplesPerBlockExpected);

    for (int i = 0; i < numOfLayers; i++)
    {
        LayersViewPort.LayersContainer.Layers[i].LayerWave.transport.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }
    
    transportStateChanged(Stopped);


    
}

void MainComponent::transportStateChanged(TransportState newState)
{
    if (newState != state)
    {
        state = newState;
        
        switch (state) {
            case Stopped:
                setTransportsPos(0.0);
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
                setTransportsStart();
                DBG("state = starting");
                break;
            case Stopping:
                ControlBar.bPlay.setEnabled(true);
                ControlBar.bPlay.setToggleState(false, juce::NotificationType::dontSendNotification);
                ControlBar.bPause.setEnabled(false);
                ControlBar.bPause.setToggleState(false, juce::NotificationType::dontSendNotification);
                ControlBar.bStop.setEnabled(false);
                ControlBar.bStop.setToggleState(true, juce::NotificationType::dontSendNotification);
                setTransportsStop();
                DBG("state = stopping");
                break;
            case Pause:
                ControlBar.bPlay.setEnabled(true);
                ControlBar.bPlay.setToggleState(false, juce::NotificationType::dontSendNotification);
                ControlBar.bPause.setEnabled(false);
                ControlBar.bPause.setToggleState(true, juce::NotificationType::dontSendNotification);
                ControlBar.bStop.setEnabled(true);
                ControlBar.bStop.setToggleState(false, juce::NotificationType::dontSendNotification);
                setTransportsStop();
                DBG("state = pause");
                break;
        }
    }
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    for (int i = 0; i < numOfLayers; i++)
    {
        if (source == &LayersViewPort.LayersContainer.Layers[i].LayerWave.transport)
        {
            if (LayersViewPort.LayersContainer.Layers[i].LayerWave.transport.isPlaying())
            {
                transportStateChanged(Playing);
            }
            else
            {
                if (state == Pause)
                {
                    transportStateChanged(Pause);
                }
                else
                {
                    transportStateChanged(Stopped);
                }
            }
        }
    }
}

void MainComponent::setTransportsPos(float pos)
{
    for (int i = 0; i < numOfLayers; i++)
    {
        LayersViewPort.LayersContainer.Layers[i].LayerWave.transport.setPosition(pos);
    }
}
void MainComponent::setTransportsStart()
{
    for (int i = 0; i < numOfLayers; i++)
    {
        LayersViewPort.LayersContainer.Layers[i].LayerWave.transport.start();
        std::cout << "isPlaying Layer "<< i << ": " << LayersViewPort.LayersContainer.Layers[i].LayerWave.transport.isPlaying() << std::endl;
    }
}
void MainComponent::setTransportsStop()
{
    for (int i = 0; i < numOfLayers; i++)
    {
        LayersViewPort.LayersContainer.Layers[i].LayerWave.transport.stop();
        std::cout << "isPlaying Layer "<< i << ": " << LayersViewPort.LayersContainer.Layers[i].LayerWave.transport.isPlaying() << std::endl;
    }
}

// set Looping does somehow not work like this?
void MainComponent::setTransportsLoop(bool b)
{
    DBG("does not work");
//    for (int i = 0; i < numOfLayers; i++)
//    {
//        LayersViewPort.LayersContainer.Layers[i].LayerWave.transport.setLooping(true);
//        std::cout << "Loop? Layer "<< i << ": " << LayersViewPort.LayersContainer.Layers[i].LayerWave.transport.isLooping() << std::endl;
//    }
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!
    
    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    
    bufferToFill.clearActiveBufferRegion();

    // todo: how to copy data from the respective audio play Buffers here? 
   // LayersViewPort.LayersContainer.Layers[0].LayerWave.playBuffer.
    
    
    //-------------------------------------------------
    /*
     with this method pausing/stopping all the transport sources is kinda laggy
     
     Maybe you can empty the bufferToFill first and then stop all transports.
     Then the sound is stopped directly.
     So that all transports start playing again at the same time, remember the time of the button click and then reset all transports to this time?
    */
    
    // get Buffers of all transport sources
//    for (int i = 0; i < numOfLayers; i++)
//    {
//        LayersViewPort.LayersContainer.Layers[i].LayerWave.transport.getNextAudioBlock( ???? );
//    }
    
    //do smth with it
        //????
    
    //put it in the Buffer to fill
    //LayersViewPort.LayersContainer.Layers[0].LayerWave.transport.getNextAudioBlock(bufferToFill);
    auto numChannels = bufferToFill.buffer->getNumChannels();
    auto lengthInSamples = bufferToFill.numSamples;
    int nullInt = 0;
    if (LayersViewPort.LayersContainer.Layers[0].LayerWave.fileLoaded && LayersViewPort.LayersContainer.Layers[1].LayerWave.fileLoaded && LayersViewPort.LayersContainer.Layers[2].LayerWave.fileLoaded) {

        functionPointerType blendMode = getBlendModeFct(LayersViewPort.LayersContainer.Layers[2].LayerControl.selectedBlendMode);

        //todo: die smarte for schleife für alle Layer

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
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    
    ControlBar.setBounds(0, 0, getWidth(), getHeight()/5);
    LayersViewPort.setBounds(0, getHeight()/5, getWidth(), getHeight() -  getHeight()/5);
}
