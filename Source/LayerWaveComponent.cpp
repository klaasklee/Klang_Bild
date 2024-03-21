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
    // movePlayhead
    addMouseListener(this, true);
    
    // load audio functionality
    fileLoaded = false;
    openButton.onClick = [this] { openButtonClicked(); };
    addAndMakeVisible(&openButton);
    
    formatManager.registerBasicFormats(); //now we can read wav and aiff formats
}

LayerWaveComponent::~LayerWaveComponent()
{
}

//draws Waveform
void LayerWaveComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::darkgrey);   // clear the background

    if (updateWaveform < 1 && fileLoaded)
    {
        p.clear();
        audioPoints.clear();
        int ratio = ((globalSampleRate*timeLineSize)/(getWidth()-waveBorder*2));
        auto buffer = playBuffer.getReadPointer(0);

        DBG("update waveform");
        //DBG(playBuffer.getNumSamples());
        //DBG(ratio);


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
        g.fillRoundedRectangle(rect, 3);
        
        g.setColour(juce::Colours::white);
        g.strokePath(p, juce::PathStrokeType(2));
        
        g.setColour(juce::Colours::white);
        g.drawRoundedRectangle(rect, 7, 3);

    }
    else if (fileLoaded)
    {
        g.setColour(juce::Colours::black);
        g.fillRoundedRectangle(rect, 3);
        
        g.setColour(juce::Colours::white);
        g.strokePath(p, juce::PathStrokeType(2));
        
        g.setColour(juce::Colours::white);
        g.drawRoundedRectangle(rect, 7, 3);
        
    }
    
    
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), waveBorder);   // draw an outline around the component
    updateWaveform++;
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


            fileLoaded = true;

            findParentComponentOfClass<MainComponent>()->transportStateChanged(MainComponent::Stop);

            //          setAudioChannels (0, (int) reader->numChannels);

            DBG(reader->getFormatName());

            blockDrag = false;

            updateWaveform = 0;

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
        
        // todo: nur wenn das sample nicht komplett aus dem fenster verschwindet verschieben
    
        playOffsetInPx = playOffsetInPx + distance;
        
        int ratio = ((timeLineSize * globalSampleRate) / (getWidth() - waveBorder * 2));
        playOffsetInSamples = -playOffsetInPx * ratio;
        
        updateWaveform = 0;
        repaint();
        
        boolMouseDrag = false;
    }
}

void LayerWaveComponent::resized()
{
    updateWaveform = 1;
    openButton.setBounds(getWidth()-85-waveBorder, 5+waveBorder, 80, 30);
}
