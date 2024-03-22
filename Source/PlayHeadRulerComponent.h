/*
  ==============================================================================

    PlayHeadRulerComponent.h
    Created: 19 Mar 2024 8:51:20pm
    Author:  Klaas Klee

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "AlertWindowComponent.h"

//==============================================================================
/*
*/
class PlayHeadRulerComponent  : public juce::Component
{
public:
    PlayHeadRulerComponent();
    ~PlayHeadRulerComponent() override;

    void mouseDown(const juce::MouseEvent& event) override;
    
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayHeadRulerComponent)

    juce::TextButton bSetTimeLineSize;
    

    void showSetTimeLineSizeAlertWindow();
};
