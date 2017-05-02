/*
  ==============================================================================

    Track.cpp
    Created: 11 Apr 2017 5:25:06pm
    Author:  Samuel Thompson Parke-Wolfe

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Track.h"

//==============================================================================
Track::Track()
:
GhostAudioActivated(false),
bar_time(0),
sample_time(0),
firstTimePlayed(true),
previousButton(-1),
compressorActive(false)
{
    midi_note = 0;
    ghost_midi_note = 0;
    
    setLookAndFeel(&lookAndFeel_Mixer);
}

Track::~Track()
{
//    std::cout << "Track Desctructor Called" << std::endl;
    for(auto button : buttons)
    {
        delete button;
        button = nullptr;
    }
//    std::cout << "Track Deleted" << std::endl;
}

void Track::paint (Graphics& g)
{

}

void Track::resized()
{
    Rectangle<int> area(getLocalBounds());
    for(auto button : buttons)
    {
        //area.removeFromTop(gap);
        button->setBounds(area.removeFromTop(buttonHeight));
        //area.removeFromTop(gap);
    }
}

void Track::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    synth.setMinimumRenderingSubdivisionSize(samplesPerBlockExpected);
    synth.setCurrentPlaybackSampleRate(sampleRate);
    ghostSynth.setMinimumRenderingSubdivisionSize(samplesPerBlockExpected);
    ghostSynth.setCurrentPlaybackSampleRate(sampleRate);
    
    ghostAudioOutput.setSize(2, samplesPerBlockExpected);
}

void Track::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    midi.clear();
    
    noButtonPressed = true;
    
    //Keep
    bool looping = false;
    
    for(int sample_it = 0; sample_it < bufferToFill.numSamples; sample_it ++)
    {
        if(bar_time > barLength)
        {
            bar_time = fmod(bar_time, barLength);
            if(sample_time > currentSampleLength)
            {
                sample_time = fmod(sample_time, currentSampleLength);

                looping = true;
                for(int button_it= 0;button_it<buttons.size();button_it++)
                {
                    if(buttons[button_it]->getToggleState())
                    {
                        noButtonPressed = false;
                        
//                        std::cout << name << ": " << "Previous Button: " << previousButton << std::endl;
                        
                        if(button_it== previousButton)
                        {
                            if(firstTimePlayed)
                            {
//                                std::cout << name << ": " << "Playing Sample: " <<button_it*3 << " Intro2" << std::endl;
                                
                                midi.addEvent(MidiMessage::noteOn(1,button_it*3, float(1.0)),(sample_it));
                                
                                currentSampleLength = sample_lengths[(button_it*3)];
                                
                                firstTimePlayed = false;
                            }
                            else
                            {
//                                std::cout << name << ": " << "Playing Sample: " <<button_it*3 << " Loop" << std::endl;
                                
                                midi.addEvent(MidiMessage::noteOff(1, (button_it*3)+1, float(1.0)),(sample_it));
                                midi.addEvent(MidiMessage::noteOn(1, (button_it*3)+1, float(1.0)),(sample_it));
                                currentSampleLength = sample_lengths[(button_it*3)+1];
                            }
                        }
                        else
                        {
                            //If the previous button was another button
                            if(previousButton != -1)
                            {
                                //Play the outro of the last button
//                                std::cout << name << ": " << "Playing Button: " << previousButton << " Outro1" << std::endl;
                                
                                //Turn off intro and loop just incase
                                midi.addEvent(MidiMessage::noteOff(1, (previousButton*3), float(1.0)),(sample_it));
                                midi.addEvent(MidiMessage::noteOff(1, (previousButton*3)+1, float(1.0)),(sample_it));
                                
                                midi.addEvent(MidiMessage::noteOn(1, (previousButton*3)+2, float(1.0)),(sample_it));
                                currentSampleLength = sample_lengths[(previousButton*3)+2];
                                
                                firstTimePlayed = true;
                            }
                            else
                            {
//                                std::cout << name << ": " << "Playing Sample: " <<button_it*3 << " Intro1" << std::endl;
                                
                                midi.addEvent(MidiMessage::noteOn(1,button_it*3, float(1.0)),(sample_it));
                                
                                currentSampleLength = sample_lengths[(button_it*3)];
                                
                                firstTimePlayed = false;
                            }
                        }
                        previousButton =button_it;
                    }
                }
                
                if(noButtonPressed)
                {
                    if(previousButton != -1)
                    {
//                        std::cout << name << ": " << "Playing Button: " << " Outro2" << std::endl;
                        
                        //Turn off intro and loop just incase
                        midi.addEvent(MidiMessage::noteOff(1, (previousButton*3), float(1.0)),(sample_it));
                        midi.addEvent(MidiMessage::noteOff(1, (previousButton*3)+1, float(1.0)),(sample_it));
                        
                        midi.addEvent(MidiMessage::noteOn(1, (previousButton*3)+2, float(1.0)),(sample_it));
                        
                        currentSampleLength = sample_lengths[(previousButton*3)+2];
                    }
                    else
                    {
                        currentSampleLength = barLength;
                    }
                    previousButton = -1;
                }
            }
        }
        bar_time++;
        sample_time++;
    }
    
//    if(strncmp (name,"BASS",2) == 0)
    mysynth.renderNextBlock(*bufferToFill.buffer, midi, 0, bufferToFill.numSamples);

    /*
    if(GhostAudioActivated)
    {
        ghostAudioOutput.clear();
        ghostSynth.renderNextBlock(ghostAudioOutput, midi, 0, bufferToFill.numSamples);
        ghostAudioOutput.applyGain(Decibels::decibelsToGain(50.0));
    }
    */
    
    //Add Compression
    if(compressorActive)
    {
        compressor.process(sideChainInputL,
                           sideChainInputR,
                           bufferToFill.buffer->getWritePointer(0),
                           bufferToFill.buffer->getWritePointer(1),
                           bufferToFill.numSamples,
                           true);
    }
}

void Track::buttonClicked(Button* button)
{
    for(auto mybutton : buttons)
        if(mybutton->getToggleState())
            if(mybutton != button)
                mybutton->setToggleState(false, dontSendNotification);
}


void Track::setNewSound(const char* _name, int& sample_length, juce::AudioFormatReader *reader)
{
    if(reader != nullptr)
    {
//        std::cout << "MidiNote: " << midi_note << std::endl;
        
        double length = round(reader->lengthInSamples);
        double newlength = length;
        
        double modlength = std::fmod(length, barLength);
        double diff = 0.0;
        
        if(modlength < barLength/2.0)
        {
            diff = modlength;
            newlength -= diff;
        }
        else
        {
            diff = barLength - modlength;
            newlength += diff;
        }
        
//        std::cout.precision(17);
//        std::cout << "Old Length: " << std::fixed << length << " New Length: " << std::fixed << newlength << " Diff : " << std::fixed << diff << std::endl;
        
        sample_lengths.push_back(newlength);
        //sample_lengths.push_back(length);
        
        BigInteger midi_notes;
        midi_notes.setRange(midi_note, 1, true);
        
        /*
        synth.addSound(new SamplerSound(_name,
                                        *reader,
                                        midi_notes,
                                        midi_note,   // root midi note
                                        0.0,  // attack time
                                        0.0,  // release time
                                        60.0*3.0));  // maximum sample length
        
        synth.addVoice (new SamplerVoice());
         
        */
        
        midi_note++;
        
        name[0] = _name[0];
        name[1] = _name[1];
        name[2] = _name[2];
        name[3] = _name[3];

        mysynth.addBuffer(reader);
        
//        std::cout << "Name: " << name << std::endl;
        
        if( strncmp(&_name[6],"LOOP",4) == 0)
        {
//            std::cout << &_name[6] << std::endl;
            ToggleButton* togglebutton = new ToggleButton;
            togglebutton->setButtonText(_name);
            togglebutton->setToggleState(false, dontSendNotification);
            addAndMakeVisible(togglebutton);
            togglebutton->addListener(this);
            buttons.push_back(togglebutton);
            
//            std::cout << "BUTTON CREATED" <<std::endl;
        }
        
        delete reader;
    }
}

void Track::setNewGhostSound(const char* _name, int& sample_length, juce::AudioFormatReader *reader)
{
    if(reader != nullptr)
    {
//        std::cout << "GhostMidiNote: " << ghost_midi_note << std::endl;
        
        BigInteger ghost_midi_notes;
        ghost_midi_notes.setRange(ghost_midi_note, 1, true);
        
        ghostSynth.addSound(new SamplerSound(_name,
                                             *reader,
                                             ghost_midi_notes,
                                             ghost_midi_note,   // root midi note
                                             0.0,  // attack time
                                             0.0,  // release time
                                             60.0*3.0));  // maximum sample length
        
        ghostSynth.addVoice (new SamplerVoice());
        ghost_midi_note++;
        
        delete reader;
    }
}

//Needs to be called directly before get next audio block.
void Track::setCompresstionInput(float* left, float*right)
{
    sideChainInputL = left;
    sideChainInputR = right;
}


