/*
  ==============================================================================

    LayerControlComponent.cpp
    Created: 9 Feb 2024 10:39:46am
    Author:  Klaas Klee

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LayerControlComponent.h"
#include "Globals.h"

//==============================================================================
LayerControlComponent::LayerControlComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    //Gain Knob
    sGain.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    sGain.setRange(0.0, 1.0);
    sGain.setValue(0.5);
    sGain.setNumDecimalPlacesToDisplay(2);
    sGain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 20);
    sGain.setTextValueSuffix(" dB gain");
    sGain.addListener(this);
    sGain.setLookAndFeel(&LookAndFeel001);
    sGain.setBounds(30, 47, 100, 60);
    addAndMakeVisible(sGain);
    
    //Pan Knob
    sPan.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    sPan.setRange(0.0, 1.0);
    sPan.setValue(0.5);
    sPan.setNumDecimalPlacesToDisplay(2);
    sPan.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 20);
    sPan.setTextValueSuffix(" pan");
    sPan.addListener(this);
    sPan.setLookAndFeel(&LookAndFeel001);
    sPan.setBounds(120, 47, 100, 60);
    addAndMakeVisible(sPan);
    
    for (int i = 0; i < static_cast<int>(BlendModes::Duck)+1; i++)
    {
        BlendModeDropdown.addItem(getBlendModeName(static_cast<BlendModes>(i)), i+1);
    }
    BlendModeDropdown.addListener(this);

    BlendModeDropdown.setJustificationType(juce::Justification::centred);
    BlendModeDropdown.setBounds(55, 130, 140, 30);
    addAndMakeVisible(BlendModeDropdown);

    BlendModeDropdown.setSelectedId(1);
}

LayerControlComponent::~LayerControlComponent()
{
}

void LayerControlComponent::paint (juce::Graphics& g)
{
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 15);   // draw an outline around the component
}

void LayerControlComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
