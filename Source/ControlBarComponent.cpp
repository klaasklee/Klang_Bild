/*
  ==============================================================================

    ControlBarComponent.cpp
    Created: 9 Feb 2024 10:37:57am
    Author:  Klaas Klee

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ControlBarComponent.h"
#include "Globals.h"
#include "MainComponent.h"

//==============================================================================
ControlBarComponent::ControlBarComponent() : bPlay("play"), bPause("pause"), bStop("stop"), bLoop("loop"), recordButton("record")
{
    // BG
    imgBG = juce::ImageCache::getFromMemory(BinaryData::bgControlBar_2_jpg, BinaryData::bgControlBar_2_jpgSize);
//
    
    //Volume
    sVolumeDial.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    sVolumeDial.setRange(0.0, 1.0);
    sVolumeDial.setValue(volumeOnStartUp);
    sVolumeDial.setDoubleClickReturnValue(true, volumeOnStartUp);
    sVolumeDial.setNumDecimalPlacesToDisplay(2);
    sVolumeDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 20);
    sVolumeDial.setColour(juce::Slider::textBoxBackgroundColourId, GlobalColors::black);
//    sVolumeDial.setTextValueSuffix(" dB");
    sVolumeDial.addListener(this);
    sVolumeDial.setLookAndFeel(&LookAndFeel001);
    addAndMakeVisible(sVolumeDial);
    
    
    //Tools
    bEins.setButtonText("1");
    bEins.setLookAndFeel(&LookAndFeel001);
    bEins.onClick = [this] { ; }; // still needs to be initialized
    bEins.setEnabled(false); // no function therefore disabled
    addAndMakeVisible(bEins);
    
    bZwei.setButtonText("2");
    bZwei.setLookAndFeel(&LookAndFeel001);
    bZwei.onClick = [this] { ; }; // still needs to be initialized
    bZwei.setEnabled(false); // no function therefore disabled
    addAndMakeVisible(bZwei);
    
    bDrei.setButtonText("3");
    bDrei.setLookAndFeel(&LookAndFeel001);
    bDrei.onClick = [this] { ; }; // still needs to be initialized
    bDrei.setEnabled(false); // no function therefore disabled
    addAndMakeVisible(bDrei);
    
    bVier.setButtonText("4");
    bVier.setLookAndFeel(&LookAndFeel001);
    bVier.onClick = [this] { ; }; // still needs to be initialized
    bVier.setEnabled(false); // no function therefore disabled
    addAndMakeVisible(bVier);
    
    bFünf.setButtonText("5");
    bFünf.setLookAndFeel(&LookAndFeel001);
    bFünf.onClick = [this] { ; }; // still needs to be initialized
    bFünf.setEnabled(false); // no function therefore disabled
    addAndMakeVisible(bFünf);
    
    bSechs.setButtonText("6");
    bSechs.setLookAndFeel(&LookAndFeel001);
    bSechs.onClick = [this] { ; }; // still needs to be initialized
    bSechs.setEnabled(false); // no function therefore disabled
    addAndMakeVisible(bSechs);
    
    
    //MainControls
    addAndMakeVisible(Logo);
    
    bPlay.setColour(juce::TextButton::buttonColourId, GlobalColors::controlBarPlayButtonNotActive);
    bPlay.setColour(juce::TextButton::buttonOnColourId, GlobalColors::controlBarPlayButtonActive);
    bPlay.setLookAndFeel(&LookAndFeel001);
    bPlay.setColour(juce::TextButton::textColourOnId, GlobalColors::white);
    bPlay.setColour(juce::TextButton::textColourOffId, GlobalColors::white);
    bPlay.onClick = [this] { bPlayClicked(); };
    bPlay.setEnabled(true);
    addAndMakeVisible(bPlay);

    bPause.setColour(juce::TextButton::buttonColourId, GlobalColors::controlBarPauseButtonNotActive);
    bPause.setColour(juce::TextButton::buttonOnColourId, GlobalColors::controlBarPauseButtonActive);
    bPause.setLookAndFeel(&LookAndFeel001);
    bPause.setColour(juce::TextButton::textColourOnId, GlobalColors::white);
    bPause.setColour(juce::TextButton::textColourOffId, GlobalColors::white);
    bPause.onClick = [this] { bPauseClicked(); };
    bPause.setEnabled(true);
    addAndMakeVisible(bPause);

    bStop.setColour(juce::TextButton::buttonColourId, GlobalColors::controlBarStopButtonNotActive);
    bStop.setColour(juce::TextButton::buttonOnColourId, GlobalColors::controlBarStopButtonActive);
    bStop.setLookAndFeel(&LookAndFeel001);
    bStop.setColour(juce::TextButton::textColourOnId, GlobalColors::white);
    bStop.setColour(juce::TextButton::textColourOffId, GlobalColors::white);
    bStop.onClick = [this] { bStopClicked(); };
    bStop.setEnabled(true);
    bStop.setToggleState(true, juce::NotificationType::dontSendNotification);
    addAndMakeVisible(bStop);

    bLoop.setColour(juce::TextButton::buttonColourId, GlobalColors::controlBarLoopButtonNotActive);
    bLoop.setColour(juce::TextButton::buttonOnColourId, GlobalColors::controlBarLoopButtonActive);
    bLoop.setLookAndFeel(&LookAndFeel001);
    bLoop.setColour(juce::TextButton::textColourOnId, GlobalColors::white);
    bLoop.setColour(juce::TextButton::textColourOffId, GlobalColors::white);
    bLoop.onClick = [this] { bLoopClicked(); };
    bLoop.setEnabled(true);
    bLoop.setToggleState(false, juce::NotificationType::dontSendNotification);
    addAndMakeVisible(bLoop);
    
    recordButton.onClick = [this] { recordButtonClicked(); };
    recordButton.setColour(juce::TextButton::buttonColourId, GlobalColors::controlBarRecordButtonNotActive);
    recordButton.setColour(juce::TextButton::buttonOnColourId, GlobalColors::controlBarRecordButtonActive);
    recordButton.setLookAndFeel(&LookAndFeel001);
    recordButton.setColour(juce::TextButton::textColourOnId, GlobalColors::white);
    recordButton.setColour(juce::TextButton::textColourOffId, GlobalColors::white);
    addAndMakeVisible(&recordButton);
    
    addAndMakeVisible(AudioMeter);

    lTimeCode.setColour (juce::Label::textColourId, GlobalColors::white);
    lTimeCode.setColour (juce::Label::backgroundColourId, GlobalColors::black);
    lTimeCode.setLookAndFeel(&LookAndFeel003);
    lTimeCode.setFont(GlobalFonts::titleFont);
    lTimeCode.setText("TIMECODE", juce::dontSendNotification);
    lTimeCode.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(lTimeCode);
                                    
}

ControlBarComponent::~ControlBarComponent()
{
}

void ControlBarComponent::sliderValueChanged(juce::Slider *slider)
{
    if (slider == &sVolumeDial)
    {
        std::cout << "volume: " << sVolumeDial.getValue() << std::endl;
        findParentComponentOfClass<MainComponent>()->setVolume(sVolumeDial.getValue());
    }
}

void ControlBarComponent::bPlayClicked()
{
//    DBG("play clicked");
    findParentComponentOfClass<MainComponent>()->transportStateChanged(MainComponent::Play);
}

void ControlBarComponent::bStopClicked()
{
//    DBG("stop clicked");
    findParentComponentOfClass<MainComponent>()->transportStateChanged(MainComponent::Stop);
}

void ControlBarComponent::bPauseClicked()
{
//    DBG("stop clicked");
    findParentComponentOfClass<MainComponent>()->transportStateChanged(MainComponent::Pause);
}

void ControlBarComponent::bLoopClicked()
{
    toggleLoop();
}

void ControlBarComponent::toggleLoop()
{
    if (bLoop.getToggleState())
    {
        bLoop.setToggleState(false, juce::NotificationType::dontSendNotification);
    }else
    {
        bLoop.setToggleState(true, juce::NotificationType::dontSendNotification);
    }
    std::cout << "loop toggled: " << bLoop.getToggleState() << std::endl;
    findParentComponentOfClass<MainComponent>()->setTransportLoop(bLoop.getToggleState());
}

void ControlBarComponent::recordButtonClicked()
{
//    DBG("export Button clicked");
    findParentComponentOfClass<MainComponent>()->toggleExportState();
}

void ControlBarComponent::paint (juce::Graphics& g)
{
    g.fillAll(GlobalColors::controlBarBG);
    // Draw BG
//    g.drawImage(imgBG, getLocalBounds().toFloat(), juce::RectanglePlacement::fillDestination, false);
}

void ControlBarComponent::resized()
{
    //init FlexBoxes
    juce::FlexBox controlBarFB;
    juce::FlexBox volumeFB;
    juce::FlexBox toolsFB;
    juce::FlexBox mainControlsFB;
    juce::FlexBox infoFB;
    
    //ControlBar FlexBox
    controlBarFB.flexWrap = juce::FlexBox::Wrap::noWrap;
    controlBarFB.alignContent = juce::FlexBox::AlignContent::center;
    
    controlBarFB.items.add(juce::FlexItem(volumeFB).withMinWidth(getLocalBounds().getWidth()* 1/6));
    controlBarFB.items.add(juce::FlexItem(toolsFB).withMinWidth(getLocalBounds().getWidth()* 1/6));
    controlBarFB.items.add(juce::FlexItem(mainControlsFB).withMinWidth(getLocalBounds().getWidth()* 2/6));
    controlBarFB.items.add(juce::FlexItem(infoFB).withMinWidth(getLocalBounds().getWidth()* 2/6));
    
    
    //Volume FlexBox
    volumeFB.items.add(juce::FlexItem(sVolumeDial).withMinWidth(getLocalBounds().getWidth()* 1/6).withMaxHeight(getHeight()*0.95).withMargin(8));
    
    
    //Tools FlexBox
    toolsFB.flexDirection = juce::FlexBox::Direction::column;
    toolsFB.alignContent = juce::FlexBox::AlignContent::flexStart;
    
    float buttonMargin = 1;
    toolsFB.items.add(juce::FlexItem(bEins).withFlex(1).withMargin(buttonMargin));
    toolsFB.items.add(juce::FlexItem(bZwei).withFlex(1).withMargin(buttonMargin));
    toolsFB.items.add(juce::FlexItem(bDrei).withFlex(1).withMargin(buttonMargin));
    toolsFB.items.add(juce::FlexItem(bVier).withFlex(1).withMargin(buttonMargin));
    toolsFB.items.add(juce::FlexItem(bFünf).withFlex(1).withMargin(buttonMargin));
    toolsFB.items.add(juce::FlexItem(bSechs).withFlex(1).withMargin(buttonMargin));
    
    
    //MainContols FlexBox
    mainControlsFB.flexWrap = juce::FlexBox::Wrap::wrap;
    mainControlsFB.justifyContent = juce::FlexBox::JustifyContent::center;
    
    mainControlsFB.items.add(juce::FlexItem(Logo).withMinWidth(getLocalBounds().getWidth()* 2/6).withHeight(getHeight()*3/4-25));
    mainControlsFB.items.add(juce::FlexItem(bPlay).withMinWidth(60).withHeight(getHeight()/4));
    mainControlsFB.items.add(juce::FlexItem(bPause).withMinWidth(60).withHeight(getHeight()/4));
    mainControlsFB.items.add(juce::FlexItem(bStop).withMinWidth(60).withHeight(getHeight()/4));
    mainControlsFB.items.add(juce::FlexItem(bLoop).withMinWidth(60).withHeight(getHeight()/4));
    mainControlsFB.items.add(juce::FlexItem(recordButton).withMinWidth(60).withHeight(getHeight()/4));
    
    
    //Info FlexBox
    infoFB.flexDirection = juce::FlexBox::Direction::column;
    infoFB.alignContent = juce::FlexBox::AlignContent::flexStart;
    
//    infoFB.items.add(juce::FlexItem(lMeter).withFlex(1));
    infoFB.items.add(juce::FlexItem(AudioMeter).withFlex(1));
    infoFB.items.add(juce::FlexItem(lTimeCode).withFlex(1));
    
    
    //performFlexLayout
    //bounds der äußeren FlexBox festlegen um die verchachtelten FlexBoxen damit auch zu definieren
    controlBarFB.performLayout(getLocalBounds());
}
