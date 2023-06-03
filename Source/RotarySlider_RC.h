/*
  ==============================================================================

    RotarySlider_RC.h
    Created: 3 Jun 2023 11:46:42am
    Author:  robbe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

struct LookAndFeel : juce::LookAndFeel_V4
{
    void drawRotarySlider (juce::Graphics&,
                            int x, int y, int width, int height,
                            float sliderPosProportional,
                            float rotaryStartAngle,
                            float rotaryEndAngle,
                            juce::Slider&) override{}
};

class RotarySlider : public juce::Slider
{
public:
    RotarySlider(juce::RangedAudioParameter& parameter, const juce::String& unitSuffix);
    ~RotarySlider();

    //override
    void paint(juce::Graphics& graphics) override;

    //My functions
    juce::Rectangle<int> GetSliderBounds() const;
    int GetTextHeight() const { return 14; }
    juce::String GetDisplayString() const;
private:
    LookAndFeel lnf;

    juce::RangedAudioParameter* param;
    juce::String suffix;
};