/*
  ==============================================================================

    Globals.h
    Created: 9 Feb 2024 10:42:05am
    Author:  Klaas Klee

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

const float globalSampleRate = 44100;

const float volumeOnStartUp = 0.5f;

const int numOfLayers = 5;

const int layerHeight = 200;
const int layerControlW = 250;
const int waveBorder = 15;
const int labelW = 36;
const int playHeadW = 15;
const int globalNumChannels = 2;

// timeLineSize in seconds OnStartUp
const int timeLineSizeOnStartUp = 90;

// exportTime in seconds
const float exportTime = 60;


// if you add a Blendmode
// add BlendModeName in LayerControlComponent.h
// change last Blendmode in LayerControlComponent.cpp line 42
// add Blendmode in LayerBlendModeControlComponent and make changes according to parameters the Blendmode needs

enum BlendModes
{
    Normal,
    Multiply,
    Duck,
    VariableFilter,
    Binary
};

enum AlertWindows
{
    ExportWindow,
    SetTimeLineSizeWindow
};


//==============================================================================
//Styling COLORS
//==============================================================================

namespace GlobalColors {

    const juce::Colour black = juce::Colour(0xff000000);
    const juce::Colour white = juce::Colour(0xffffffff);

    const juce::Colour dropShadow = juce::Colour(0x60000000);

    const juce::Colour brightText = juce::Colour(0xffffffff);
    const juce::Colour darkText = juce::Colour(0xff000000);

    const juce::Colour buttonActive = juce::Colour(0xffbd3a31);
    const juce::Colour buttonNotActive = juce::Colour(0xff000000);

    // ControlBar
    const juce::Colour controlBarBG = juce::Colour(0xff9E9E9E);

    const juce::Colour controlBarPlayButtonActive = juce::Colour(0xff398732);
    const juce::Colour controlBarPlayButtonNotActive = juce::Colour(0xff000000);
    const juce::Colour controlBarPauseButtonActive = juce::Colour(0xffA16C2E);
    const juce::Colour controlBarPauseButtonNotActive = juce::Colour(0xff000000);
    const juce::Colour controlBarStopButtonActive = juce::Colour(0xff824242);
    const juce::Colour controlBarStopButtonNotActive = juce::Colour(0xff000000);
    const juce::Colour controlBarLoopButtonActive = juce::Colour(0xff2e516e);
    const juce::Colour controlBarLoopButtonNotActive = juce::Colour(0xff000000);
    const juce::Colour controlBarRecordButtonActive = juce::Colour(0xff824242);
    const juce::Colour controlBarRecordButtonNotActive = juce::Colour(0xff000000);

    const juce::Colour controlBarTimeStampBG = juce::Colour(0xff8D8D8D);

    // PlayHeadRuler
    const juce::Colour playHeadRulerBG = juce::Colour(0xff8D8D8D);
//  const juce::Colour playHeadRulerLines = juce::Colour(0xffffffff);

    // Layer
    const juce::Colour bG = juce::Colour(0xffCFCFCF);
    const juce::Colour layerLabel = juce::Colour(0xff9E9E9E);
    // LayerControl
    const juce::Colour layerControlBg = juce::Colour(0xffD9D9D9);
    // LayerWave
    const juce::Colour layerWaveBg = juce::Colour(0xffD9D9D9);


}

namespace GlobalFonts {
const juce::Font titleFont = juce::Font("Eastman Roman Alt", "Regular", 40.0f);
const juce::Font mainFont = juce::Font("Eastman Roman Alt", "Regular", 20.0f);
}

//==============================================================================
//Styling - LOOKANDFEEL
//==============================================================================

// Main Styling LookAndFeel
class LookAndFeel001 : public juce::LookAndFeel_V4
{
public:
    
    void drawRotarySlider (juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider)
    {
        float centerX = x + width/2;
        float centerY = y + height/2;
        
        float topOffset = centerX - (centerX-(centerX/10));
        
        float diameter = fmin(width, height - topOffset*2);
        float radius = diameter/2;
        float angle = rotaryStartAngle + (sliderPos* (rotaryEndAngle - rotaryStartAngle));
        juce::Rectangle<float> dialArea (centerX-radius,centerY-radius+topOffset/2,diameter,diameter);

        juce::Path triangle;
        triangle.addTriangle(centerX-topOffset, 0, centerX+topOffset, 0, centerX, topOffset);
        
        g.setColour(GlobalColors::black);
        g.fillEllipse(dialArea);
        g.fillPath(triangle);
        
        g.setColour(GlobalColors::white);
        juce::Path dialHandle;
        dialHandle.addEllipse(0-radius*0.28/2, -radius*0.82, radius*0.28, radius*0.26);
        g.fillPath(dialHandle, juce::AffineTransform::rotation(angle).translated(centerX, centerY));
    }
    
    void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                                           float sliderPos, float minSliderPos, float maxSliderPos,
                                           const juce::Slider::SliderStyle style, juce::Slider& slider)
    {
//        g.fillAll (slider.findColour (juce::Slider::backgroundColourId));

        if (style == juce::Slider::LinearBar || style == juce::Slider::LinearBarVertical)
        {
            const float fx = (float) x, fy = (float) y, fw = (float) width, fh = (float) height;

            juce::Path p;

            if (style == juce::Slider::LinearBarVertical)
                p.addRectangle (fx, sliderPos, fw, 1.0f + fh - sliderPos);
            else
                p.addRectangle (fx, fy, sliderPos - fx, fh);

            auto baseColour = slider.findColour (juce::Slider::thumbColourId)
                                    .withMultipliedSaturation (slider.isEnabled() ? 1.0f : 0.5f)
                                    .withMultipliedAlpha (0.8f);

            g.setGradientFill (juce::ColourGradient::vertical (baseColour.brighter (0.08f), 0.0f,
                                                         baseColour.darker (0.08f), (float) height));
            g.fillPath (p);

            g.setColour (baseColour.darker (0.2f));

            if (style == juce::Slider::LinearBarVertical)
                g.fillRect (fx, sliderPos, fw, 1.0f);
            else
                g.fillRect (sliderPos, fy, 1.0f, fh);

            drawLinearSliderOutline (g, x, y, width, height, style, slider);
        }
        else
        {
            drawLinearSliderBackground (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
            drawLinearSliderThumb (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
        }
    }
    void drawLinearSliderThumb (juce::Graphics& g, int x, int y, int width, int height,
                                                float sliderPos, float minSliderPos, float maxSliderPos,
                                                const juce::Slider::SliderStyle style, juce::Slider& slider)
    {
        auto sliderRadius = (float) (getSliderThumbRadius (slider) - 2);

        auto knobColour = GlobalColors::white;

        const float outlineThickness = slider.isEnabled() ? 0.8f : 0.3f;

        if (style == juce::Slider::LinearHorizontal || style == juce::Slider::LinearVertical)
        {
            float kx, ky;

            if (style == juce::Slider::LinearVertical)
            {
                kx = (float) x + (float) width * 0.5f;
                ky = sliderPos;
            }
            else
            {
                kx = sliderPos;
                ky = (float) y + (float) height * 0.5f;
            }

//            drawGlassSphere (g,
//                             kx - sliderRadius,
//                             ky - sliderRadius,
//                             sliderRadius * 2.0f,
//                             knobColour, outlineThickness);

            g.setColour (knobColour);
            g.fillRect(kx - sliderRadius/2, ky - sliderRadius/2, sliderRadius/2 * 2.0f, sliderRadius/2 * 2.0f);
        }
        else
        {
            if (style == juce::Slider::ThreeValueVertical)
            {
                drawGlassSphere (g, (float) x + (float) width * 0.5f - sliderRadius,
                                 sliderPos - sliderRadius,
                                 sliderRadius * 2.0f,
                                 knobColour, outlineThickness);
            }
            else if (style == juce::Slider::ThreeValueHorizontal)
            {
                drawGlassSphere (g,sliderPos - sliderRadius,
                                 (float) y + (float) height * 0.5f - sliderRadius,
                                 sliderRadius * 2.0f,
                                 knobColour, outlineThickness);
            }

            if (style == juce::Slider::TwoValueVertical || style == juce::Slider::ThreeValueVertical)
            {
                auto sr = juce::jmin (sliderRadius, (float) width * 0.4f);

                drawGlassPointer (g, juce::jmax (0.0f, (float) x + (float) width * 0.5f - sliderRadius * 2.0f),
                                  minSliderPos - sliderRadius,
                                  sliderRadius * 2.0f, knobColour, outlineThickness, 1);

                drawGlassPointer (g,
                                  juce::jmin ((float) x + (float) width - sliderRadius * 2.0f,
                                        (float) x + (float) width * 0.5f),
                                  maxSliderPos - sr,
                                  sliderRadius * 2.0f,
                                  knobColour,
                                  outlineThickness,
                                  3);
            }
            else if (style == juce::Slider::TwoValueHorizontal || style == juce::Slider::ThreeValueHorizontal)
            {
                auto sr = juce::jmin (sliderRadius, (float) height * 0.4f);

                drawGlassPointer (g, minSliderPos - sr,
                                  juce::jmax (0.0f, (float) y + (float) height * 0.5f - sliderRadius * 2.0f),
                                  sliderRadius * 2.0f, knobColour, outlineThickness, 2);

                drawGlassPointer (g,
                                  maxSliderPos - sliderRadius,
                                  juce::jmin ((float) y + (float) height - sliderRadius * 2.0f,
                                        (float) y + (float) height * 0.5f),
                                  sliderRadius * 2.0f,
                                  knobColour,
                                  outlineThickness,
                                  4);
            }
        }
    }
    
    void drawLinearSliderBackground (juce::Graphics& g, int x, int y, int width, int height,
                                                     float /*sliderPos*/,
                                                     float /*minSliderPos*/,
                                                     float /*maxSliderPos*/,
                                                     const juce::Slider::SliderStyle /*style*/, juce::Slider& slider)
    {
        const float sliderRadius = (float) (getSliderThumbRadius (slider) - 2);

        const juce::Colour trackColour (GlobalColors::black);
        const juce::Colour gradCol1 (juce::Colours::black);
        const juce::Colour gradCol2 (juce::Colours::black);
        juce::Path indent;

        if (slider.isHorizontal())
        {
            auto iy = (float) y + (float) height * 0.5f - sliderRadius * 0.5f;

            g.setGradientFill (juce::ColourGradient::vertical (gradCol1, iy, gradCol2, iy + sliderRadius));

//            indent.addRoundedRectangle ((float) x - sliderRadius * 0.5f, iy, (float) width + sliderRadius, sliderRadius, 5.0f);
            indent.addRoundedRectangle ((float) x - sliderRadius * 0.5f, iy, (float) width + sliderRadius, sliderRadius, 0.0f);
        }
        else
        {
            auto ix = (float) x + (float) width * 0.5f - sliderRadius * 0.5f;

            g.setGradientFill (juce::ColourGradient::horizontal (gradCol1, ix, gradCol2, ix + sliderRadius));

            indent.addRoundedRectangle (ix, (float) y - sliderRadius * 0.5f, sliderRadius, (float) height + sliderRadius, 0.0f);
        }

        g.fillPath (indent);

        g.setColour (trackColour.contrasting (0.5f));
        g.strokePath (indent, juce::PathStrokeType (0.5f));
    }
    
    void drawToggleButton (juce::Graphics &g, juce::ToggleButton &button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
    {
        auto fontSize = fmin (15.0f, (float) button.getHeight() * 0.75f);
        auto tickWidth = fontSize * 1.1f;

        drawTickBox (g, button, 4.0f, ((float) button.getHeight() - tickWidth) * 0.5f,
                     tickWidth, tickWidth,
                     button.getToggleState(),
                     button.isEnabled(),
                     shouldDrawButtonAsHighlighted,
                     shouldDrawButtonAsDown);

        g.setColour (button.findColour (juce::ToggleButton::textColourId));
        g.setFont (fontSize);

        if (! button.isEnabled())
            g.setOpacity (0.5f);

        g.drawFittedText (button.getButtonText(),
                          button.getLocalBounds().withTrimmedLeft (juce::roundToInt (tickWidth) + 10)
                                                 .withTrimmedRight (2),
                          juce::Justification::centredLeft, 10);
    }
    void drawTickBox (juce::Graphics& g, juce::Component& component,
                                      float x, float y, float w, float h,
                                      const bool ticked,
                                      [[maybe_unused]] const bool isEnabled,
                                      [[maybe_unused]] const bool shouldDrawButtonAsHighlighted,
                                      [[maybe_unused]] const bool shouldDrawButtonAsDown)
    {
        juce::Rectangle<float> tickBounds (x, y, w, h);

        g.setColour (GlobalColors::buttonNotActive);
        g.fillRect (tickBounds);

        if (ticked)
        {
            g.setColour (GlobalColors::buttonActive);
            g.fillRect (tickBounds);
        }
    }
    
    void drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                                               bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
    {
        juce::Colour baseColour (backgroundColour.withMultipliedSaturation (button.hasKeyboardFocus (true) ? 1.3f : 0.9f)
                                           .withMultipliedAlpha (button.isEnabled() ? 0.9f : 0.5f));

        if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
            baseColour = baseColour.contrasting (shouldDrawButtonAsDown ? 0.2f : 0.1f);

    //    const bool flatOnLeft   = button.isConnectedOnLeft();
    //    const bool flatOnRight  = button.isConnectedOnRight();
    //    const bool flatOnTop    = button.isConnectedOnTop();
    //    const bool flatOnBottom = button.isConnectedOnBottom();

        const float width  = (float) button.getWidth()  - 1.0f;
        const float height = (float) button.getHeight() - 1.0f;

        if (width > 0 && height > 0)
        {
    //        const float cornerSize = 4.0f;

            juce::Path outline;
    //        outline.addRoundedRectangle (0.5f, 0.5f, width, height, cornerSize, cornerSize,
    //                                     ! (flatOnLeft  || flatOnTop),
    //                                     ! (flatOnRight || flatOnTop),
    //                                     ! (flatOnLeft  || flatOnBottom),
    //                                     ! (flatOnRight || flatOnBottom));
            outline.addRectangle(0.5f, 0.5f, width, height);

            drawButtonShape (g, outline, baseColour, height);
        }
    }
    static void drawButtonShape (juce::Graphics& g, const juce::Path& outline, juce::Colour baseColour, float height)
    {
    //    const float mainBrightness = baseColour.getBrightness();
    //    const float mainAlpha = baseColour.getFloatAlpha();

    //    g.setGradientFill (ColourGradient::vertical (baseColour.brighter (0.2f), 0.0f,
    //                                                 baseColour.darker (0.25f), height));
        g.setGradientFill (juce::ColourGradient::vertical (baseColour, 0.0f,
                                                     baseColour, height));
        g.fillPath (outline);

    //    g.setColour (Colours::white.withAlpha (0.4f * mainAlpha * mainBrightness * mainBrightness));
    //    g.strokePath (outline, PathStrokeType (1.0f), AffineTransform::translation (0.0f, 1.0f)
    //                                                        .scaled (1.0f, (height - 1.6f) / height));
    //
    //    g.setColour (Colours::black.withAlpha (0.4f * mainAlpha));
    //    g.strokePath (outline, PathStrokeType (1.0f));
    }
};

// LookAndFeel for Buttons with white Stroke Outline
class LookAndFeel002 : public juce::LookAndFeel_V4
{
public:
    void drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                                               bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
    {
        juce::Colour baseColour (backgroundColour.withMultipliedSaturation (button.hasKeyboardFocus (true) ? 1.3f : 0.9f)
                                           .withMultipliedAlpha (button.isEnabled() ? 0.9f : 0.5f));

        if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
            baseColour = baseColour.contrasting (shouldDrawButtonAsDown ? 0.2f : 0.1f);

        const float width  = (float) button.getWidth()  - 1.0f;
        const float height = (float) button.getHeight() - 1.0f;

        if (width > 0 && height > 0)
        {

            juce::Path outline;

            outline.addRectangle(0.5f, 0.5f, width, height);

            drawButtonShape (g, outline, baseColour, height);
        }
    }
    static void drawButtonShape (juce::Graphics& g, const juce::Path& outline, juce::Colour baseColour, float height)
    {
        g.setGradientFill (juce::ColourGradient::vertical (baseColour, 0.0f,
                                                     baseColour, height));
        g.fillPath (outline);

        g.setColour (GlobalColors::white);
        g.strokePath (outline, juce::PathStrokeType (2.0f));
    }
};

// LookAndFeel explicitly for Timcode Label
class LookAndFeel003 : public juce::LookAndFeel_V4
{
public:
    void drawLabel (juce::Graphics& g, juce::Label& label)
    {
//        g.fillAll (label.findColour (juce::Label::backgroundColourId));
        
        juce::Rectangle<int> bgBounds;
        bgBounds = label.getLocalBounds().reduced(label.getLocalBounds().getWidth()*0.3, label.getLocalBounds().getHeight()*0.1);
        g.fillRect(bgBounds);
    
        if (! label.isBeingEdited())
        {
            auto alpha = label.isEnabled() ? 1.0f : 0.5f;
            const juce::Font font (getLabelFont (label));
    
            g.setColour (label.findColour (juce::Label::textColourId).withMultipliedAlpha (alpha));
            g.setFont (font);
    
            auto textArea = getLabelBorderSize (label).subtractedFrom (label.getLocalBounds());
    
            g.drawFittedText (label.getText(), textArea, label.getJustificationType(),
                              juce::jmax (1, (int) ((float) textArea.getHeight() / font.getHeight())),
                              label.getMinimumHorizontalScale());
    
            g.setColour (label.findColour (juce::Label::outlineColourId).withMultipliedAlpha (alpha));
        }
        else if (label.isEnabled())
        {
            g.setColour (label.findColour (juce::Label::outlineColourId));
        }
    
        g.drawRect (label.getLocalBounds());
    }
};
