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
    juce::Slider& slider)
{
    auto bounds = juce::Rectangle<float>(x, y, width, height);

    graphics.setColour(juce::Colour(153u, 0u, 0u));
    graphics.fillEllipse(bounds);

    graphics.setColour(juce::Colour(200u, 0u, 0u));
    graphics.drawEllipse(bounds, 1.f);

    graphics.setColour(juce::Colour(20u, 20u, 20u));
    if (auto* rs = dynamic_cast<RotarySlider*>(&slider))
    {
        auto center = bounds.getCentre();

        //draw arrow knob
        juce::Path p;

        juce::Rectangle<float> r;
        r.setLeft(center.getX() - 5);
        r.setRight(center.getX() + 5);
        r.setTop(bounds.getY());
        r.setBottom(center.getY()- rs->GetTextHeight() * 3);
        
        p.addRoundedRectangle(r, 2.f);

        jassert(rotaryStartAngle < rotaryEndAngle);

        auto sliderAngleInRad = juce::jmap(sliderPosProportional, 0.f, 1.f, rotaryStartAngle, rotaryEndAngle);

        p.applyTransform(juce::AffineTransform().rotated(sliderAngleInRad, center.getX(), center.getY()));

        graphics.fillPath(p);

        //Draw Box for Text
        graphics.setFont(rs->GetTextHeight());
        auto text = rs->GetDisplayString();
        auto strWidth = graphics.getCurrentFont().getStringWidth(text);

        r.setSize(80, rs->GetTextHeight() + 4);
        r.setCentre(bounds.getCentre());

        graphics.setColour(juce::Colour(100u, 0u, 0u));
        graphics.fillRect(r);

        graphics.setColour(juce::Colours::whitesmoke);
        graphics.drawFittedText(text, r.toNearestInt(), juce::Justification::centred, 1);
    }



}

//==============================================================================
RotarySlider::RotarySlider(juce::RangedAudioParameter& parameter, const juce::String& name, const juce::String& unitSuffix):
    juce::Slider(juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
    param(&parameter),
    suffix(unitSuffix),
    name(name)
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

    //Draw Label
    graphics.setColour(juce::Colours::whitesmoke);
    graphics.setFont(GetTextHeight());

    juce::Rectangle<float> r;
    r.setSize(graphics.getCurrentFont().getStringWidth(name), GetTextHeight());
    auto bounds = getLocalBounds();
    r.setCentre(bounds.getCentreX(), GetTextHeight());

    graphics.drawFittedText(name, r.toNearestInt(), juce::Justification::centred, 1);
}

juce::Rectangle<int> RotarySlider::GetSliderBounds() const
{
    auto bounds = getLocalBounds();

    auto size = juce::jmin(bounds.getWidth(), bounds.getHeight());

    size -= GetTextHeight() * 3;
    juce::Rectangle<int> r;
    r.setSize(size, size);
    r.setCentre(bounds.getCentreX(), 0);
    r.setY(GetTextHeight() * 2);

    return r;
}

juce::String RotarySlider::GetDisplayString() const
{
    juce::String str;

    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(param))
    {
        float val = getValue();

        str = juce::String(val, 1);
    }

    if (auto* intParam = dynamic_cast<juce::AudioParameterInt*>(param))
    {
        float val = getValue();
        str = juce::String(val,0);
    }

    if (suffix.isNotEmpty())
    {
        str << suffix;
    }

    return str;
}

juce::String RotarySlider::GetName() const
{
    return name;
}

