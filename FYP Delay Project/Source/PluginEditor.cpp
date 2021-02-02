/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FYPDelayProjectAudioProcessorEditor::FYPDelayProjectAudioProcessorEditor (FYPDelayProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);
    
    feedbackDelSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    feedbackDelSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(feedbackDelSlider);
    feedbackDelLabel.setText("Feedback Delay", dontSendNotification);
    feedbackDelLabel.attachToComponent(&feedbackDelSlider, true);
    addAndMakeVisible(feedbackDelLabel);
    
    timeSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    timeSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(timeSlider);
    timeLabel.setText("Delay Time", dontSendNotification);
    timeLabel.attachToComponent(&timeSlider, true);
    addAndMakeVisible(timeLabel);
    
    saturationSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    saturationSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(saturationSlider);
    saturationLabel.setText("Saturation", dontSendNotification);
    saturationLabel.attachToComponent(&saturationSlider, true);
    addAndMakeVisible(saturationLabel);
    /*
    feedbackSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    feedbackSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(feedbackSlider);
    feedbackLabel.setText("Feedback", dontSendNotification);
    feedbackLabel.attachToComponent(&feedbackSlider, true);
    addAndMakeVisible(feedbackLabel);
    
    vibratoSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    vibratoSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(vibratoSlider);
    vibratoLabel.setText("Vibrato", dontSendNotification);
    vibratoLabel.attachToComponent(&vibratoSlider, true);
    addAndMakeVisible(vibratoLabel);
    
    mixSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mixSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(mixSlider);
    mixLabel.setText("Mix", dontSendNotification);
    mixLabel.attachToComponent(&mixSlider, true);
    addAndMakeVisible(mixLabel);
    
    inGainSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    inGainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(inGainSlider);
    inLabel.setText("Saturation", dontSendNotification);
    inLabel.attachToComponent(&inGainSlider, true);
    addAndMakeVisible(inLabel);
    
    outGainSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    outGainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(outGainSlider);
    outLabel.setText("Output Gain", dontSendNotification);
    outLabel.attachToComponent(&outGainSlider, true);
    addAndMakeVisible(outLabel);
    */
    feedbackDelSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FEEDBACK", feedbackDelSlider);
    
    timeSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TIME", timeSlider);
    
    saturationSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "SATURATION", saturationSlider);
    
    /*
    feedbackSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FEEDBACK", feedbackSlider);
    
    vibratoSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "VIBRATO", vibratoSlider);
    
    mixSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MIX", mixSlider);
    
    inGainSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "INPUT", inGainSlider);
    
    outGainSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OUTPUT", outGainSlider);
     */
}

FYPDelayProjectAudioProcessorEditor::~FYPDelayProjectAudioProcessorEditor()
{
}

//==============================================================================
void FYPDelayProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    
}

void FYPDelayProjectAudioProcessorEditor::resized()
{
    feedbackDelSlider.setBounds(200, 0, 200, 100);
    timeSlider.setBounds(500, 0, 200, 100);
    saturationSlider.setBounds(200, 100, 200, 100);
    /*
    feedbackSlider.setBounds(200, 100, 200, 100);
    vibratoSlider.setBounds(500, 0 , 200, 100);
    mixSlider.setBounds(200, 200, 200, 100);
    inGainSlider.setBounds(200, 300, 200, 100);
    outGainSlider.setBounds(500, 300, 200, 100);
     */
}
