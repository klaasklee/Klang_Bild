/*
  ==============================================================================

    LayerViewPortComponent.cpp
    Created: 9 Feb 2024 10:38:18am
    Author:  Klaas Klee

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LayerViewPortComponent.h"

//==============================================================================
LayerViewPortComponent::LayerViewPortComponent()
{
    // BG
    imgBG = juce::ImageCache::getFromMemory(BinaryData::bgLayersContainer_2_jpg, BinaryData::bgLayersContainer_2_jpgSize);

    addAndMakeVisible(LayerViewport);
}

LayerViewPortComponent::~LayerViewPortComponent()
{
}

void LayerViewPortComponent::paint(juce::Graphics& g)
{
    g.fillAll(GlobalColors::bG);
    // Draw BG
    g.drawImage(imgBG, getLocalBounds().toFloat(), juce::RectanglePlacement::fillDestination, false);
}

void LayerViewPortComponent::resized()
{
    LayersContainer.setBounds(0, 0, getLocalBounds().getWidth(), LayersContainer.getLayerContainerHeight());

    LayerViewport.setBounds(getLocalBounds());
    LayerViewport.setViewedComponent(&LayersContainer, false);
    
    LayerViewport.setScrollBarsShown(false, false, true, true);
}
