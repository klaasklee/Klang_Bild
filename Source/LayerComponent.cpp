/*
  ==============================================================================

    LayerComponent.cpp
    Created: 9 Feb 2024 10:39:21am
    Author:  Klaas Klee

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LayerComponent.h"
#include "Globals.h"

//==============================================================================
LayerComponent::LayerComponent() : layerUp("Up", juce::Colours::black, juce::Colours::grey, juce::Colours::lightgrey), layerDown("Down", juce::Colours::black, juce::Colours::grey, juce::Colours::lightgrey)
{
    addAndMakeVisible(LayerControl);
    addAndMakeVisible(LayerWave);
    
    // move Layer Up/Down Buttons
    juce::BorderSize<int> border;
    border.setTop(0);
    border.setBottom(0);
    border.setLeft(7);
    border.setRight(0);
    
    // upArrow Button
    juce::Path upArrowPath;
    juce::Line<float> upLine1;
    upLine1.setStart(0, 20);
    upLine1.setEnd(10.5, 4.5);
    juce::Line<float> upLine2;
    upLine2.setStart(9.5, 4.5);
    upLine2.setEnd(20, 20);
    upArrowPath.addLineSegment(upLine1, 3);
    upArrowPath.addLineSegment(upLine2, 3);
    layerUp.setBounds(waveBorder, waveBorder + 60, 30, 15);
    layerUp.setShape(upArrowPath, false, false, false);
    layerUp.setBorderSize(border);
    layerUp.onClick = [this] { moveLayerUp(); };
    addAndMakeVisible(layerUp);
    
    // downArrow Button
    juce::Path downArrowPath;
    juce::Line<float> downLine1;
    downLine1.setStart(0, 0);
    downLine1.setEnd(10.5, 15.5);
    juce::Line<float> downLine2;
    downLine2.setStart(9.5, 15.5);
    downLine2.setEnd(20, 0);
    downArrowPath.addLineSegment(downLine1, 3);
    downArrowPath.addLineSegment(downLine2, 3);
    layerDown.setBounds(waveBorder, waveBorder + 100, 30, 15);
    layerDown.setShape(downArrowPath, false, false, false);
    layerDown.setBorderSize(border);
    layerDown.onClick = [this] { moveLayerDown(); };
    addAndMakeVisible(layerDown);
}

LayerComponent::~LayerComponent()
{
}

void LayerComponent::paint (juce::Graphics& g)
{
    // main (middle) area
    g.fillAll(juce::Colours::darkgrey);
    
    // left area
    g.setColour(juce::Colours::lightgrey);
    g.fillRect(waveBorder, waveBorder, 36, getHeight()-waveBorder*2);
    
    // paint LayerIndex Number
    g.setColour(juce::Colours::black);
    g.setFont(25);
    g.drawText(juce::String(layerIndex), waveBorder, waveBorder+5, 36, waveBorder+30, juce::Justification::centred);
}
void LayerComponent::resized()
{
    juce::FlexBox layerFB;
    layerFB.flexDirection = juce::FlexBox::Direction::row;
    layerFB.flexWrap = juce::FlexBox::Wrap::wrap;
    
        layerFB.items.add(juce::FlexItem(LayerControl).withWidth(layerControlW));
        layerFB.items.add(juce::FlexItem(LayerWave).withFlex(1));
   
    layerFB.performLayout(getLocalBounds());
}

void LayerComponent::moveLayerUp()
{
    DBG("moveLayerUp");
}
void LayerComponent::moveLayerDown()
{
    DBG("moveLayerDown");
}
