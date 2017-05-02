/*
  ==============================================================================

    Mixer.h
    Created: 6 Mar 2017 7:26:34pm
    Author:  Samuel Thompson Parke-Wolfe

  ==============================================================================
*/

#ifndef MIXER_H_INCLUDED
#define MIXER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include <random>
#include "LookAndFeels.h"
#include "Track.h"
#include "Compressor.h"
#include <array>

class CompressorGui
:
public Component
{
public:
    Slider sliders[6];
    Label labels[6];
    
    CompressorGui(Slider::Listener *listener)
    {
        labels[0].setText("Input Gain", dontSendNotification);
        sliders[0].setRange(-64.0, 6.0);
        sliders[0].setValue(0.0);
        labels[1].setText("Ratio", dontSendNotification);
        sliders[1].setRange(0.0, 1.0);
        sliders[1].setValue(0.5);
        labels[2].setText("Attack", dontSendNotification);
        sliders[2].setRange(0, 10);
        sliders[2].setValue(2.0);
        labels[3].setText("Release", dontSendNotification);
        sliders[3].setRange(0, 500);
        sliders[3].setValue(19.8);
        labels[4].setText("Threshold", dontSendNotification);
        sliders[4].setRange(-64.0, 6.0);
        sliders[4].setValue(0);
        labels[5].setText("Output Gain", dontSendNotification);
        sliders[5].setRange(-64.0, 6.0);
        sliders[5].setValue(0);
        
        for(int i = 0; i<6; i++)
        {
            addAndMakeVisible(&sliders[i]);
            sliders[i].addListener(listener);
            sliders[i].setSliderStyle(Slider::LinearBarVertical);
            labels[i].attachToComponent (&sliders[i], false);
        }
    }
    
    void paint (Graphics& g) override
    {
        
    }
    void resized() override
    {
        Rectangle<int> areaSliders(getLocalBounds());
        areaSliders.removeFromTop(40);
        areaSliders.removeFromBottom(20);
        float sliderGap = (areaSliders.getWidth()/6)*0.2;;
        float sliderWidth = (areaSliders.getWidth()/6)*0.6;
        
        for(int i = 0; i<6; i++)
        {
            areaSliders.removeFromLeft(sliderGap);
            sliders[i].setBounds(areaSliders.removeFromLeft(sliderWidth));
            areaSliders.removeFromLeft(sliderGap);
        }
    }
    
    void sliderValueChanged(Slider* slider, Track* track)
    {
        if(slider == &sliders[0])
        {
            track->compressor.set_input(Decibels::decibelsToGain(slider->getValue()));
        }
        if(slider == &sliders[1])
        {
            track->compressor.set_ratio(slider->getValue());
        }
        if(slider == &sliders[2])
        {
            track->compressor.set_attack(round(slider->getValue()* 44.1));
            //std::cout << (slider->getValue() * 44.1) << std::endl;
        }
        if(slider == &sliders[3])
        {
            track->compressor.set_release(round(slider->getValue() * 44.1));
            //std::cout << (slider->getValue() * 44.1) << std::endl;
        }
        if(slider == &sliders[4])
        {
            track->compressor.set_threshold(Decibels::decibelsToGain(slider->getValue()));
            //std::cout << Decibels::decibelsToGain(slider->getValue()) << std::endl;
        }
        if(slider == &sliders[5])
        {
            track->compressor.set_output(Decibels::decibelsToGain(slider->getValue()));
        }
    }
    
    
};

class Mixer
:
public Component,
public AudioSource,
public Button::Listener,
public Slider::Listener,
public Timer
{
public:
    
    bool compressorOn;
    //GUI's not perminent
    Slider sliders[6];
    Label labels[6];
    ToggleButton editButtons[5];
    int currentCompressor;
    
    std::array<CompressorGui*, 5> compressorGuis;
    
    AudioFormatManager formatManager;
    
    std::vector<ToggleButton*> buttons;
    Synthesiser synth;

    std::vector<Track*> tracks;
    
    Compressor compressor;
    
    //Midi Sequencing:
    MidiBuffer midiBuffer;
    double BPM = 123.0;
    float number_of_beats_per_bar = 4;
    float number_of_bars = 4;
    
    int time;
    double barLength;
    
    std::vector<bool> midiEvents;
    
    int maxNumButtonsX, maxNumButtonsY;
    std::vector<std::vector<int>> buttonPos;
    
public:
    Mixer();
    ~Mixer();
    
    void paint (Graphics& g) override;
    void resized() override;
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    
    void buttonClicked(Button* button) override;
    void sliderValueChanged(Slider* slider) override;

    void timerCallback() override
    {
        repaint();
        resized();
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Mixer)
};



#endif  // MIXER_H_INCLUDED
