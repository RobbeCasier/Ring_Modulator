/*
  ==============================================================================

    ComboBox_RC.cpp
    Created: 3 Jun 2023 3:13:56pm
    Author:  robbe

  ==============================================================================
*/

#include "ComboBox_RC.h"

CustomComboBox::CustomComboBox(juce::RangedAudioParameter& parameter)
{
    auto& parameters = dynamic_cast<juce::AudioParameterChoice&>(parameter);
    addItemList(parameters.choices, 1);
}

CustomComboBox::~CustomComboBox()
{
}
