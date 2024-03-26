/*
  ==============================================================================

    LayerWaveComponent.cpp
    Created: 9 Feb 2024 10:40:04am
    Author:  Klaas Klee

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LayerWaveComponent.h"
#include "MainComponent.h"
#include "Globals.h"

//==============================================================================
LayerWaveComponent::LayerWaveComponent() : openButton("import audio (WAV, MP3)")
{
    // BG
    imgBG = juce::ImageCache::getFromMemory(BinaryData::bgLayerControlAndWave_2_png, BinaryData::bgLayerControlAndWave_2_pngSize);
    
    // movePlayhead
    addMouseListener(this, true);
    
    // load audio functionality
    fileLoaded = false;
    openButton.onClick = [this] { openButtonClicked(); };
    openButton.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    openButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::black);
    openButton.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    openButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    openButton.setLookAndFeel(&LookAndFeel002);
    addAndMakeVisible(&openButton);
    
    formatManager.registerBasicFormats(); //now we can read wav and aiff formats
}

LayerWaveComponent::~LayerWaveComponent()
{
}

//draws Waveform
void LayerWaveComponent::paint (juce::Graphics& g)
{
//    g.fillAll (GlobalColors::layerWaveBg);   // clear the background
    
//    g.setColour (GlobalColors::bG);
//    g.drawRect (getLocalBounds(), waveBorder);   // draw an outline around the component
    
    // Dropshadow on waveBorder
    juce::DropShadow dropShadow(GlobalColors::dropShadow, 10, juce::Point<int>(0, 5));
    dropShadow.drawForRectangle(g, juce::Rectangle<int>(waveBorder, waveBorder, getWidth()-waveBorder*2, layerHeight-waveBorder*2));
    
    juce::Rectangle<int> waveBG;
    waveBG.setBounds(getLocalBounds().getX()+waveBorder, getLocalBounds().getY()+waveBorder, getLocalBounds().getWidth()-waveBorder*2, getLocalBounds().getHeight()-waveBorder*2);
    g.setColour(GlobalColors::layerWaveBg);
    g.fillRect(waveBG);
    // Draw BG
    g.reduceClipRegion(waveBG);
    g.drawImage(imgBG, waveBG.toFloat(), juce::RectanglePlacement::fillDestination, false);
    
    juce::Rectangle<int> clipRegion = getLocalBounds().reduced(waveBorder); // Define the area to cut off shapes
    // Reduce the clipping region to the specified rectangle
    g.reduceClipRegion(clipRegion);
    
    if (updateWaveform && fileLoaded)
    {
        p.clear();
        audioPoints.clear();
        int timeLineSize = findParentComponentOfClass<MainComponent>()->timeLineSize;
        int ratio = ((globalSampleRate*timeLineSize)/(getWidth()-waveBorder*2));
        auto buffer = playBuffer.getReadPointer(0);
        
        DBG("update waveform");

        // scale audio on x axis
        for (int sample = 0; sample < playBuffer.getNumSamples(); sample += ratio)
        {
            audioPoints.push_back(buffer[sample]);
        }
        
        p.startNewSubPath(playOffsetInPx+waveBorder, getHeight()/2);
        
        // scale audio on y axis
        for (int sample = 0; sample < audioPoints.size(); ++sample)
        {
            auto point = juce::jmap<float>(audioPoints[sample], -1.0f, 1.0f, getHeight()-waveBorder, 0+waveBorder);
            p.lineTo(sample+waveBorder+playOffsetInPx, point);
        }

        rect.setBounds(waveBorder+playOffsetInPx, waveBorder, p.getBounds().getRight()-playOffsetInPx-waveBorder, getHeight()-waveBorder*2);

        g.setColour(juce::Colours::black);
//        g.fillRoundedRectangle(rect, 3);
        g.fillRect(rect.expanded(3));
        
        g.setColour(GlobalColors::layerWaveBg);
        g.strokePath(p, juce::PathStrokeType(2));
        
//        g.setColour(juce::Colours::black);
//        g.drawRoundedRectangle(rect, 7, 3);
//        g.drawRect(rect.expanded(0.5), 3);
        
        g.setColour(GlobalColors::white);
        if (playOffsetInPx <= 0)
        {
            g.drawText(fileName, waveBorder+5, 15, p.getBounds().getRight()-waveBorder-7, 20, juce::Justification::left);
        }
        else
        {
            g.drawText(fileName, waveBorder+playOffsetInPx+5, 15, p.getBounds().getRight()-playOffsetInPx-waveBorder-7, 20, juce::Justification::left);
        }
    }
    else if (fileLoaded)
    {
        g.setColour(juce::Colours::black);
//        g.fillRoundedRectangle(rect, 3);
        g.fillRect(rect.expanded(3));
        
        g.setColour(GlobalColors::layerWaveBg);
        g.strokePath(p, juce::PathStrokeType(2));
        
//        g.setColour(juce::Colours::black);
//        g.drawRoundedRectangle(rect, 7, 3);
//        g.drawRect(rect.expanded(0.5), 3);

        g.setColour(GlobalColors::white);
        if (playOffsetInPx <= 0)
        {
            g.drawText(fileName, waveBorder+5, 15, p.getBounds().getRight()-waveBorder-7, 20, juce::Justification::left);
        }else
        {
            g.drawText(fileName, waveBorder+playOffsetInPx+5, 15, p.getBounds().getRight()-playOffsetInPx-waveBorder-7, 20, juce::Justification::left);
        }
    }
    updateWaveform = false;
}

void LayerWaveComponent::openButtonClicked()
{
//    DBG("openButton");
    blockDrag = true;
    importAudio();
}

void LayerWaveComponent::importAudio()
{
    DBG("importAudio()");
    
//    findParentComponentOfClass<MainComponent>()->shutdownAudio();
    
    //choose file
    juce::FileChooser chooser ("choose a wav or mp3 file", juce::File{}, "*.wav; *.mp3");
    
    //if user chooses file
    if (chooser.browseForFileToOpen())
    {
        juce::File file;
        
        //what did the user choose?
        file = chooser.getResult();
        //read file
        juce::AudioFormatReader* reader = formatManager.createReaderFor(file);
        
        if (reader != nullptr)
        {
            duration = (float)reader->lengthInSamples / reader->sampleRate;

            // save audio to Audio buffer
            int lengthInSamples = (int)reader->lengthInSamples;
            fileBuffer.setSize((int)reader->numChannels, lengthInSamples);
            playBuffer.setSize((int)reader->numChannels, lengthInSamples);
            playPos = 0;
            playOffsetInPx = 0;
            playOffsetInSamples = 0;

            //reads data into the fileBuffer
            reader->read(&fileBuffer,
                0,                              //destination start sample
                (int)reader->lengthInSamples,   //numSamples
                0,                              //reader start sample
                true,                           //use right channel
                true);                          //use left channel


            // todo: resample to correct samplerate before loading to RAM


            auto numChannels = fileBuffer.getNumChannels();

            //copy audio from fileBuffer to playBuffer, with zero shift initially
            for (int ch = 0; ch < numChannels; ch++) {
                playBuffer.copyFrom(ch,            //  destination buffer channel index
                    0,                             //  sample offset in output buffer
                    fileBuffer,                    //  source buffer
                    ch % numChannels,              //  channel of input buffer
                    0,                             //  start copy position in input buffer
                    lengthInSamples);              //  number of samples to copy
            }

            fileName = file.getFileName();
            fileLoaded = true;

            findParentComponentOfClass<MainComponent>()->transportStateChanged(MainComponent::Stop);

            //          setAudioChannels (0, (int) reader->numChannels);

            DBG(reader->getFormatName());
            DBG(file.getFileName());

            blockDrag = false;

            updateWaveform = true;

            repaint();
        }
    }
}

void LayerWaveComponent::resampleAudioBuffer(juce::AudioBuffer<float>& srcBuffer, juce::AudioBuffer<float>& destBuffer, float srcSampleRate, float destSampleRate)
{
    
/*
 dosnt work right now; just use the same sampleRate for yor import files
 
 todo:
 use oversampledBuffer to resample manually
" Wouldn’t have thought it’s difficult to do though."
 */
    
//    // Spezifikationen für die Eingangs- und Ausgangsrate
//        juce::dsp::ProcessSpec srcSpec;
//        srcSpec.sampleRate = srcSampleRate;
//        srcSpec.maximumBlockSize = srcBuffer.getNumSamples();
//        srcSpec.numChannels = srcBuffer.getNumChannels();
//
//        // Resampling-Verhältnis
//        double ratio = globalSampleRate / srcSpec.sampleRate;
//
//        // Resampler erstellen
//        juce::dsp::Oversampling<float> oversampling{ srcSpec.numChannels, 4, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR, false };
//        oversampling.initProcessing(srcSpec.maximumBlockSize);
//
//        // Eingangs- und Ausgangsblöcke
//        juce::dsp::AudioBlock<float> srcBlock(srcBuffer);
//        juce::dsp::AudioBlock<float> destBlock(destBuffer);
//
//        // Resampling durchführen
//        oversampling.processSamplesUp(srcBlock);
//        oversampling.processSamplesDown(destBlock);
//
//        // Sicherstellen, dass der Ausgangs-Puffer die richtige Anzahl von Samples hat
//        destBuffer.setSize(destBuffer.getNumChannels(), static_cast<int>(destBuffer.getNumSamples()), false, false, true);
}

void LayerWaveComponent::mouseDrag(const juce::MouseEvent& event)
{
//    DBG("mouse drag");
    if (!boolMouseDrag && !blockDrag)
    {
        mouseDownX = event.getMouseDownX();
        boolMouseDrag = true;
    }
}
void LayerWaveComponent::mouseUp(const juce::MouseEvent& event)
{
//    DBG("mouse up");
    if (boolMouseDrag)
    {
        DBG("move waveform");
//        DBG(mouseDownX);
//        DBG(event.getPosition().getX());
        
        int distance = event.getPosition().getX() - mouseDownX;
//        DBG("distance: ");
//        DBG(distance);
    
        playOffsetInPx = playOffsetInPx + distance;
        
        int timeLineSize = findParentComponentOfClass<MainComponent>()->timeLineSize;
        int ratio = ((timeLineSize * globalSampleRate) / (getWidth() - waveBorder * 2));
        playOffsetInSamples = -playOffsetInPx * ratio;
        
        updateWaveform = true;
        repaint();
        
        boolMouseDrag = false;
    }
}

void LayerWaveComponent::resized()
{
    updateWaveform = true;
    openButton.setBounds(getWidth()-85-waveBorder, 5+waveBorder, 80, 30);
}
