/*
  ==============================================================================

    Globals.h
    Created: 9 Feb 2024 10:42:05am
    Author:  Klaas Klee

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

const int numOfLayers = 5;
const int layerHeight = 200;

enum BlendModes
{
    Normal,
    Subdivide,
    Blend,
    Multiply,
    Duck
};
//BlendModes mode;

//==============================================================================
//Styling

namespace GlobalColors {

    const juce::Colour black = juce::Colour(0xff000000);
    const juce::Colour white = juce::Colour(0xffffffff);

    const juce::Colour brightText = juce::Colour(0xffffffff);
    const juce::Colour darkText = juce::Colour(0xff000000);

    const juce::Colour BG = juce::Colour(0xffCFCFCF);

    const juce::Colour controlBarBG = juce::Colour(0xff9E9E9E);

    const juce::Colour toolButtonActive = juce::Colour(0xffbd3a31);
    const juce::Colour toolButtonNotActive = juce::Colour(0xff000000);

    const juce::Colour controlBarPlayButtonActive = juce::Colour(0xff47FF37);
    //const juce::Colour controlBarPlayButtonNotActive = juce::Colour(0xff47FF37);
    //const juce::Colour controlBarPauseButtonActive = juce::Colour(0xffA16C2E);
    const juce::Colour controlBarPauseButtonNotActive = juce::Colour(0xffA16C2E);
    //const juce::Colour controlBarStopButtonActive = juce::Colour(0xff824242);
    const juce::Colour controlBarStopButtonNotActive = juce::Colour(0xff824242);

    const juce::Colour controlBarTimeStampBG = juce::Colour(0xff8D8D8D);

}

namespace GlobalFonts {
const juce::Font titleFont = juce::Font("Eastman Roman Alt", "Regular", 40.0f);
const juce::Font mainFont = juce::Font("Eastman Roman Alt", "Regular", 20.0f);
}
