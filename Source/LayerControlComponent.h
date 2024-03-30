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
#include "LayerBlendmodeControlComponent.h"

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
            layerBlendmodeControlComponentPointer->updateLayerBlendModeControl(selectedBlendMode);
        }
    }
    
    void paint (juce::Graphics&) override;
    void resized() override;

    // Knobs
    juce::Slider sGain;
    juce::Slider sPan;

    juce::ComboBox BlendModeDropdown;

    // MUTE and "Delete"
    bool layerActive = false;
    
    // for "delete"
    void deactivateLayer();
    
    // pointer of LayerBlendmodeControlComponent of current Layer
    LayerBlendmodeControlComponent* layerBlendmodeControlComponentPointer;
    
    juce::TextButton bMute;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LayerControlComponent)
    
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
            case BlendModes::VariableFilter:
                return "FILTER";
            case BlendModes::CrossSynth:
                return "CROSSSYNTHESIS";
            case BlendModes::FreqFill:
                return "FREQUENCY FILL";
            case BlendModes::PitchShift:
                return "PITCH SHIFT";
            case BlendModes::Binary:
                return "BINARY XOR";
                
            default:
                return "Unknown";
        }
    };
    
    // BG
    juce::Image imgBG;
};
