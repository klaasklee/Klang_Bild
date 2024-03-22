#include "MainComponent.h"
#include "Globals.h"

//==============================================================================
MainComponent::MainComponent() : SetTimeLineSizeAlertWindow("OK", "CANCEL", "Timeline Size:", "in seconds:  ", AlertWindows::SetTimeLineSizeWindow), ExportAlertWindow("OK", "CANCEL", "Export:", "filename: ", AlertWindows::ExportWindow)
{
    setSize (1200, 800);
    
    timeLineSize = timeLineSizeOnStartUp;

    addAndMakeVisible(ControlBar);
    addAndMakeVisible(PlayHeadRuler);
    addAndMakeVisible(LayersViewPort);
    
//    PlayHead.setBounds(getWidth(), getHeight()/5, 2, getHeight() -  getHeight()/5);
    addAndMakeVisible(PlayHead);
    
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

// initialize the static filter 
juce::dsp::StateVariableTPTFilter<float> MainComponent::filter;

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    outBuffer.setSize(2, samplesPerBlockExpected);
    transportStateChanged(Stop);

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlockExpected;
    spec.numChannels = globalNumChannels;
    spec.sampleRate = sampleRate;
    filter.prepare(spec);
    filter.reset();
    

}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (state == Play || state == Export) {

        bufferToFill.clearActiveBufferRegion();

        auto numChannels = bufferToFill.buffer->getNumChannels();
        auto lengthInSamples = bufferToFill.numSamples;
//        DBG(lengthInSamples);
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

            outBuffer.clear();



            // calculate blend modes, if there are at least two active layers
            // for only one active layer, the loop will be skipped
            for (int i = 0; i < numActiveLayers; i++) {
                if (!LayersViewPort.LayersContainer.Layers[activeLayerIndexes[i]].LayerControl.layerMute) {
                    functionPointerType calcBlendMode = getBlendModeFct(LayersViewPort.LayersContainer.Layers[activeLayerIndexes[i]].LayerControl.selectedBlendMode); // get the blend mode of the second to last layer, the blend mode of the last layer is always ignored
                    calcBlendMode(outBuffer,
                        LayersViewPort.LayersContainer.Layers[activeLayerIndexes[i]],
                        outBuffer,
                        lengthInSamples,
                        nullInt,
                        (playPosInSamples + LayersViewPort.LayersContainer.Layers[activeLayerIndexes[i]].LayerWave.playOffsetInSamples));
                }
            }


            //copy the calculation result to output (bufferToFill)
            for (int ch = 0; ch < numChannels; ch++) {
                bufferToFill.buffer->copyFrom(ch,                                       //  destination buffer channel index
                    0,                                                                  //  sample offset in output buffer
                    outBuffer,                                                          //  source buffer
                    ch % numChannels,                                                   //  channel of input buffer
                    0,                                                                  //  start copy position in input buffer
                    lengthInSamples);                                                   //  number of samples to copy                
            }
            bufferToFill.buffer->applyGain(0, lengthInSamples, globalVolume);
            
            // maoves the playPosition one bufferlenght forward
            if (playPosInSamples > (timeLineSize * globalSampleRate) - lengthInSamples) {
                if (globalLoop) {
                    playPosInSamples = 0;
                }
                else {
                    // calls transportStateChanged() asyncon bec og changing some GUI elements etc...
                    juce::MessageManager::callAsync([=]()
                    {
                        transportStateChanged(Stop);
                    });
                }                
            }
            else {
                playPosInSamples += lengthInSamples;
            }

            // update Timecode + PlayHead
            juce::MessageManager::callAsync([=]()
            {
                int ratio = ((timeLineSize*globalSampleRate)/(LayersViewPort.LayersContainer.Layers[0].LayerWave.getWidth()-waveBorder*2));
                playHeadPos = playPosInSamples/ratio;
                
                // setTimeCode
                ControlBar.lTimeCode.setText(juce::String::formatted("%.3f", playPosInSamples/globalSampleRate), juce::dontSendNotification);
                // setPlayHead
                PlayHead.setBounds(layerControlW+waveBorder+playHeadPos, getHeight()/5, 2, getHeight() -  getHeight()/5);
            });
        }

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
        
        // update AudioMeter
        ControlBar.AudioMeter.updateAudioMeter(bufferToFill);
    }


    if (state == Stop)
    {
//        // find the last active layer, and the number of active layers
//        for (int layerCounter = numOfLayers - 1; layerCounter >= 0; layerCounter--)
//        {
//        }
        
        int ratio = ((timeLineSize * globalSampleRate) / (LayersViewPort.LayersContainer.Layers[0].LayerWave.getWidth() - waveBorder * 2));
        playPosInSamples = playHeadStartPos * ratio;
        
        // update Timecode + PlayHead
        juce::MessageManager::callAsync([=]()
            {
                // setTimeCode
                ControlBar.lTimeCode.setText(juce::String::formatted("%.3f", playPosInSamples/globalSampleRate), juce::dontSendNotification);
                // setPlayHead
                PlayHead.setBounds(layerControlW+waveBorder+playHeadStartPos, getHeight()/5, 2, getHeight() -  getHeight()/5);
            });
    }

    if (state == Pause)
    {
        int ratio = ((timeLineSize * globalSampleRate) / (LayersViewPort.LayersContainer.Layers[0].LayerWave.getWidth() - waveBorder * 2));
        playPosInSamples = playHeadPos * ratio;
        
        // update Timecode + PlayHead
        juce::MessageManager::callAsync([=]()
            {
                // setTimeCode
                ControlBar.lTimeCode.setText(juce::String::formatted("%.3f", playPosInSamples/globalSampleRate), juce::dontSendNotification);
                // setPlayHead
                PlayHead.setBounds(layerControlW+waveBorder+playHeadPos, getHeight()/5, 2, getHeight() -  getHeight()/5);
            });
        
    }
    
}


void MainComponent::blendModeAdd(juce::AudioSampleBuffer& layerA, LayerComponent& layerB, juce::AudioSampleBuffer& outLayer, int numSamples, int playPosA, int playPosB)
{
    int numChannelsA = layerA.getNumChannels();
    int numChannelsB = layerB.LayerWave.playBuffer.getNumChannels();

    const float* readA;
    const float* readB;
    float* writeOut;

    jassert (numChannelsA == numChannelsB);

    int samplesLeftToPlay = layerB.LayerWave.playBuffer.getNumSamples() - playPosB; // we only need to check B, because A is always the outBuffer which is exactly the size of the Block

    for (int ch = 0; ch < numChannelsA; ch++)
    {
        float gain = layerB.LayerControl.channelGain[ch];
        if (samplesLeftToPlay > 0 && playPosB >= 0)
        {
            readB = layerB.LayerWave.playBuffer.getReadPointer(ch, playPosB);
            writeOut = outLayer.getWritePointer(ch);
            readA = layerA.getReadPointer(ch, playPosA);

            for (int i = 0; i < numSamples; i++) {
                // perform adding
                *writeOut++ = *readA++ + ((i < samplesLeftToPlay) ? ((*readB++) * gain) : 0);
            }
        }
        else { // if one track is finished, just copy the other one to output
            writeOut = outLayer.getWritePointer(ch);
            readA = layerA.getReadPointer(ch, playPosA);

            for (int i = 0; i < numSamples; i++) {
                *writeOut++ = *readA++;
            }
        }

    }
    // do not increment playPosA as it is always the outBuffer!
    //playPosA += numSamples;
    //playPosB += numSamples;
}

void MainComponent::blendModeMult(juce::AudioSampleBuffer& layerA, LayerComponent& layerB, juce::AudioSampleBuffer& outLayer, int numSamples, int playPosA, int playPosB)
{
    int numChannelsA = layerA.getNumChannels();
    int numChannelsB = layerB.LayerWave.playBuffer.getNumChannels();

    const float* readA;
    const float* readB;
    float* writeOut;

    jassert(numChannelsA == numChannelsB);

    int samplesLeftToPlay = layerB.LayerWave.playBuffer.getNumSamples() - playPosB; // we only need to check B, because A is always the outBuffer which is exactly the size of the Block

    for (int ch = 0; ch < numChannelsA; ch++)
    {
        float gain = layerB.LayerControl.channelGain[ch];
        if (samplesLeftToPlay > 0 && playPosB >= 0)
        {
            readA = layerA.getReadPointer(ch, playPosA);
            readB = layerB.LayerWave.playBuffer.getReadPointer(ch, playPosB);
            writeOut = outLayer.getWritePointer(ch);

            for (int i = 0; i < numSamples; i++) {
                *writeOut++ = (*readA++) * (*readB++) * gain;
            }
        }
        else { // if one track is finished, just copy the other one to output
            readA = layerA.getReadPointer(ch, playPosA);
            writeOut = outLayer.getWritePointer(ch);

            for (int i = 0; i < numSamples; i++) {
                *writeOut++ = *readA++;
            }
        }
    }
    //playPosA += numSamples;
    //playPosB += numSamples;
}

void MainComponent::blendModeDuck(juce::AudioSampleBuffer& layerA, LayerComponent& layerB, juce::AudioSampleBuffer& outLayer, int numSamples, int playPosA, int playPosB)
{
    // computes the average power per block of the active layer of interest (layerB), which has this blendMode selected. 
    // use this value, to scale the other oudio (layerA) per block
    // todo: interpolate smoothly between blocks

    int numChannelsA = layerA.getNumChannels();
    int numChannelsB = layerB.LayerWave.playBuffer.getNumChannels();

    const float intensity = 10;
    const float offset = 0.001;

    const float* readA;
    const float* readB;
    float* writeOut;

    jassert(numChannelsA == numChannelsB);

    int samplesLeftToPlay = layerB.LayerWave.playBuffer.getNumSamples() - playPosB; // we only need to check B, because A is always the outBuffer which is exactly the size of the Block

    for (int ch = 0; ch < numChannelsA; ch++)
    {
        float gain = layerB.LayerControl.channelGain[ch];
        if (samplesLeftToPlay > numSamples && playPosB >= 0)
        {
            readA = layerA.getReadPointer(ch, playPosA);
            readB = layerB.LayerWave.playBuffer.getReadPointer(ch, playPosB);
            writeOut = outLayer.getWritePointer(ch);

            // calculate the mean power of this block and the next 
            double maxPowerB = 0;
            double powerB = 0;
            double powerBNextBlock = 0;
            for (int i = 0; i < numSamples; i++) {
                powerB += (*readB) * ((*readB++));
            }
            for (int i = 0; i < numSamples; i++) {
                powerBNextBlock += (*readB) * ((*readB++));
            }
            powerB /= numSamples;
            powerBNextBlock /= numSamples;

            // scale the buffer A with the power of buffer B; interpolate linearly to the next block power. 
            for (int i = 0; i < numSamples; i++) {
                double x;
                x = i * ((powerBNextBlock - powerB) / numSamples);
                *writeOut++ = (*readA++) * (((powerB + x) + offset) * intensity * gain);
                //*writeOut++ = std::sqrt(((*readA) * (*readA++)) - 0*(((powerB + x) + offset) * intensity * gain));
                //*writeOut++ = std::sqrt(*readA * (*readA));
                //readA++;
            }
        }
        else { // if one track is finished, just copy the other one to output
            readA = layerA.getReadPointer(ch, playPosA);
            writeOut = outLayer.getWritePointer(ch);

            for (int i = 0; i < numSamples; i++) {
                *writeOut++ = *readA++;
            }
        }
    }
    //playPosA += numSamples;
    //playPosB += numSamples;
}

void MainComponent::blendModeBinary(juce::AudioSampleBuffer& layerA, LayerComponent& layerB, juce::AudioSampleBuffer& outLayer, int numSamples, int playPosA, int playPosB)
{
    int numChannelsA = layerA.getNumChannels();
    int numChannelsB = layerB.LayerWave.playBuffer.getNumChannels();

    const float* readA;
    const float* readB;
    float* writeOut;

    jassert(numChannelsA == numChannelsB);

    int samplesLeftToPlay = layerB.LayerWave.playBuffer.getNumSamples() - playPosB; // we only need to check B, because A is always the outBuffer which is exactly the size of the Block

    for (int ch = 0; ch < numChannelsA; ch++)
    {
        float gain = layerB.LayerControl.channelGain[ch];
        if (samplesLeftToPlay > 0 && playPosB >= 0)
        {
            readB = layerB.LayerWave.playBuffer.getReadPointer(ch, playPosB);
            writeOut = outLayer.getWritePointer(ch);
            readA = layerA.getReadPointer(ch, playPosA);

            for (int i = 0; i < numSamples; i++) {
                // perform adding
                float accuracyFactor = 1e6;
                long a = (long) (accuracyFactor  * (*readA++));
                long b = (long) (accuracyFactor  * (*readB++) * gain);
                *writeOut++ = (float)((a ^ ((i < samplesLeftToPlay) ? (b) : 0)) / accuracyFactor);
            }
        }
        else { // if one track is finished, just copy the other one to output
            writeOut = outLayer.getWritePointer(ch);
            readA = layerA.getReadPointer(ch, playPosA);

            for (int i = 0; i < numSamples; i++) {
                *writeOut++ = *readA++;
            }
        }

    }
    // do not increment playPosA as it is always the outBuffer!
    //playPosA += numSamples;
    //playPosB += numSamples;
}

void MainComponent::blendModeVariableFilter(juce::AudioSampleBuffer& layerA, LayerComponent& layerB, juce::AudioSampleBuffer& outLayer, int numSamples, int playPosA, int playPosB)
{

    // todo: inverted option; other filter modes;

    int numChannelsA = layerA.getNumChannels();
    int numChannelsB = layerB.LayerWave.playBuffer.getNumChannels();

    const float* readA;
    const float* readB;
    float* writeOut;
    double powerB = 0;

    float freqRange = 5000; // used to scale the affected frequency range
    float freqOffset = 30;
    float filterCutoff;

    jassert(numChannelsA == numChannelsB);

    int samplesLeftToPlay = layerB.LayerWave.playBuffer.getNumSamples() - playPosB; // we only need to check B, because A is always the outBuffer which is exactly the size of the Block


    if (samplesLeftToPlay > 0 && playPosB >= 0)
    {
        for (int ch = 0; ch < numChannelsB; ch++)
        {
            float gain = layerB.LayerControl.channelGain[ch];
            readB = layerB.LayerWave.playBuffer.getReadPointer(ch, playPosB);
            for (int i = 0; i < numSamples; i++) {
                powerB += (*readB) * ((*readB++)) * gain * gain;
            }
            
        }
        powerB /= (numChannelsB * numSamples);
    }

    filterCutoff = powerB * freqRange + freqOffset;
    filterCutoff = (filterCutoff > globalSampleRate / 2) ? (globalSampleRate / 2) : filterCutoff; // check if cutoff is below nyquist

    if (samplesLeftToPlay > numSamples) {
        //auto inputAudioBlock = juce::dsp::AudioBlock<float>(layerA);
        auto outputAudioBlock = juce::dsp::AudioBlock<float>(outLayer);
        auto context = juce::dsp::ProcessContextReplacing<float>(outputAudioBlock);
        filter.setResonance(0.95);
        filter.setCutoffFrequency(filterCutoff);
        filter.process(context);
    }

}
void MainComponent::setFilterType() 
{
    using fType = juce::dsp::StateVariableTPTFilterType;

    switch (filterType) {
    case FilterType::Lowpass:
        filter.setType(fType::lowpass);
        break;
    case FilterType::Bandpass:
        filter.setType(fType::bandpass);
        break;
    case FilterType::Highpass:
        filter.setType(fType::highpass);
        break;
    default: 
        filter.setType(fType::lowpass);
    }
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
                
                // reset AudioMeter
                ControlBar.AudioMeter.resetAudioMeter();
                
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
                
                // reset AudioMeter
                ControlBar.AudioMeter.resetAudioMeter();
                
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
        prepareAudioExport();
    }
    else
    {
        samplesWritten = 0;
        exportBuffer.setSize(outBuffer.getNumChannels(), globalSampleRate*exportTime);

        transportStateChanged(Export);
    }
}

void MainComponent::prepareAudioExport()
{
    ExportAlertWindow.setBounds(getWidth()/2-150, getHeight()/2-150, 300, 300);
    addAndMakeVisible(ExportAlertWindow);
}

void MainComponent::exportAudioToFile(juce::AudioBuffer<float> &buffer, juce::String fileName)
{
    DBG("exportAudioToFile()");
    
    if (fileName != "")
    {
        juce::FileChooser chooser("Aufnahme speichern", {}, "*.wav");
        juce::File file;
        file.create();
        if (chooser.browseForDirectory())
        {
            file = chooser.getResult();
            
            juce::String filePath = file.getFullPathName()+"/"+fileName+".wav";
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
                {
                    writer->writeFromAudioSampleBuffer (buffer, 0, int(samplesWritten));
                }
            }
        else
        {
            DBG("export failed");
        }
    }
    else
    {
        DBG("incorrect fileName");
    }
}

void MainComponent::killExportAlertWindow()
{
    removeChildComponent(&ExportAlertWindow);
}

void MainComponent::setTimeLineSize(int i)
{
    timeLineSize = i;
    
    int ratio = ((timeLineSize * globalSampleRate) / (LayersViewPort.LayersContainer.Layers[0].LayerWave.getWidth() - waveBorder * 2));
    LayersViewPort.LayersContainer.Layers[0].LayerWave.playOffsetInPx = -LayersViewPort.LayersContainer.Layers[0].LayerWave.playOffsetInSamples / ratio;
    
    
    LayersViewPort.LayersContainer.Layers[0].LayerWave.updateWaveform = true;
    killExportAlertWindow();
}

void MainComponent::killSetTimeLineSizeAlertWindow()
{
    removeChildComponent(&SetTimeLineSizeAlertWindow);
}

void MainComponent::setPlayHeadPos(int pos)
{
//    DBG("setPlayHeadPos()");
//    DBG(pos);
    
    // sets Position of PlayHead
    // set Pos only works insite the actual drawn wave
    // when the edge is clicked, the playhead moves to the very left or right
    
//    transportStateChanged(Stop);
    
    int layersWaveBorder = waveBorder;
    int layersWidth = LayersViewPort.LayersContainer.Layers[0].LayerWave.getWidth();
    
    if (pos > layersWaveBorder && pos < layersWidth - layersWaveBorder)
    {
        playHeadStartPos = pos - layersWaveBorder;
        
        int ratio = ((timeLineSize * globalSampleRate) / (LayersViewPort.LayersContainer.Layers[0].LayerWave.getWidth() - waveBorder * 2));
        playPosInSamples = playHeadStartPos * ratio;
        
        PlayHead.setBounds(layerControlW + playHeadStartPos, getHeight()/5, 2, getHeight() -  getHeight()/5);
    }
    else if (pos < layersWaveBorder)
    {
        playHeadStartPos = 0;
        
        playPosInSamples = 0;
        
        PlayHead.setBounds(layerControlW + playHeadStartPos, getHeight()/5, 2, getHeight() -  getHeight()/5);
    }
    else if (pos > layersWidth - layersWaveBorder)
    {
        playHeadStartPos = layersWidth - (layersWaveBorder*2);
        
        int ratio = ((timeLineSize * globalSampleRate) / (LayersViewPort.LayersContainer.Layers[0].LayerWave.getWidth() - waveBorder * 2));
        playPosInSamples = playHeadStartPos * ratio;
        
        PlayHead.setBounds(layerControlW + playHeadStartPos, getHeight()/5, 2, getHeight() -  getHeight()/5);
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
}

void MainComponent::resized()
{
    ControlBar.setBounds(0, 0, getWidth(), getHeight()/5-30);
    PlayHeadRuler.setBounds(0, getHeight()/5-30, getWidth(), 30);
    LayersViewPort.setBounds(0, getHeight()/5, getWidth(), getHeight() -  getHeight()/5);
        
    ExportAlertWindow.setBounds(500, 500, 200, 200);
    
}
