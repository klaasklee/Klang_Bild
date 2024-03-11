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
    addAndMakeVisible(LayerViewport);
}

LayerViewPortComponent::~LayerViewPortComponent()
{
}

void LayerViewPortComponent::resized()
{
    LayersContainer.setBounds(0, 0, getLocalBounds().getWidth(), LayersContainer.getLayerContainerHeight());

    LayerViewport.setBounds(getLocalBounds());
    LayerViewport.setViewedComponent(&LayersContainer, false);
    
    LayerViewport.setScrollBarsShown(false, false, true, true);
}
