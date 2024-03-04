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
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(LayerViewport);
}

LayerViewPortComponent::~LayerViewPortComponent()
{
}

void LayerViewPortComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    LayersContainer.setBounds(0, 0, getLocalBounds().getWidth(), LayersContainer.getLayerContainerHeight());

    LayerViewport.setBounds(getLocalBounds());
    LayerViewport.setViewedComponent(&LayersContainer, false);
    
    LayerViewport.setScrollBarsShown(false, false, true, true);
}
