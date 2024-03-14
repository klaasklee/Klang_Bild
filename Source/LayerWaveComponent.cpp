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
LayerWaveComponent::LayerWaveComponent() : openButton("open file (WAV, MP3)")
{
    fileLoaded = false; 
    openButton.onClick = [this] { openButtonClicked(); };
    addAndMakeVisible(&openButton);
    
    formatManager.registerBasicFormats(); //now we can read wav and aiff formats
}

LayerWaveComponent::~LayerWaveComponent()
{
}

void LayerWaveComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 15);   // draw an outline around the component
}

void LayerWaveComponent::resized()
{
    openButton.setBounds(getWidth()/2-75, getHeight()/2-15, 150, 30);
}

void LayerWaveComponent::openButtonClicked()
{
    DBG("open File clicked");
    
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
            duration = (float) reader->lengthInSamples / reader->sampleRate;
            
            // save audio to Audio buffer
            int lengthInSamples = (int)reader->lengthInSamples;
            fileBuffer.setSize((int)reader->numChannels, lengthInSamples);
            playBuffer.setSize((int)reader->numChannels, lengthInSamples);
            playPos = 0;
            playOffset = 0;
            
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
