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
LayerComponent::LayerComponent() : layerUp("Up", juce::Colours::darkgrey, juce::Colours::white, juce::Colours::darkred), layerDown("Down", juce::Colours::darkgrey, juce::Colours::white, juce::Colours::darkred)
{
    addAndMakeVisible(LayerControl);
    addAndMakeVisible(LayerWave);
    
    // move Layer Up/Down Buttons
    juce::BorderSize<int> border;
    border.setTop(3);
    border.setBottom(0);
    border.setLeft(0);
    border.setRight(0);
    
    // upArrow Button
    juce::Path upArrowPath;
    juce::Line<float> upLine1;
    upLine1.setStart(0, 20);
    upLine1.setEnd(10, 5);
    juce::Line<float> upLine2;
    upLine2.setStart(10, 5);
    upLine2.setEnd(20, 20);
    upArrowPath.addLineSegment(upLine1, 3);
    upArrowPath.addLineSegment(upLine2, 3);
    layerUp.setBounds(150, 20, 30, 15);
    layerUp.setShape(upArrowPath, false, false, false);
    layerUp.setBorderSize(border);
    layerUp.onClick = [this] { moveLayerUp(); };
    addAndMakeVisible(layerUp);
    
    // downArrow Button
    juce::Path downArrowPath;
    juce::Line<float> downLine1;
    downLine1.setStart(0, 0);
    downLine1.setEnd(10, 15);
    juce::Line<float> downLine2;
    downLine2.setStart(10, 15);
    downLine2.setEnd(20, 0);
    downArrowPath.addLineSegment(downLine1, 3);
    downArrowPath.addLineSegment(downLine2, 3);
    layerDown.setBounds(190, 20, 30, 15);
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
//    g.fillAll(juce::Colours::darkgrey);
    g.setColour(juce::Colours::white);
    
    // paint LayerIndex Number
    g.setFont(20);
    g.drawText(juce::String(layerIndex), 20, 20, 20, 20, juce::Justification::centred);
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
