/*
  ==============================================================================

    WaveModulationUtility.h
    Created: 3 Jun 2023 8:28:32pm
    Author:  robbe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace Wave
{
    enum class WaveType
    {
        SINE,
        SQUARE,
        TRIANGLE,
        SAWTOOTH
    };


    inline double GetSineSignal(const float& currentPhase)
    {
        return std::sin(currentPhase);
    }
    inline double GetSquareSignal(const float& currentPhase)
    {
        float modulationSignal = (currentPhase < juce::MathConstants<float>::pi) ? 1.0f : -1.0f;
        return modulationSignal;
    }
    inline double GetTriangleSignal(const float& currentPhase)
    {
        const float normalizedPhase = currentPhase / (2.0f * juce::MathConstants<float>::pi);
        const float shiftedPhase = normalizedPhase - 0.25f;
        const float triangleValue = 1.0f - 4.0f * std::fabs(shiftedPhase - std::floor(shiftedPhase + 0.5f));
        return triangleValue;
    }
    inline double GetSawtoothSignal(const float& currentPhase)
    {
        const float normalizedPhase = currentPhase / juce::MathConstants<float>::twoPi;
        const float fractionalPart = normalizedPhase - std::floor(normalizedPhase + 0.5f);

        return 2.0f * fractionalPart;
    }

    inline double GetModulationSignal(const WaveType& type, const float& currentPhase)
    {
        switch (type)
        {
        case WaveType::SINE:
            return -1.f*GetSineSignal(currentPhase);
            break;
        case WaveType::SQUARE:
            return -1.f * GetSquareSignal(currentPhase);
            break;
        case WaveType::TRIANGLE:
            return -1.f * GetTriangleSignal(currentPhase);
            break;
        case WaveType::SAWTOOTH:
            return -1.f * GetSawtoothSignal(currentPhase);
            break;
        default:
            break;
        }
    }
}