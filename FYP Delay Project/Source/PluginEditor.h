/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class FYPDelayProjectAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FYPDelayProjectAudioProcessorEditor (FYPDelayProjectAudioProcessor&);
    ~FYPDelayProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    
    Slider feedbackDelSlider;
    Slider timeSlider;
    Slider saturationSlider;
    /*
    Slider feedbackSlider;
    Slider vibratoSlider;
    Slider mixSlider;
    Slider inGainSlider;
    Slider outGainSlider;
    */
    Label feedbackDelLabel;
    Label timeLabel;
    Label saturationLabel;
    /*
    Label feedbackLabel;
    Label vibratoLabel;
    Label mixLabel;
    Label inLabel;
    Label outLabel;
     */
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> feedbackDelSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> timeSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> saturationSliderAttachment;

    /*
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> feedbackSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> vibratoSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mixSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> inGainSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> outGainSliderAttachment;
    */
    
    
    FYPDelayProjectAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FYPDelayProjectAudioProcessorEditor)
};
