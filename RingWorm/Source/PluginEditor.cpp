/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RingWormAudioProcessorEditor::RingWormAudioProcessorEditor (RingWormAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 400);
    
    addAndMakeVisible (hi_l_depth_slider);
    addAndMakeVisible (hi_r_depth_slider);
    addAndMakeVisible (lo_l_depth_slider);
    addAndMakeVisible (lo_r_depth_slider);
    
    addAndMakeVisible (hi_l_rate_slider);
    addAndMakeVisible (hi_r_rate_slider);
    addAndMakeVisible (lo_l_rate_slider);
    addAndMakeVisible (lo_r_rate_slider);
    
    hi_l_depth_slider.setSliderStyle (juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    hi_r_depth_slider.setSliderStyle (juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    lo_l_depth_slider.setSliderStyle (juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    lo_r_depth_slider.setSliderStyle (juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    
    hi_l_rate_slider.setSliderStyle (juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    hi_r_rate_slider.setSliderStyle (juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    lo_l_rate_slider.setSliderStyle (juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    lo_r_rate_slider.setSliderStyle (juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    
    hi_l_depth_slider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
    hi_r_depth_slider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
    lo_l_depth_slider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
    lo_r_depth_slider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
    
    hi_l_rate_slider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
    hi_r_rate_slider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
    lo_l_rate_slider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
    lo_r_rate_slider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    sliderAttachments.emplace_back (std::make_unique <SliderAttachment> (audioProcessor.apvts, "HI_L_RATE", hi_l_rate_slider));
    sliderAttachments.emplace_back (std::make_unique <SliderAttachment> (audioProcessor.apvts, "HI_R_RATE", hi_r_rate_slider));
    sliderAttachments.emplace_back (std::make_unique <SliderAttachment> (audioProcessor.apvts, "LO_L_RATE", lo_l_rate_slider));
    sliderAttachments.emplace_back (std::make_unique <SliderAttachment> (audioProcessor.apvts, "LO_R_RATE", lo_r_rate_slider));
    
    sliderAttachments.emplace_back (std::make_unique <SliderAttachment> (audioProcessor.apvts, "HI_L_DEPTH", hi_l_depth_slider));
    sliderAttachments.emplace_back (std::make_unique <SliderAttachment> (audioProcessor.apvts, "HI_R_DEPTH", hi_r_depth_slider));
    sliderAttachments.emplace_back (std::make_unique <SliderAttachment> (audioProcessor.apvts, "LO_L_DEPTH", lo_l_depth_slider));
    sliderAttachments.emplace_back (std::make_unique <SliderAttachment> (audioProcessor.apvts, "LO_R_DEPTH", lo_r_depth_slider));
}

RingWormAudioProcessorEditor::~RingWormAudioProcessorEditor()
{
}

//==============================================================================
void RingWormAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
}

void RingWormAudioProcessorEditor::resized()
{
    auto knobWidth = getWidth() * 0.25;
    auto knobHeight = getHeight() * 0.25;
    
    auto row2X = getWidth() * 0.75;
    
    hi_l_depth_slider.setBounds (0, 0, knobWidth, knobHeight);
    hi_l_rate_slider.setBounds  (0, hi_l_depth_slider.getBottom(), knobWidth, knobHeight);
    lo_l_depth_slider.setBounds (0, hi_l_rate_slider.getBottom(), knobWidth, knobHeight);
    lo_l_rate_slider.setBounds  (0, lo_l_depth_slider.getBottom(), knobWidth, knobHeight);
    
    hi_r_depth_slider.setBounds (row2X, 0, knobWidth, knobHeight);
    hi_r_rate_slider.setBounds  (row2X, hi_r_depth_slider.getBottom(), knobWidth, knobHeight);
    lo_r_depth_slider.setBounds (row2X, hi_r_rate_slider.getBottom(), knobWidth, knobHeight);
    lo_r_rate_slider.setBounds  (row2X, lo_r_depth_slider.getBottom(), knobWidth, knobHeight);
}
