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
    if (state == Play || state == Export) {

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

        // apply gain and pan for all active layers
        applyGainForAllTracks(lengthInSamples, numActiveLayers, activeLayerIndexes, true);

        if (numActiveLayers > 0) {

            // always copy the last active layer to the outBuffer.
            int samplesLeftToPlay = LayersViewPort.LayersContainer.Layers[activeLayerIndexes[0]].LayerWave.playBuffer.getNumSamples() - LayersViewPort.LayersContainer.Layers[activeLayerIndexes[0]].LayerWave.playPos;
            if (samplesLeftToPlay > 0) {
                for (int ch = 0; ch < numChannels; ch++) {
                    outBuffer.copyFrom(ch,                                                              //  destination buffer channel index
                        0,                                                                              //  sample offset in output buffer
                        LayersViewPort.LayersContainer.Layers[activeLayerIndexes[0]].LayerWave.playBuffer,       //  source buffer
                        ch % numChannels,                                                               //  channel of input buffer
                        LayersViewPort.LayersContainer.Layers[activeLayerIndexes[0]].LayerWave.playPos,          //  start copy position in input buffer
                        std::min(lengthInSamples, samplesLeftToPlay));                                                               //  number of samples to copy
                }
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


        // revert gain application and pan for all active layers
        applyGainForAllTracks(lengthInSamples, numActiveLayers, activeLayerIndexes, false);

        if (state == Export)
        {
            //copy outBuffer to exportBuffer
            for (int ch = 0; ch < numChannels; ch++) {
                exportBuffer.copyFrom(ch,                                               //  destination buffer channel index
                    int(samplesWritten),                                                     //  sample offset in output buffer
                    outBuffer,                                                          //  source buffer
                    ch % numChannels,                                                   //  channel of input buffer
                    0,                                                                  //  start copy position in input buffer
                    lengthInSamples);                                                   //  number of samples to copy
            }
            
            samplesWritten += lengthInSamples;

            if (samplesWritten >= globalSampleRate*exportTime) {
                // this ends recording and the recording will be saved
                toggleExportState();
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


void MainComponent::applyGainForAllTracks(int lengthInSamples, int numActiveLayers, int activeLayerIndexes[], bool applyRevert) {
    for (int i = 0; i < numActiveLayers; i++) {
        float gain, pan;
        gain = LayersViewPort.LayersContainer.Layers[activeLayerIndexes[i]].LayerControl.gain;
        pan = 2 * (LayersViewPort.LayersContainer.Layers[activeLayerIndexes[i]].LayerControl.pan - 0.5);
        int samplesLeftToPlay = LayersViewPort.LayersContainer.Layers[activeLayerIndexes[0]].LayerWave.playBuffer.getNumSamples() - LayersViewPort.LayersContainer.Layers[activeLayerIndexes[0]].LayerWave.playPos;

        if (samplesLeftToPlay > 0) {
            int playPos;
            playPos = LayersViewPort.LayersContainer.Layers[activeLayerIndexes[i]].LayerWave.playPos;
            for (int ch = 0; ch < 2; ch++) {
                double gainFactor = 2 * (1 - (std::pow(-1, ch)) * pan);
                if (applyRevert) //apply
                    LayersViewPort.LayersContainer.Layers[activeLayerIndexes[i]].LayerWave.playBuffer.applyGain(ch, playPos, std::min(lengthInSamples, samplesLeftToPlay), (gainFactor * gain));
                else //revert
                    LayersViewPort.LayersContainer.Layers[activeLayerIndexes[i]].LayerWave.playBuffer.applyGain(ch, playPos - lengthInSamples, std::min(lengthInSamples, samplesLeftToPlay), 1 / (gainFactor * gain));
            }
        }
    }
}

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
                ControlBar.recordButton.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
                DBG("state = stop");
                break;
            case Play:
                ControlBar.bPlay.setEnabled(false);
                ControlBar.bPlay.setToggleState(true, juce::NotificationType::dontSendNotification);
                ControlBar.bPause.setEnabled(true);
                ControlBar.bPause.setToggleState(false, juce::NotificationType::dontSendNotification);
                ControlBar.bStop.setEnabled(true);
                ControlBar.bStop.setToggleState(false, juce::NotificationType::dontSendNotification);
                ControlBar.recordButton.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
                DBG("state = play");
                break;
            case Pause:
                ControlBar.bPlay.setEnabled(true);
                ControlBar.bPlay.setToggleState(false, juce::NotificationType::dontSendNotification);
                ControlBar.bPause.setEnabled(false);
                ControlBar.bPause.setToggleState(true, juce::NotificationType::dontSendNotification);
                ControlBar.bStop.setEnabled(true);
                ControlBar.bStop.setToggleState(false, juce::NotificationType::dontSendNotification);
                ControlBar.recordButton.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
                DBG("state = pause");
                break;
            case Export:
                ControlBar.bPlay.setEnabled(false);
                ControlBar.bPlay.setToggleState(true, juce::NotificationType::dontSendNotification);
                ControlBar.bPause.setEnabled(true);
                ControlBar.bPause.setToggleState(false, juce::NotificationType::dontSendNotification);
                ControlBar.bStop.setEnabled(true);
                ControlBar.bStop.setToggleState(false, juce::NotificationType::dontSendNotification);
                ControlBar.recordButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
                DBG("state = export");
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

void MainComponent::toggleExportState()
{
    if (state == Export)
    {
        transportStateChanged(Stop);
        exportAudioToFile(exportBuffer);
    }
    else
    {
        samplesWritten = 0;
        exportBuffer.setSize(outBuffer.getNumChannels(), globalSampleRate*exportTime);

        transportStateChanged(Export);
    }
}
void MainComponent::exportAudioToFile(juce::AudioBuffer<float> buffer)
{
    DBG("exportAudioFile()");
    
    juce::FileChooser chooser("Aufnahme speichern", {}, "*.wav");
    juce::File file;
    file.create();
    if (chooser.browseForDirectory())
    {
        file = chooser.getResult();
        
        // AlertWindow with TextInput for FileName
        
        alertWindow = std::make_unique<juce::AlertWindow>("Alert",
                                                          "Choose an Option",
                                                          juce::MessageBoxIconType::WarningIcon,
                                                          nullptr);
        
        alertWindow->addAndMakeVisible(alertTextEditor);
        alertTextEditor.setBounds(10, 60, 300, 24);
        
        alertWindow->showOkCancelBox(alertWindow->InfoIcon,
                                    "export",
                                    "filename:",
                                    "OK",
                                    "Abbrechen",
                                    this,
                                    nullptr
                                    );
        
        juce::String fileName = alertTextEditor.getText();
        if (fileName != "")
        {
        juce::String filePath = file.getFullPathName()+"/"+fileName+"i.wav";
        DBG(filePath);
        
        juce::WavAudioFormat format;
        std::unique_ptr<juce::AudioFormatWriter> writer;
        writer.reset (format.createWriterFor (new juce::FileOutputStream (filePath),    // output Stream
                                              globalSampleRate,                         // sampleRate
                                              buffer.getNumChannels(),                  // num of channels
                                              24,                                       // bits per sample
                                              {},                                       // metadataValues
                                              0));                                      // qualityOptionIndex
        if (writer != nullptr)
                writer->writeFromAudioSampleBuffer (buffer, 0, int(samplesWritten));
            
        }
        else
        {
            DBG("incorrect fileName");
        }
    }
    else
    {
        DBG("export failed");
    }
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
