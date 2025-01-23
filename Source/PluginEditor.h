/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Scale.h"


//==============================================================================
/**
*/
class PlatooAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer,
private juce::ComboBox::Listener
{
public:
    PlatooAudioProcessorEditor (PlatooAudioProcessor&);
    ~PlatooAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void comboBoxChanged(juce::ComboBox *box) override;
    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PlatooAudioProcessor& audioProcessor;
    juce::ComboBox keyDropdown;
    juce::ComboBox scaleDropdown;
    juce::ToggleButton octDown;

    juce::Rectangle<int> keyC;
    juce::Rectangle<int> keyDb;
    juce::Rectangle<int> keyD;
    juce::Rectangle<int> keyEb;
    juce::Rectangle<int> keyE;
    juce::Rectangle<int> keyF;
    juce::Rectangle<int> keyGb;
    juce::Rectangle<int> keyG;
    juce::Rectangle<int> keyAb;
    juce::Rectangle<int> keyA;
    juce::Rectangle<int> keyBb;
    juce::Rectangle<int> keyB;

    platoo::Scale scale;
    std::map<std::string, juce::Colour> notesColours{
        {"naturalNotActive", juce::Colour(23, 90, 105)},
        {"naturalActive", juce::Colour(59, 219, 255)},
        {"sharpNotActive", juce::Colour (12, 21, 25)},
        {"sharpActive", juce::Colour(22, 173, 209)},
    };
    
//    std::map<std::string, juce::Colour> colours{
//        {"background": }
//    };
    
    juce::Colour backgroundColour = juce::Colour(234, 229, 219);

    std::map<int, juce::Rectangle<int>> itor;
    void setUpDropdown(juce::ComboBox &dropdown, std::vector<std::string> &options, int selectedOption, bool addListener);
    
    int currentNoteNumber;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlatooAudioProcessorEditor)
};
