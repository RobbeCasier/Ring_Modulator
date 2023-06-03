/*
  ==============================================================================

    ComboBox_RC.h
    Created: 3 Jun 2023 3:13:56pm
    Author:  robbe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CustomComboBox : public juce::ComboBox
{
public:
    CustomComboBox(juce::RangedAudioParameter& parameter);
    ~CustomComboBox();
private:
};