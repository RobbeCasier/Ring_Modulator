/*
  ==============================================================================

    WaveDisplay.cpp
    Created: 3 Jun 2023 7:21:41pm
    Author:  robbe

  ==============================================================================
*/

#include "WaveDisplay.h"
#include "WaveModulationUtility.h"

WaveDisplay::WaveDisplay(juce::AudioProcessorValueTreeState& apvts, const juce::String& parameter, const float& percentUsageOfBounds):
    apvts{apvts},
    param{parameter},
    percentageUsageOfBounds{ percentUsageOfBounds }
{
    apvts.addParameterListener(param, this);
}

WaveDisplay::~WaveDisplay()
{
    apvts.removeParameterListener(param, this);
}

void WaveDisplay::paint(juce::Graphics& graphics)
{
    graphics.setColour(juce::Colour(20u, 20u, 20u));
    graphics.fillRoundedRectangle(display.toFloat(), 15.f);

    juce::Path wavePath;

    const int width = display.getWidth() - 20;
    const int height = display.getHeight() - 20;

    const int numPoints = width;
    const float phaseIncrement = juce::MathConstants<float>::twoPi * numCycles / numPoints;

    for (int x = 0; x < numPoints; x++)
    {
        const float phase = x * phaseIncrement;
        const float modulation = Wave::GetModulationSignal((Wave::WaveType)apvts.getRawParameterValue(param)->load(), phase);
        const float y = height * 0.5f * (1.0f + modulation);
        const juce::Point<float> point(static_cast<float>(x) + display.getX() + 5, y+22.5);
        if (x == 0)
            wavePath.startNewSubPath(point);
        else
            wavePath.lineTo(point);
    }

    graphics.setColour(juce::Colours::whitesmoke);
    graphics.strokePath(wavePath, juce::PathStrokeType(2.f));
}

void WaveDisplay::resized()
{
    display = getLocalBounds();
    auto centre = display.getCentre();

    int offSet = display.getHeight() - display.getHeight() * percentageUsageOfBounds;
    display.setSize(display.getWidth() - offSet, display.getHeight() - offSet);
    display.setCentre(centre);
}

void WaveDisplay::parameterChanged(const juce::String& parameterID, float newValue)
{
    repaint();
}


