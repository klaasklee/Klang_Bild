/*
  ==============================================================================

    LogoComponent.h
    Created: 24 Mar 2024 11:58:57am
    Author:  Klaas Klee

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LogoComponent  : public juce::Component
{
public:
    LogoComponent();
    ~LogoComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LogoComponent)
    
    std::unique_ptr<juce::Drawable> svgLogo;
};
