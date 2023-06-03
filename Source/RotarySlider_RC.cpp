/*
  ==============================================================================

    RotarySlider_RC.cpp
    Created: 3 Jun 2023 11:46:42am
    Author:  robbe

  ==============================================================================
*/

#include "RotarySlider_RC.h"

RotarySlider::RotarySlider(juce::RangedAudioParameter& parameter, const juce::String& unitSuffix):
    juce::Slider(juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
    param(&parameter),
    suffix(unitSuffix)
{
    setLookAndFeel(&lnf);
}

RotarySlider::~RotarySlider()
{
    setLookAndFeel(nullptr);
}

void RotarySlider::paint(juce::Graphics& graphics)
{
}

juce::Rectangle<int> RotarySlider::GetSliderBounds() const
{
    return juce::Rectangle<int>();
}

juce::String RotarySlider::GetDisplayString() const
{
    return juce::String();
}
