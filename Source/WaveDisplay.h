/*
  ==============================================================================

    WaveDisplay.h
    Created: 3 Jun 2023 7:21:41pm
    Author:  robbe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class WaveDisplay final: public juce::Component, public juce::AudioProcessorValueTreeState::Listener
{
public:
    WaveDisplay(juce::AudioProcessorValueTreeState& apvts, const juce::String& parameter, const float& percentUsageOfBounds);
    ~WaveDisplay();

    void paint(juce::Graphics& graphics) override;
    void resized() override;

    void parameterChanged(const juce::String& parameterID, float newValue) override;
private:

    juce::AudioProcessorValueTreeState& apvts;
    juce::String param;
    juce::Rectangle<int> display;

    float percentageUsageOfBounds = 1.f;

    const float numCycles = 1.f;
};