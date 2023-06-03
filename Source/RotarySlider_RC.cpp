/*
  ==============================================================================

    RotarySlider_RC.cpp
    Created: 3 Jun 2023 11:46:42am
    Author:  robbe

  ==============================================================================
*/

#include "RotarySlider_RC.h"

void LookAndFeel::drawRotarySlider(
    juce::Graphics& graphics, 
    int x, 
    int y, 
    int width, 
    int height, 
    float sliderPosProportional, 
    float rotaryStartAngle, 
    float rotaryEndAngle, 
    juce::Slider&)
{
    auto bounds = juce::Rectangle<float>(x, y, width, height);

    graphics.setColour(juce::Colour(153u, 0u, 0u));
    graphics.fillEllipse(bounds);

    graphics.setColour(juce::Colour(200u, 0u, 0u));
    graphics.drawEllipse(bounds, 1.f);

    //draw arrow knob
    graphics.setColour(juce::Colour(20u, 20u, 20u));
    auto center = bounds.getCentre();

    juce::Path p;

    juce::Rectangle<float> r;
    r.setLeft(center.getX() - 5);
    r.setRight(center.getX() + 5);
    r.setTop(bounds.getY());
    r.setBottom(center.getY()-50);

    p.addRectangle(r);

    jassert(rotaryStartAngle < rotaryEndAngle);

    auto sliderAngleInRad = juce::jmap(sliderPosProportional, 0.f, 1.f, rotaryStartAngle, rotaryEndAngle);

    p.applyTransform(juce::AffineTransform().rotated(sliderAngleInRad, center.getX(), center.getY()));

    graphics.fillPath(p);
}

//==============================================================================
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
    auto startAng = juce::degreesToRadians(180.f + 45.f);
    auto endAng = juce::degreesToRadians(180.f - 45.f) + juce::MathConstants<float>::twoPi;

    auto range = getRange();

    auto sliderBounds = GetSliderBounds();

    getLookAndFeel().drawRotarySlider(
        graphics, 
        sliderBounds.getX(), 
        sliderBounds.getY(), 
        sliderBounds.getWidth(), 
        sliderBounds.getHeight(), 
        juce::jmap(getValue(), range.getStart(), range.getEnd(), 0.0, 1.0),
        startAng, 
        endAng, 
        *this);
}

juce::Rectangle<int> RotarySlider::GetSliderBounds() const
{
    auto bounds = getLocalBounds();

    auto size = juce::jmin(bounds.getWidth(), bounds.getHeight());

    size -= GetTextHeight() * 2;
    juce::Rectangle<int> r;
    r.setSize(size, size);
    r.setCentre(bounds.getCentreX(), 0);
    r.setY(2);

    return r;
}

juce::String RotarySlider::GetDisplayString() const
{
    return juce::String();
}

