/*
  ==============================================================================

    AlertWindowComponent.cpp
    Created: 14 Mar 2024 9:13:36pm
    Author:  Klaas Klee

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AlertWindowComponent.h"
#include "MainComponent.h"

//==============================================================================
AlertWindowComponent::AlertWindowComponent() : b1(b1Txt), b2(b2Txt)
{
    b1.onClick = [this] { b1Clicked(); };
    b1.setBounds(40, 200, 100, 50);
    addAndMakeVisible(b1);
    b2.onClick = [this] { b2Clicked(); };
    b2.setBounds(160, 200, 100, 50);
    addAndMakeVisible(b2);
    
    textEditor.setBounds(50, 120, 200, 30);
    addAndMakeVisible(textEditor);
}

AlertWindowComponent::~AlertWindowComponent()
{
}

void AlertWindowComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::white);   // clear the background

    g.setColour (juce::Colours::black);
    g.drawRect (getLocalBounds(), 10);   // draw an outline around the component

    g.setColour (juce::Colours::black);
    g.setFont (35.0f);
    g.drawText ("Export", 0, 30, getWidth(), 30, juce::Justification::centred, true);   // draw some placeholder text
    g.setFont (18.0f);
    g.drawText ("filename:", 0, 70, getWidth(), 20, juce::Justification::centred, true);   // draw some placeholder text
    
}

void AlertWindowComponent::resized()
{
}

// OK Button clicked
void AlertWindowComponent::b1Clicked()
{
    juce::String string = textEditor.getText();
    DBG("fileName: "+string);
    findParentComponentOfClass<MainComponent>()->exportAudioToFile(findParentComponentOfClass<MainComponent>()->exportBuffer, string);
    findParentComponentOfClass<MainComponent>()->killAlertWindow();
}
// CancelButton clicked
void AlertWindowComponent::b2Clicked()
{
    DBG("export canceled");
    findParentComponentOfClass<MainComponent>()->killAlertWindow();
}
