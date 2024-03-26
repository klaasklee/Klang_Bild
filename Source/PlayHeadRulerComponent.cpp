/*
  ==============================================================================

    PlayHeadRulerComponent.cpp
    Created: 19 Mar 2024 8:51:20pm
    Author:  Klaas Klee

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlayHeadRulerComponent.h"
#include "Globals.h"
#include "MainComponent.h"

//==============================================================================
PlayHeadRulerComponent::PlayHeadRulerComponent() : bSetTimeLineSize("set Timeline length")

{
    // BG
    imgBG = juce::ImageCache::getFromMemory(BinaryData::bgPlayHeadRuler_2_jpg, BinaryData::bgPlayHeadRuler_2_jpgSize);
    
    bSetTimeLineSize.onClick = [this] { showSetTimeLineSizeAlertWindow(); };
    bSetTimeLineSize.setBounds(waveBorder, 6, layerControlW-waveBorder*2, 20);
    bSetTimeLineSize.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    bSetTimeLineSize.setLookAndFeel(&LookAndFeel001);
    bSetTimeLineSize.setColour(juce::TextButton::textColourOnId, GlobalColors::white);
    bSetTimeLineSize.setColour(juce::TextButton::textColourOffId, GlobalColors::white);
    addAndMakeVisible(&bSetTimeLineSize);
}


PlayHeadRulerComponent::~PlayHeadRulerComponent()
{
}

void PlayHeadRulerComponent::paint (juce::Graphics& g)
{
    g.fillAll (GlobalColors::playHeadRulerBG);
    // Draw BG
//    g.drawImage(imgBG, getLocalBounds().toFloat(), juce::RectanglePlacement::fillDestination, false);
    
    g.setColour(juce::Colours::black);
    
    int timeLineSize = findParentComponentOfClass<MainComponent>()->timeLineSize;
    
    int ratio = (globalSampleRate*timeLineSize)/(getWidth()-layerControlW-waveBorder*2);
    
    int big = 10;
    
    for (int sample = 0; sample <= timeLineSize*globalSampleRate; sample += (globalSampleRate*1))
    {
        if (big == 10)
        {
//            DBG("big");
            g.drawLine(250+waveBorder +1 + (sample / ratio), 10, 250+waveBorder +1 + (sample / ratio), 30, 3);
            big = 0;
        }
        else
        {
//            DBG("smal");
            g.drawLine(250+waveBorder +1 + (sample / ratio), 15, 250+waveBorder +1 + (sample / ratio), 30, 2);
        }
        big ++;
        
    }
    
    // Dropshadow on Ruler
    juce::DropShadow dropShadow(GlobalColors::dropShadow, 10, juce::Point<int>(0, 5));
    dropShadow.drawForRectangle(g, juce::Rectangle<int>(-10, -7, getWidth()+20, 0));

}

void PlayHeadRulerComponent::mouseDown(const juce::MouseEvent& event)
{
//    DBG("PlayHeadRulerComponent - MouseDown");
    findParentComponentOfClass<MainComponent>()->setPlayHeadPos(event.getMouseDownX()-layerControlW);
}

void PlayHeadRulerComponent::showSetTimeLineSizeAlertWindow()
{
    int w = findParentComponentOfClass<MainComponent>()->getWidth();
    int h = findParentComponentOfClass<MainComponent>()->getHeight();
    
    findParentComponentOfClass<MainComponent>()->SetTimeLineSizeAlertWindow.setBounds(w/2-150, h/2-150, 300, 300);
    findParentComponentOfClass<MainComponent>()->addAndMakeVisible(&findParentComponentOfClass<MainComponent>()->SetTimeLineSizeAlertWindow);
}

void PlayHeadRulerComponent::resized()
{
}
