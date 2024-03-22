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
    float channelGain[globalNumChannels];

    BlendModes selectedBlendMode;
    
    void sliderValueChanged (juce::Slider* slider) override
    {
        if (slider == &sGain)
        {
            gain = 0.99 * sGain.getValue() + 0.005; // avoid zero gain to be able to restore applyGain()
            std::cout << "gain: " << gain << std::endl;
        }
        else if (slider == &sPan)
        {
            pan = 0.005 + 0.99*sPan.getValue(); // avoid extrene pan to be able to restore applyGain()
            std::cout << "pan: " << pan << std::endl;
        }

        for (int ch = 0; ch < globalNumChannels; ch++) {
            channelGain[ch] = gain * 2 * (1 - ((std::pow(-1, ch)) * 2 * (pan - 0.5)));
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

    // Knobs
    juce::Slider sGain;
    juce::Slider sPan;
    
    // MUTE
    bool layerMute = false;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LayerControlComponent)
    
    juce::ComboBox BlendModeDropdown;
    
    juce::ToggleButton bMute;
    void bMuteClicked();
    
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
            case BlendModes::Binary:
                return "BINARY XOR";
                
            default:
                return "Unknown";
        }
    };
};
