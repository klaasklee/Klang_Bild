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
class LayerControlComponent  : public juce::Component,
                               public juce::ComboBox::Listener
{
public:
    LayerControlComponent();
    ~LayerControlComponent() override;

    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override
    {
        if (comboBoxThatHasChanged == &BlendModeDropdown)
        {
            BlendModes selectedOption = static_cast<BlendModes>(BlendModeDropdown.getSelectedId() - 1);
        }
    }
    
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LayerControlComponent)
    
    juce::ComboBox BlendModeDropdown;

    juce::String getBlendModeName(BlendModes value)
    {
        switch (value) {
            case BlendModes::Normal:
                return "NORMAL";
            case BlendModes::Subdivide:
                return "SUBDIVIDE";
            case BlendModes::Blend:
                return "BLEND";
            case BlendModes::Multiply:
                return "MULTIPLY";
            case BlendModes::Duck:
                return "DUCK";
                
            default:
                return "Unknown";
        }
    };
};
