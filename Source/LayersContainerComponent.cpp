/*
  ==============================================================================

    LayersContainerComponent.cpp
    Created: 9 Feb 2024 10:38:35am
    Author:  Klaas Klee

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LayersContainerComponent.h"
#include "Globals.h"
#include "LayerViewPortComponent.h"

//==============================================================================
LayersContainerComponent::LayersContainerComponent()
{   
    int index = numOfLayers-1;
    for (int i = 0; i < numOfLayers; i++) {
        addAndMakeVisible(Layers[i]);
        Layers[i].layerIndex = index;
        index--;
    }
    setBaseLayerControls();
}

LayersContainerComponent::~LayersContainerComponent()
{
}

void LayersContainerComponent::resized()
{
    resortGuiLayers();
}

void LayersContainerComponent::paint (juce::Graphics& g)
{
    
}

void LayersContainerComponent::resortGuiLayers()
{
    juce::FlexBox layersFB;
    layersFB.flexDirection = juce::FlexBox::Direction::row;
    layersFB.alignContent = juce::FlexBox::AlignContent::flexStart;
    layersFB.flexWrap = juce::FlexBox::Wrap::wrap;
    
    for (int i = numOfLayers-1; i >= 0; i--) {
        layersFB.items.add(juce::FlexItem(Layers[getLayerByIndex(i)]).withHeight(200).withWidth(getLocalBounds().getWidth()));
    }

    layersFB.performLayout(getLocalBounds());
}

int LayersContainerComponent::getLayerByIndex(int index)
{
    for (int i = 0; i < numOfLayers; i++) {
        if (Layers[i].layerIndex == index)
        {
            return i;
        }
    }
    return 0;
}

void LayersContainerComponent::changeLayerIndex(bool direction, int index)
{
    // direction = true - move up
    // direction = false - move down
    
    DBG(index);
    
    if (direction)
    {
        DBG("up");
        if (index != numOfLayers-1)
        {
            // switch index of layer to placeHolder
            int placeHolder = numOfLayers + 1;
            Layers[getLayerByIndex(index)].layerIndex = placeHolder;
            // switch index of layer above -1
            Layers[getLayerByIndex(index+1)].layerIndex = index;
            // switch index of layer + 1
            Layers[getLayerByIndex(placeHolder)].layerIndex = index + 1;
            
            Layers[getLayerByIndex(index)].LayerControl.BlendModeDropdown.setVisible(true);
            Layers[getLayerByIndex(index)].bToggleShowBlendPara.setVisible(true);
            Layers[getLayerByIndex(index+1)].LayerControl.BlendModeDropdown.setVisible(true);
            Layers[getLayerByIndex(index+1)].bToggleShowBlendPara.setVisible(true);
            setBaseLayerControls();
        }
        else
        {
            DBG("cant move layer up");
        }
    }
    else if (!direction)
    {
        DBG("down");
        if (index != 0)
        {
            // switch index of layer to placeHolder
            int placeHolder = numOfLayers + 1;
            Layers[getLayerByIndex(index)].layerIndex = placeHolder;
            // switch index of layer below +1
            Layers[getLayerByIndex(index-1)].layerIndex = index;
            // switch index of layer - 1
            Layers[getLayerByIndex(placeHolder)].layerIndex = index - 1;
            
            Layers[getLayerByIndex(index)].LayerControl.BlendModeDropdown.setVisible(true);
            Layers[getLayerByIndex(index)].bToggleShowBlendPara.setVisible(true);
            Layers[getLayerByIndex(index-1)].LayerControl.BlendModeDropdown.setVisible(true);
            Layers[getLayerByIndex(index-1)].bToggleShowBlendPara.setVisible(true);
            setBaseLayerControls();
        }
        else
        {
            DBG("cant move layer down");
        }
    }
    resortGuiLayers();
}

void LayersContainerComponent::setBaseLayerControls()
{
    // hides baseLayer Controls
    Layers[getLayerByIndex(0)].LayerControl.BlendModeDropdown.setVisible(false);
    Layers[getLayerByIndex(0)].bToggleShowBlendPara.setVisible(false);
    Layers[getLayerByIndex(0)].bToggleShowBlendPara.setButtonText("show parameters");
    Layers[getLayerByIndex(0)].LayerBlendmodeControl.setVisible(false);
    // sets baseLayer to BlendMode Normal
    Layers[getLayerByIndex(0)].LayerControl.BlendModeDropdown.setSelectedId(1, juce::NotificationType::dontSendNotification);
    Layers[getLayerByIndex(0)].LayerControl.selectedBlendMode = Normal;
    Layers[getLayerByIndex(0)].LayerControl.layerBlendmodeControlComponentPointer->updateLayerBlendModeControl(Normal);
}
