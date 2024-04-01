/*
  ==============================================================================

    LayerBlendmodeControlComponent.cpp
    Created: 22 Mar 2024 2:34:55pm
    Author:  Klaas Klee

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LayerBlendmodeControlComponent.h"
#include "Globals.h"

//==============================================================================
LayerBlendmodeControlComponent::LayerBlendmodeControlComponent()
{
    
    resetLayerBlendModeControl();

}

LayerBlendmodeControlComponent::~LayerBlendmodeControlComponent()
{
}

void LayerBlendmodeControlComponent::paint (juce::Graphics& g)
{
    g.fillAll (GlobalColors::layerControlBg);
}

void LayerBlendmodeControlComponent::resized()
{
}

// change Parameters and Layout for BlendModes here
void LayerBlendmodeControlComponent::updateLayerBlendModeControl(BlendModes newBlendmode)
{
    switch (newBlendmode) {
        case Normal:
            resetLayerBlendModeControl();
            bPara1.setVisible(false);
            bPara2.setVisible(false);
            lPara1.setVisible(false);
            sPara1.setVisible(false);
            lPara2.setVisible(false);
            sPara2.setVisible(false);
            break;
        case Multiply:
            resetLayerBlendModeControl();
            bPara1.setVisible(false);
            bPara2.setVisible(false);
            lPara1.setVisible(false);
            sPara1.setVisible(false);
            lPara2.setVisible(false);
            sPara2.setVisible(false);
            break;
        case Duck:
            resetLayerBlendModeControl();
            bPara1.setVisible(false);
            bPara2.setVisible(false);
            lPara1.setVisible(false);
            sPara1.setVisible(false);
            lPara2.setVisible(false);
            sPara2.setVisible(false);
            break;
        case CrossSynth:
            resetLayerBlendModeControl();
            bPara1.setVisible(false);
            bPara2.setVisible(false);
            lPara1.setVisible(true);
            sPara1.setVisible(true);
            lPara1.setText("smoothing order", juce::NotificationType::dontSendNotification);
            sPara1.setRange(1, 10);
            sPara1.setDoubleClickReturnValue(true, 2);
            sPara1.setValue(2);
            lPara2.setVisible(true);
            sPara2.setVisible(true);
            lPara2.setText("treble boost", juce::NotificationType::dontSendNotification);
            sPara2.setRange(0, 20);
            sPara2.setDoubleClickReturnValue(true, 0);
            sPara2.setValue(0);
            break;
        case FreqFill:
            resetLayerBlendModeControl();
            bPara1.setVisible(false);
            bPara2.setVisible(false);
            lPara1.setVisible(true);
            sPara1.setVisible(true);
            lPara1.setText("smoothing order", juce::NotificationType::dontSendNotification);
            sPara1.setRange(1, 10);
            sPara1.setDoubleClickReturnValue(true, 2);
            sPara1.setValue(2);
            lPara2.setVisible(true);
            sPara2.setVisible(true);
            lPara2.setText("treble boost", juce::NotificationType::dontSendNotification);
            sPara2.setRange(0, 20);
            sPara2.setDoubleClickReturnValue(true, 0);
            sPara2.setValue(0);
            break;
        case PitchShift:
            resetLayerBlendModeControl();
            bPara1.setVisible(false);
            bPara2.setVisible(false);
            lPara1.setVisible(true);
            sPara1.setVisible(true);
            lPara1.setText("shift intensity", juce::NotificationType::dontSendNotification);
            sPara1.setRange(-20, 20);
            sPara1.setDoubleClickReturnValue(true, 2);
            sPara1.setValue(2);
            sPara1.setNumDecimalPlacesToDisplay(0);
            lPara2.setVisible(false);
            sPara2.setVisible(false);
            break;
        case VariableFilter:
            resetLayerBlendModeControl();
            bPara1.setVisible(true);
            bPara2.setVisible(true);
            bPara1.setButtonText("LPF");
            bPara2.setButtonText("HPF");
            bPara1.setToggleState(true, juce::NotificationType::dontSendNotification);
            bPara2.setToggleState(false, juce::NotificationType::dontSendNotification);
            lPara1.setVisible(true);
            sPara1.setVisible(true);
            lPara2.setVisible(true);
            sPara2.setVisible(true);
            lPara1.setText("Freq Range", juce::NotificationType::dontSendNotification);
            lPara2.setText("Resonance", juce::NotificationType::dontSendNotification);
            sPara1.setRange(0, 15000);
            sPara2.setRange(0.1, 10);
            sPara1.setValue(5000);
            sPara1.setDoubleClickReturnValue(true, 5000);
            sPara2.setValue(1);
            sPara2.setDoubleClickReturnValue(true, 1);
            break;
        case Binary:
            resetLayerBlendModeControl();
            bPara1.setVisible(true);
            bPara2.setVisible(true);
            bPara1.setButtonText("AND");
            bPara2.setButtonText("OR");
            bPara1.setToggleState(false, juce::NotificationType::dontSendNotification);
            bPara2.setToggleState(false, juce::NotificationType::dontSendNotification);
            lPara1.setVisible(false);
            sPara1.setVisible(false);
            lPara2.setVisible(false);
            sPara2.setVisible(false);
            break;
        default:
            break;
    }
}

void LayerBlendmodeControlComponent::resetLayerBlendModeControl()
{
    // bool Parameter 1 as toggleButton
    bPara1.setButtonText("toggle 1");
    bPara1.onClick = [this] { bPara1Clicked(); };
    bPara1.setToggleState(false, juce::NotificationType::dontSendNotification);
    bPara1.setBounds(10, 8, layerControlW-36-waveBorder*2-20, 20);
    bPara1.setColour(juce::ToggleButton::textColourId, GlobalColors::black);
    bPara1.setLookAndFeel(&LookAndFeel001);
    addAndMakeVisible(bPara1);
    
    // bool Parameter 2 as toggleButton
    bPara2.setButtonText("toggle 2");
    bPara2.onClick = [this] { bPara2Clicked(); };
    bPara2.setToggleState(false, juce::NotificationType::dontSendNotification);
    bPara2.setBounds(10, 33, layerControlW-36-waveBorder*2-20, 20);
    bPara2.setColour(juce::ToggleButton::textColourId, GlobalColors::black);
    bPara2.setLookAndFeel(&LookAndFeel001);
    addAndMakeVisible(bPara2);
    
    // label for Parameter 1
    lPara1.setText("float parameter 1", juce::NotificationType::dontSendNotification);
    lPara1.setBounds(10, 55, layerControlW-36-waveBorder*2-20, 20);
    lPara1.setColour(juce::Label::textColourId, GlobalColors::black);
    addAndMakeVisible(lPara1);
    // float Parameter 1 as slider
    sPara1.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    sPara1.setRange(0.0, 1.0);
    sPara1.setValue(0.001);
    sPara1.setDoubleClickReturnValue(true, 0.001);
    sPara1.setNumDecimalPlacesToDisplay(2);
    sPara1.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 15);
    sPara1.setColour(juce::Slider::textBoxTextColourId, GlobalColors::white);
    sPara1.setColour(juce::Slider::textBoxBackgroundColourId, GlobalColors::black);
    sPara1.setColour(juce::Slider::textBoxOutlineColourId, GlobalColors::black);
    sPara1.setLookAndFeel(&LookAndFeel001);
    sPara1.addListener(this);
    sPara1.setBounds(10, 65, layerControlW-36-waveBorder*2-20, 30);
    addAndMakeVisible(sPara1);
    
    // label for Parameter 2
    lPara2.setText("float parameter 2", juce::NotificationType::dontSendNotification);
    lPara2.setBounds(10, 90, layerControlW-36-waveBorder*2-20, 20);
    lPara2.setColour(juce::Label::textColourId, GlobalColors::black);
    addAndMakeVisible(lPara2);
    // float Parameter 2 as slider
    sPara2.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    sPara2.setRange(0.0, 1.0);
    sPara2.setValue(0.001);
    sPara2.setDoubleClickReturnValue(true, 0.001);
    sPara2.setNumDecimalPlacesToDisplay(2);
    sPara2.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 15);
    sPara2.setColour(juce::Slider::textBoxTextColourId, GlobalColors::white);
    sPara2.setColour(juce::Slider::textBoxBackgroundColourId, GlobalColors::black);
    sPara2.setColour(juce::Slider::textBoxOutlineColourId, GlobalColors::black);
    sPara2.setLookAndFeel(&LookAndFeel001);
    sPara2.addListener(this);
    sPara2.setBounds(10, 100, layerControlW-36-waveBorder*2-20, 30);
    addAndMakeVisible(sPara2);
}


void LayerBlendmodeControlComponent::sliderValueChanged(juce::Slider* slider)
{
        if (slider == &sPara1)
        {
            fPara1 = sPara1.getValue();
        }
        else if (slider == &sPara2)
        {
            fPara2 = sPara2.getValue();
        }
}

void LayerBlendmodeControlComponent::bPara1Clicked()
{
    boPara1 = bPara1.getToggleState();
}
void LayerBlendmodeControlComponent::bPara2Clicked()
{
    boPara2 = bPara1.getToggleState();
}
