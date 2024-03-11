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
            
            // todo: resample to correct samplerate before loading to RAM
            
            reader->read(&fileBuffer,
                0,                              //destination start sample                                 
                (int)reader->lengthInSamples,   //numSamples                               
                0,                              //reader start sample                                  
                true,                           //use right channel                                
                true);                          //use left channel     

            auto numChannels = fileBuffer.getNumChannels();

            //copy audio to playBuffer, with zero shift initially
            for (int ch = 0; ch < numChannels; ch++) {
                playBuffer.copyFrom(ch,            //  destination buffer channel index
                    0,                             //  sample offset in output buffer
                    fileBuffer,                    //  source buffer
                    ch % numChannels,              //  channel of input buffer
                    0,                             //  start copy position in input buffer
                    lengthInSamples);              //  number of samples to copy
            }
            fileLoaded = true; 


            findParentComponentOfClass<MainComponent>()->transportStateChanged(MainComponent::Stopped);
            
 //          setAudioChannels (0, (int) reader->numChannels);
            
            DBG(reader->getFormatName());
        }
    }
}
