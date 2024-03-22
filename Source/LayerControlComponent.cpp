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
#include "LayerComponent.h"

//==============================================================================
LayerControlComponent::LayerControlComponent()
{
    
    //Gain Knob
    sGain.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    sGain.setRange(0.0, 1.0);
    sGain.setValue(0.5);
    sGain.setDoubleClickReturnValue(true, 0.5);
    sGain.setNumDecimalPlacesToDisplay(2);
    sGain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 20);
    sGain.setTextValueSuffix(" dB gain");
    sGain.addListener(this);
    sGain.setLookAndFeel(&LookAndFeel001);
    sGain.setBounds(30, 55, 100, 60);
    addAndMakeVisible(sGain);
    
    //Pan Knob
    sPan.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    sPan.setRange(0.0, 1.0);
    sPan.setValue(0.5);
    sPan.setDoubleClickReturnValue(true, 0.5);
    sPan.setNumDecimalPlacesToDisplay(2);
    sPan.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 20);
    sPan.setTextValueSuffix(" pan");
    sPan.addListener(this);
    sPan.setLookAndFeel(&LookAndFeel001);
    sPan.setBounds(120, 55, 100, 60);
    addAndMakeVisible(sPan);
    
    // change the last BlendMode here
    for (int i = 0; i < static_cast<int>(BlendModes::Binary)+1; i++)
    {
        BlendModeDropdown.addItem(getBlendModeName(static_cast<BlendModes>(i)), i+1);
    }
    BlendModeDropdown.addListener(this);

    BlendModeDropdown.setJustificationType(juce::Justification::centred);
    BlendModeDropdown.setBounds(55, 130, 140, 30);
    
    addAndMakeVisible(BlendModeDropdown);

    BlendModeDropdown.setSelectedId(1);
    
    // muteButton
    bMute.setButtonText("Mute");
    bMute.setLookAndFeel(&LookAndFeel001);
    bMute.onClick = [this] { bMuteClicked(); };
    bMute.setEnabled(true);
    bMute.setBounds(50, 15, 70, 30);
    addAndMakeVisible(bMute);
}

LayerControlComponent::~LayerControlComponent()
{
}

void LayerControlComponent::paint (juce::Graphics& g)
{
//    g.fillAll(juce::Colours::darkgrey);
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 15);   // draw an outline around the component
}

void LayerControlComponent::resized()
{
    
}

void LayerControlComponent::bMuteClicked()
{
    layerMute = bMute.getToggleState();
    std::cout << "layerMute: " << bMute.getToggleState() << std::endl;
}
