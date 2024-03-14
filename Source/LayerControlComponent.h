/*
  ==============================================================================

    LayerControlComponent.h
    Created: 9 Feb 2024 10:39:46am
    Author:  Klaas Klee

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Globals.h"

//==============================================================================
/*
*/
class LayerControlComponent  :  public juce::Component,
                                public juce::Slider::Listener,
                                public juce::ComboBox::Listener
{
public:
    LayerControlComponent();
    ~LayerControlComponent() override;
    
    //public variables of Layer Control
    float gain;
    float pan;
    BlendModes selectedBlendMode;
    
    void sliderValueChanged (juce::Slider* slider) override
    {
        if (slider == &sGain)
        {
            gain = sGain.getValue() + 0.01; // avoid zero gain to be able to restore applyGain()
            std::cout << "gain: " << gain << std::endl;
        }
        else if (slider == &sPan)
        {
            pan = 0.005 + 0.99*sPan.getValue(); // avoid extrene pan to be able to restore applyGain()
            std::cout << "pan: " << pan << std::endl;
        }
    }

    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override
    {
        if (comboBoxThatHasChanged == &BlendModeDropdown)
        {
            selectedBlendMode = static_cast<BlendModes>(BlendModeDropdown.getSelectedId() - 1);
        }
    }
    
    void paint (juce::Graphics&) override;
    void resized() override;

    //Knobs
    juce::Slider sGain;
    juce::Slider sPan;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LayerControlComponent)
    
    juce::ComboBox BlendModeDropdown;
    
    //styling - lookAndFeel
    LookAndFeel001 LookAndFeel001;

    juce::String getBlendModeName(BlendModes value)
    {
        switch (value) {
            case BlendModes::Normal:
                return "NORMAL";
            case BlendModes::Multiply:
                return "MULTIPLY";
            case BlendModes::Duck:
                return "DUCK";
                
            default:
                return "Unknown";
        }
    };
};
