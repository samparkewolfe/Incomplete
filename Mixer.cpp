/*
 ==============================================================================
 
 Mixer.cpp
 Created: 6 Mar 2017 7:26:34pm
 Author:  Samuel Thompson Parke-Wolfe
 
 ==============================================================================
 */

#include "Mixer.h"

Mixer::Mixer()
:
compressorOn(false)
{
    formatManager.registerBasicFormats(); // [1]
    
    maxNumButtonsX = 10;
    maxNumButtonsY = 8;
    
    barLength = ( (60.0/123.0) * /* number_of_beats_per_bar * */ 44100.0);
    //barLength = 21512.25;
    //std::cout << "Beat Length: " << barLength << std::endl;
    for(int i = 0; i<5; i++)
    {
        addAndMakeVisible(&editButtons[i]);
        editButtons[i].addListener(this);
        editButtons[i].setToggleState(false, dontSendNotification);
        
        compressorGuis[i] = new CompressorGui(this);
        addAndMakeVisible(compressorGuis[i]);
    }
    editButtons[0].setToggleState(true, dontSendNotification);
    editButtons[0].setButtonText("Hat");
    editButtons[1].setButtonText("Per");
    editButtons[2].setButtonText("Rer");
    editButtons[3].setButtonText("Noi");
    editButtons[4].setButtonText("Pad");
    
    for(int i = 0; i<8; i++)
    {
        Track* track = new Track;
        track->setBarLength(barLength);
        track->setCurrentSampleLength(barLength);
        addAndMakeVisible(track);
        tracks.push_back(track);
    }
    
    for(int i = 0; i<2; i++)
    {
        Track_Oneshot* track_oneshot = new Track_Oneshot;
        track_oneshot->setBarLength(barLength);
        track_oneshot->setCurrentSampleLength(barLength);
        addAndMakeVisible(track_oneshot);
        tracks.push_back(track_oneshot);
    }
    
    //std::cout << "TRACK SIZE: " << tracks.size() <<std::endl;
    
    
    for(int i = 0; i < BinaryData::namedResourceListSize; i++)
    {
        
        //std::cout << BinaryData::namedResourceList[i] << std::endl;
        
        int size = 0;
        const char* data = BinaryData::getNamedResource(BinaryData::namedResourceList[i], size);
        
        //std::cout << size << std::endl;
        
        AudioFormatReader* reader = formatManager.createReaderFor(new MemoryInputStream (data, size, false));
        
        
        if(strncmp (BinaryData::namedResourceList[i],"KIC",3) == 0)
        {
            tracks[0]->setNewSound(BinaryData::namedResourceList[i],
                                   size,
                                   reader);
            //tracks[0]->setGhostAudioActivated(true);
        }
        if(strncmp (BinaryData::namedResourceList[i],"BAS",3) == 0)
        {
            tracks[1]->setNewSound(BinaryData::namedResourceList[i],
                                   size,
                                   reader);
            
        }
        if(strncmp (BinaryData::namedResourceList[i],"HAT",3) == 0)
        {
            tracks[2]->setNewSound(BinaryData::namedResourceList[i],
                                   size,
                                   reader);
            tracks[2]->setCompressorActive(true);
            tracks[2]->compressor.set_input(Decibels::decibelsToGain(-20.0));
            tracks[2]->compressor.set_ratio(0.4);
            tracks[2]->compressor.set_attack(round(2.0 * 44.1));
            tracks[2]->compressor.set_release(round(19.8 * 44.1));
            tracks[2]->compressor.set_threshold(Decibels::decibelsToGain(-51.5));
            compressorGuis[0]->sliders[0].setValue((-20.0));
            compressorGuis[0]->sliders[1].setValue(0.4);
            compressorGuis[0]->sliders[2].setValue((2.0));
            compressorGuis[0]->sliders[3].setValue((19.8));
            compressorGuis[0]->sliders[4].setValue((-51.5));
        }
        if(strncmp (BinaryData::namedResourceList[i],"PER",3) == 0)
        {
            tracks[3]->setNewSound(BinaryData::namedResourceList[i],
                                   size,
                                   reader);
            tracks[3]->setCompressorActive(true);
            tracks[3]->compressor.set_input(Decibels::decibelsToGain(-20.0));
            tracks[3]->compressor.set_ratio(0.4);
            tracks[3]->compressor.set_attack(round(2.0 * 44.1));
            tracks[3]->compressor.set_release(round(19.8 * 44.1));
            tracks[3]->compressor.set_threshold(Decibels::decibelsToGain(-51.5));
            compressorGuis[1]->sliders[0].setValue((-20.0));
            compressorGuis[1]->sliders[1].setValue(0.4);
            compressorGuis[1]->sliders[2].setValue((2.0));
            compressorGuis[1]->sliders[3].setValue((19.8));
            compressorGuis[1]->sliders[4].setValue((-51.5));
            
        }
        if(strncmp (BinaryData::namedResourceList[i],"RER",3) == 0)
        {
            tracks[4]->setNewSound(BinaryData::namedResourceList[i],
                                   size,
                                   reader);
            tracks[4]->setCompressorActive(true);
            tracks[4]->compressor.set_input(Decibels::decibelsToGain(-20.0));
            tracks[4]->compressor.set_ratio(0.4);
            tracks[4]->compressor.set_attack(round(2.0 * 44.1));
            tracks[4]->compressor.set_release(round(19.8 * 44.1));
            tracks[4]->compressor.set_threshold(Decibels::decibelsToGain(-51.5));
            compressorGuis[2]->sliders[0].setValue((-20.0));
            compressorGuis[2]->sliders[1].setValue(0.4);
            compressorGuis[2]->sliders[2].setValue((2.0));
            compressorGuis[2]->sliders[3].setValue((19.8));
            compressorGuis[2]->sliders[4].setValue((-51.5));
        }
        if(strncmp (BinaryData::namedResourceList[i],"NOI",3) == 0)
        {
            tracks[5]->setNewSound(BinaryData::namedResourceList[i],
                                   size,
                                   reader);
            tracks[5]->setCompressorActive(true);
            tracks[5]->compressor.set_input(Decibels::decibelsToGain(-24.0));
            tracks[5]->compressor.set_ratio(0.4);
            tracks[5]->compressor.set_attack(round(2.0 * 44.1));
            tracks[5]->compressor.set_release(round(19.8 * 44.1));
            tracks[5]->compressor.set_threshold(Decibels::decibelsToGain(-45.5));
            compressorGuis[3]->sliders[0].setValue((-24.0));
            compressorGuis[3]->sliders[1].setValue(0.4);
            compressorGuis[3]->sliders[2].setValue((2.0));
            compressorGuis[3]->sliders[3].setValue((19.8));
            compressorGuis[3]->sliders[4].setValue((-45.5));
        }
        if(strncmp (BinaryData::namedResourceList[i],"PAD",3) == 0)
        {
            tracks[6]->setNewSound(BinaryData::namedResourceList[i],
                                   size,
                                   reader);
            tracks[6]->setCompressorActive(true);
            tracks[6]->compressor.set_input(Decibels::decibelsToGain(-10.0));
            tracks[6]->compressor.set_ratio(0.35);
            tracks[6]->compressor.set_attack(round(0.03 * 44.1));
            tracks[6]->compressor.set_release(round(498.0 * 44.1));
            tracks[6]->compressor.set_threshold(Decibels::decibelsToGain(-10.0));
            compressorGuis[3]->sliders[0].setValue((-24.0));
            compressorGuis[3]->sliders[1].setValue(0.35);
            compressorGuis[3]->sliders[2].setValue((0.03));
            compressorGuis[3]->sliders[3].setValue((498.0));
            compressorGuis[3]->sliders[4].setValue((-10.0));
        }
        if(strncmp (BinaryData::namedResourceList[i],"VOI",3) == 0)
        {
            tracks[7]->setNewSound(BinaryData::namedResourceList[i],
                                   size,
                                   reader);
        }
        if(strncmp (BinaryData::namedResourceList[i],"ONE",3) == 0)
        {
            tracks[8]->setNewSound(BinaryData::namedResourceList[i],
                                   size,
                                   reader);
        }
        if(strncmp (BinaryData::namedResourceList[i],"SHO",3) == 0)
        {
            tracks[9]->setNewSound(BinaryData::namedResourceList[i],
                                   size,
                                   reader);
        }
    }
    currentCompressor=0;
    
    startTimer(250);
}

Mixer::~Mixer()
{
    //std::cout << "Mixer Desctructor Called" << std::endl;
    for(auto track : tracks)
    {
        delete track;
        track = nullptr;
    }
    
    for(auto button : buttons)
    {
        delete button;
        button = nullptr;
    }
    
    for(auto gui : compressorGuis)
    {
        delete gui;
        gui = nullptr;
    }
    
    //std::cout << "Mixer Deleted" << std::endl;
}

void Mixer::paint (Graphics& g)
{
    g.fillAll(Colours::darkgrey);
    
    compressorGuis[currentCompressor]->setVisible(true);
    for(int i = 0; i<5; i++)
        if(i!=currentCompressor)
            compressorGuis[i]->setVisible(false);

    
}

void Mixer::resized()
{
    
    Rectangle<int> area(getLocalBounds());
    
    Rectangle<int> buttonArea;
    if(compressorOn)
        buttonArea = area.removeFromTop(area.getHeight()*0.666);
    else
        buttonArea = area;
    
    int buttonWidth = buttonArea.getWidth()/maxNumButtonsX;
    int buttonHeight = (buttonArea.getHeight()/maxNumButtonsY);
    
    for(auto track : tracks)
    {
        track->setButtonWidth(buttonWidth);
        track->setButtonHeight(buttonHeight);
        track->setBounds(buttonArea.removeFromLeft(buttonWidth));
    }
    
    if(compressorOn)
    {
        Rectangle<int> areaSliders(area);
        
        Rectangle<int> areaToggleButtons(areaSliders.removeFromTop(20));
        
//        editHatButton.setBounds(areaToggleButtons.removeFromLeft(areaToggleButtons.getWidth()/2));
//        editWhitButton.setBounds(areaToggleButtons);
        
        int toggle_button_width = areaToggleButtons.getWidth()/5;
        for(int i = 0; i<5; i++)
        {
            editButtons[i].setBounds(areaToggleButtons.removeFromLeft(toggle_button_width));
        }
        compressorGuis[currentCompressor]->setBounds(areaSliders);
    }
}

void Mixer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    barLength = ( (60.0/123.0) * /* number_of_beats_per_bar * */ sampleRate);
    //barLength = 21512.25;
    //std::cout << "Beat Length: " << barLength << std::endl;
    
    for(auto track : tracks)
    {
        track->prepareToPlay(samplesPerBlockExpected, sampleRate);
        track->setBarLength(barLength);
    }
}

void Mixer::releaseResources()
{
    
}

void Mixer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    midiBuffer.clear();
    bufferToFill.clearActiveBufferRegion();
    
    AudioSampleBuffer temp;
    temp.setSize(bufferToFill.buffer->getNumChannels(), bufferToFill.buffer->getNumSamples());
    temp.clear();
    
    AudioSampleBuffer kickGhostAudio;
    kickGhostAudio.setSize(bufferToFill.buffer->getNumChannels(), bufferToFill.buffer->getNumSamples());
    kickGhostAudio.clear();
    
    for(auto track : tracks)
    {
        bufferToFill.clearActiveBufferRegion();
        
        if(track->getCompressorActive())
        {
            track->setCompresstionInput(kickGhostAudio.getWritePointer(0),
                                        kickGhostAudio.getWritePointer(1));
        }
        
        track->setBarLength(barLength);
        track->getNextAudioBlock(bufferToFill);
        
        if(strncmp (track->getName(),"KICK",2) == 0)
        {
            kickGhostAudio.copyFrom(0, 0, bufferToFill.buffer->getWritePointer(0), bufferToFill.numSamples);
            kickGhostAudio.copyFrom(1, 0, bufferToFill.buffer->getWritePointer(1), bufferToFill.numSamples);
        }
        
        if(strncmp (track->getName(),"VOIC",2) == 0)
        {
            //+5
            bufferToFill.buffer->applyGain(Decibels::decibelsToGain(5.0));
        }
        
        if(strncmp (track->getName(),"ONES",2) == 0)
        {
            bufferToFill.buffer->applyGain(Decibels::decibelsToGain(-2.0));
        }
        if(strncmp (track->getName(),"SHOO",2) == 0)
        {
            bufferToFill.buffer->applyGain(Decibels::decibelsToGain(-2.0));
        }
        
        temp.addFrom(0, 0, bufferToFill.buffer->getWritePointer(0), bufferToFill.numSamples);
        temp.addFrom(1, 0, bufferToFill.buffer->getWritePointer(1), bufferToFill.numSamples);
        ////std::cout << temp.getWritePointer(0)[0] << std::endl;
    }
    
    ////std::cout << std::endl;
    bufferToFill.buffer->addFrom(0, 0, temp.getWritePointer(0), bufferToFill.numSamples);
    bufferToFill.buffer->addFrom(1, 0, temp.getWritePointer(1), bufferToFill.numSamples);
    
    bufferToFill.buffer->applyGain(Decibels::decibelsToGain(10.0));
    
//    if(bufferToFill.buffer->getRMSLevel(0, 0, bufferToFill.numSamples) > 1.0)
//    {
//        //std::cout<<"RMS LEVEL: " << bufferToFill.buffer->getRMSLevel(0, 0, bufferToFill.numSamples) <<std::endl;
//    }
}


void Mixer::buttonClicked(Button* button)
{
    for(int i = 0; i<5; i++)
    {
        if(button == &editButtons[i])
        {
            currentCompressor = i;
            
            for(int i1 = 0; i1<5; i1++)
            {
                if(i1 != i)
                {
                    editButtons[i1].setToggleState(false, dontSendNotification);
                }
            }
        }
    }
}


void Mixer::sliderValueChanged(Slider* slider)
{
    compressorGuis[currentCompressor]->sliderValueChanged(slider, tracks[currentCompressor+2]);
}
