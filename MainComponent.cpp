/*
 ==============================================================================
 
 This file was auto-generated!
 
 ==============================================================================
 */

#include "MainComponent.h"

MainContentComponent::MainContentComponent()
{
    addAndMakeVisible(&mixer);
    
    //setSize (640/2, 1136/2);
    int size = 654;
    setSize (size, size);
    setAudioChannels (0, 2);
}

MainContentComponent::~MainContentComponent()
{
    //std::cout << "MainContentComponent Desctructor Called" << std::endl;
    shutdownAudio();
    //std::cout << "MainContentComponent Deleted" << std::endl;
}

void MainContentComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    mixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainContentComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    
    mixer.getNextAudioBlock(bufferToFill);
}

void MainContentComponent::releaseResources()
{
    mixer.releaseResources();
}

void MainContentComponent::paint (Graphics& g)
{
    
}

void MainContentComponent::resized()
{
    mixer.setBounds(getLocalBounds());
}
