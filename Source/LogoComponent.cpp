/*
  ==============================================================================

    LogoComponent.cpp
    Created: 24 Mar 2024 11:58:57am
    Author:  Klaas Klee

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LogoComponent.h"

//==============================================================================
LogoComponent::LogoComponent()
{
    auto svgData = juce::MemoryInputStream(BinaryData::logo_2_svg, BinaryData::logo_2_svgSize, false);
    svgLogo = juce::Drawable::createFromImageDataStream(svgData);
    
//    if (svgLogo != nullptr)
//    {
//        svgLogo->setBounds(getLocalBounds());
//        addAndMakeVisible(svgLogo.get());
//    }
}

LogoComponent::~LogoComponent()
{
}

void LogoComponent::paint (juce::Graphics& g)
{
//    g.fillAll(juce::Colours::white);
    
    int vertMargin = 5;
    
    // Calculate the scale factor for resizing
    float scaleX = (float) getWidth() / (float) svgLogo->getWidth();
    float scaleY = (float) (getHeight()-vertMargin) / (float) svgLogo->getHeight();
    float scale = std::min(scaleX, scaleY);

    // Calculate the centered position after scaling
    int scaledWidth = (int) (svgLogo->getWidth() * scale);
    int scaledHeight = (int) (svgLogo->getHeight() * scale);
    int x = (getWidth() - scaledWidth) / 2;
    int y = ((getHeight()-vertMargin) - scaledHeight) / 2;
    
    svgLogo->drawWithin(g, juce::Rectangle<float>(x, y+vertMargin, (float) scaledWidth, (float) scaledHeight), juce::RectanglePlacement::centred, 1.0f);
}

void LogoComponent::resized()
{
}
