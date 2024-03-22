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
AlertWindowComponent::AlertWindowComponent(juce::String b1Txt, juce::String b2Txt, juce::String headerTxt, juce::String infoTxt, AlertWindows alertWindowType) : b1(b1Txt), b2(b2Txt)
{
    headerText = headerTxt;
    infoText = infoTxt;
    windowType = alertWindowType;
    
    b1.onClick = [this] { b1Clicked(); };
    b1.setBounds(40, 200, 100, 50);
    addAndMakeVisible(b1);
    b2.onClick = [this] { b2Clicked(); };
    b2.setBounds(160, 200, 100, 50);
    addAndMakeVisible(b2);
    
    textEditor.setBounds(50, 120, 200, 30);
    if (windowType == SetTimeLineSizeWindow)
    {
        textEditor.setInputRestrictions(3, "0123456789");
    }
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
    g.drawText (headerText, 0, 30, getWidth(), 30, juce::Justification::centred, true);
    g.setFont (18.0f);
    g.drawText (infoText, 0, 70, getWidth(), 20, juce::Justification::centred, true);
    
}

void AlertWindowComponent::resized()
{
}

// OK Button clicked
void AlertWindowComponent::b1Clicked()
{
    if (windowType == ExportWindow)
    {
        juce::String string = textEditor.getText();
        DBG(infoText+string);
        findParentComponentOfClass<MainComponent>()->exportAudioToFile(findParentComponentOfClass<MainComponent>()->exportBuffer, string);
        findParentComponentOfClass<MainComponent>()->killExportAlertWindow();
    }
    else if (windowType == SetTimeLineSizeWindow)
    {
        juce::String string = textEditor.getText();
        DBG(infoText+string);
        int timeLineSize = string.getIntValue();
        if (timeLineSize > 0)
        {
            findParentComponentOfClass<MainComponent>()->setTimeLineSize(timeLineSize);
        }
        else
        {
            DBG("timeLineSize to smal");
        }
        findParentComponentOfClass<MainComponent>()->killSetTimeLineSizeAlertWindow();
    }
    
}
// CancelButton clicked
void AlertWindowComponent::b2Clicked()
{
    DBG("AlertWindow canceled");
    if (windowType == ExportWindow)
    {
        findParentComponentOfClass<MainComponent>()->killExportAlertWindow();
    }
    else if (windowType == SetTimeLineSizeWindow)
    {
        findParentComponentOfClass<MainComponent>()->killSetTimeLineSizeAlertWindow();
    }
}
