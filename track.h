/*
 ==============================================================================
 
 Track.h
 Created: 11 Apr 2017 5:25:06pm
 Author:  Samuel Thompson Parke-Wolfe
 
 ==============================================================================
 */

#ifndef Track_H_INCLUDED
#define Track_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include "LookAndFeels.h"
#include "Compressor.h"

//0.99997
static std::vector<float> sine_wave = {1.0,0.99988,0.99966,0.99927,0.99875,0.99808,0.99725,0.99628,0.99515,0.99387,0.99243,0.99084,0.98914,0.98724,0.98523,0.98306,0.98074,0.97827,0.97565,0.9729,0.97,0.96692,0.96375,0.96039,0.95691,0.95325,0.94949,0.94556,0.9415,0.93729,0.93295,0.92847,0.92383,0.91907,0.91415,0.90912,0.90396,0.89862,0.89319,0.8876,0.88187,0.87604,0.87006,0.86392,0.8577,0.85132,0.84482,0.8382,0.83142,0.82455,0.81754,0.81042,0.80316,0.79581,0.7883,0.7807,0.77298,0.76514,0.75717,0.74908,0.74091,0.7326,0.72421,0.7157,0.70706,0.69833,0.68951,0.68057,0.67151,0.66238,0.65314,0.6438,0.63437,0.62482,0.6152,0.60547,0.59567,0.58575,0.57578,0.5657,0.55554,0.54529,0.53497,0.52457,0.51407,0.50351,0.49286,0.48215,0.47137,0.46051,0.44958,0.4386,0.42752,0.41641,0.40521,0.39395,0.38266,0.37128,0.35986,0.34839,0.33685,0.32529,0.31366,0.30197,0.29025,0.2785,0.26669,0.25485,0.24295,0.23105,0.21909,0.20709,0.19507,0.18301,0.17093,0.15884,0.1467,0.13455,0.12241,0.1102,0.097992,0.085785,0.073547,0.06131,0.049042,0.036804,0.024536,0.012268,0,-0.012268,-0.024536,-0.036804,-0.049042,-0.06131,-0.073547,-0.085785,-0.097992,-0.1102,-0.12241,-0.13455,-0.1467,-0.15884,-0.17093,-0.18301,-0.19507,-0.20709,-0.21909,-0.23105,-0.24295,-0.25485,-0.26669,-0.2785,-0.29025,-0.30197,-0.31366,-0.32529,-0.33685,-0.34839,-0.35986,-0.37128,-0.38266,-0.39395,-0.40521,-0.41641,-0.42752,-0.4386,-0.44958,-0.46051,-0.47137,-0.48215,-0.49286,-0.50351,-0.51407,-0.52457,-0.53497,-0.54529,-0.55554,-0.5657,-0.57578,-0.58575,-0.59567,-0.60547,-0.6152,-0.62482,-0.63437,-0.6438,-0.65314,-0.66238,-0.67151,-0.68057,-0.68951,-0.69833,-0.70706,-0.7157,-0.72421,-0.7326,-0.74091,-0.74908,-0.75717,-0.76514,-0.77298,-0.7807,-0.7883,-0.79581,-0.80316,-0.81042,-0.81754,-0.82455,-0.83142,-0.8382,-0.84482,-0.85132,-0.8577,-0.86392,-0.87006,-0.87604,-0.88187,-0.8876,-0.89319,-0.89862,-0.90396,-0.90912,-0.91415,-0.91907,-0.92383,-0.92847,-0.93295,-0.93729,-0.9415,-0.94556,-0.94949,-0.95325,-0.95691,-0.96039,-0.96375,-0.96692,-0.97,-0.9729,-0.97565,-0.97827,-0.98074,-0.98306,-0.98523,-0.98724,-0.98914,-0.99084,-0.99243,-0.99387,-0.99515,-0.99628,-0.99725,-0.99808,-0.99875,-0.99927,-0.99966,-0.99988,-1.0};


struct MyMidiMessage
{
public:
    bool on;
    int offset;
    int number;
};

class MySynth
{
public:
    
    MySynth()
    {
        
    }
    
    int playhead = 0;
    int activeBuffer = -1;
    
    
    std::vector<MyMidiMessage> midimessages;
    std::vector<AudioSampleBuffer> buffers;
    
    
    void addBuffer(juce::AudioFormatReader *reader)
    {
        AudioSampleBuffer buffer;
        buffer.setSize(reader->numChannels, reader->lengthInSamples);
        
        reader->read(&buffer, 0, reader->lengthInSamples, 0, true, true);
        
        //std::cout <<"SIZE WAVE SIZE: " << sine_wave.size() << std::endl;
        
        for(int i = 0; i<buffer.getNumSamples(); i++)
        {
            float** output = buffer.getArrayOfWritePointers();
            if(i < sine_wave.size())
            {
                
                output[0][i] = output[0][i] * (1.0 -((sine_wave[i] + 1.0) * 0.5));
                output[1][i] = output[1][i] * (1.0 -((sine_wave[i] + 1.0) * 0.5));

                
//                //std::cout << i << ": " << 1.0 - ((sine_wave[i] + 1.0) * 0.5) << std::endl;
                
            }
            
            if(i >= buffer.getNumSamples()-sine_wave.size())
            {
                float it_i = i - (buffer.getNumSamples()-sine_wave.size());
                
                output[0][i] = output[0][i] * ((sine_wave[it_i] + 1.0) * 0.5);
                output[1][i] = output[1][i] * ((sine_wave[it_i] + 1.0) * 0.5);
                
//                //std::cout << it_i << ": " << ((sine_wave[it_i] + 1.0) * 0.5) << std::endl;
            }
        }
        
        buffers.push_back(buffer);
        
//        for(auto bufferwha : buffers)
//            //std::cout << bufferwha.getNumSamples() << " " << bufferwha.getWritePointer(0)[bufferwha.getNumSamples()-1] << std::endl;
    }
    
    
    void renderNextBlock(AudioBuffer<float> &outputAudio, const juce::MidiBuffer &inputMidi, int startSample, int numSamples)
    {
        outputAudio.clear();
        midimessages.clear();
        
        float** output = outputAudio.getArrayOfWritePointers();
        
        for(int i = 0; i< inputMidi.getNumEvents(); i++)
        {
            int it_i = i * 9;
            
            MyMidiMessage mymidimessage;
            
            mymidimessage.offset = int(inputMidi.data[it_i+0]) + int((inputMidi.data[it_i+1]) * 256);
            
            if(inputMidi.data[it_i+6] == 128)
                mymidimessage.on = false;
            if(inputMidi.data[it_i+6] == 144)
                mymidimessage.on = true;
            
            mymidimessage.number = inputMidi.data[it_i+7];
            
            midimessages.push_back(mymidimessage);
            
            //std::cout << "MIDI MESSAGE: " << " Buffer Offset: " << mymidimessage.offset << " On: " << mymidimessage.on << " Number: " << mymidimessage.number << " Midi Messages Size: " << midimessages.size() << std::endl;
            
        }

        
        for(int i = 0; i< numSamples; i++)
        {
            for(auto mymidimessage : midimessages)
            {
                bool AllMidiMessagesAreOff = true;
                
                if(i == mymidimessage.offset)
                {
                    if(mymidimessage.on)
                    {
                        playhead = 0;
                        activeBuffer = mymidimessage.number;
                        AllMidiMessagesAreOff = false;
                    }
                    
                    if(AllMidiMessagesAreOff)
                    {
                        playhead = 0;
                        activeBuffer = -1;
                    }
                    
                    //std::cout << "i is at midi messages for: "<<mymidimessage.number<< " offset: " << mymidimessage.offset << " Playhead: " << playhead << " Active Buffer: " << activeBuffer << std::endl;

                }
            }
            
            if(activeBuffer != -1)
            {
                if(playhead < buffers[activeBuffer].getNumSamples())
                {
                    output[0][i] = buffers[activeBuffer].getWritePointer(0)[playhead];
                    output[1][i] = buffers[activeBuffer].getWritePointer(1)[playhead];

                    playhead++;
                    //playhead%=buffers[activeBuffer].getNumSamples();
                }
                else
                {
                    activeBuffer = -1;
                }
            }
        }
        
        if(inputMidi.getNumEvents())
        {
            for(int i = 0; i<5; i++)
            {
                //std::cout << midimessages[0].offset + (i-2) << ": " << output[0][midimessages[0].offset + (i-2)] << std::endl;
            }
        }
    }
};

//==============================================================================
/*
 */
class Track
:
public Component,
public AudioSource,
public Button::Listener

{
protected:
    
    LookAndFeel_Mixer lookAndFeel_Mixer;
    
    int midi_note, ghost_midi_note;
    std::vector<ToggleButton*> buttons;
    std::vector<double> sample_lengths;
    
    Synthesiser synth, ghostSynth;
    
    MySynth mysynth;
    
    bool GhostAudioActivated;
    AudioSampleBuffer ghostAudioOutput;
    
    int buttonWidth, buttonHeight;
    
    double bar_time, sample_time;
    double barLength, currentSampleLength;
    bool playing, noButtonPressed, firstTimePlayed;
    int previousButton;
    
    int it_i = 0;
    
    MidiBuffer midi;
    
    char name[4];
    
    float *sideChainInputL, *sideChainInputR;
    bool compressorActive;
    
public:
    Compressor compressor;
    
public:
    Track();
    ~Track();
    
    void paint (Graphics& g) override;
    void resized() override;
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override{}
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    
    void buttonClicked(Button* button) override;
    
    virtual void setNewSound(const char* _name, int& sample_length, AudioFormatReader* reader);
    void setNewGhostSound(const char* _name, int& sample_length, AudioFormatReader* reader);
    
    void setCompresstionInput(float* left, float*right);
    
    void setCompressorActive(bool b)    {compressorActive=b;}
    void setButtonWidth(int& i)         {buttonWidth=i;}
    void setButtonHeight(int& i)        {buttonHeight=i;}
    void setBarLength(double& f)           {barLength=f;}
    void setCurrentSampleLength(double& f) {currentSampleLength=f;}
    void setGhostAudioActivated(bool b) {GhostAudioActivated=b;}
    
    float LinearToDecibel(float linear)
    {
        float db;
        if (linear != 0.0f)
            db = 20.0f * log10(linear);
        else
            db = -144.0f;  // effectively minus infinity
        return db;
    }
    
    AudioSampleBuffer& getGhostAudioOutput() {return ghostAudioOutput;}
    char* getName() {return name;}
    bool getCompressorActive(){return compressorActive;}
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Track)
};


class Track_Oneshot
:
public Track
{
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
    {
        midi.clear();
        
        noButtonPressed = true;
        
        for(int sample_it = 0; sample_it < bufferToFill.numSamples; sample_it ++)
        {
            if(bar_time > barLength)
            {
                bar_time = fmod(bar_time, barLength);
                if(sample_time > currentSampleLength)
                {
                    sample_time = fmod(sample_time, currentSampleLength);

                    for(int i = 0; i<buttons.size(); i++)
                    {
                        if(buttons[i]->getToggleState())
                        {
                            ////std::cout <<"Button is pressed"<<std::endl;
                            noButtonPressed = false;
                            
                            ////std::cout << name << ": " << "Previous Button: " << previousButton << std::endl;
                            
                            if(i == previousButton)
                            {
                                ////std::cout << "Previous Button was the same as the current     button so turn it off" << std::endl;
                                midi.addEvent(MidiMessage::noteOff(1, i, float(1.0)), (sample_it));
                                currentSampleLength = barLength;
                                previousButton = -1;
                                MessageManagerLock mmlock;
                                buttons[i]->setToggleState(false, dontSendNotification);
                            }
                            else
                            {
                                if(previousButton != -1)
                                {
                                    ////std::cout << "Previous Button was not nothing so turn the old button off" << std::endl;
                                    midi.addEvent(MidiMessage::noteOff(1, (previousButton), float(1.0)), (sample_it));
                                }
                            
                                ////std::cout << "Current Button is different from previous button so add a midi event" <<std::endl;
                            
                                midi.addEvent(MidiMessage::noteOn(1, i, float(1.0)), (sample_it));
                            
                                currentSampleLength = sample_lengths[(i)];
                            
                                //std::cout << name << ": " << "Playing Sample: " << i << " Oneshot" << std::endl;
                            
                                previousButton = i;
                            ////std::cout << "Previous Button Is Now: " << previousButton <<std::endl;
                            }
                        }
                    }
                
                    if(noButtonPressed)
                    {
                        ////std::cout << "No Buttons Were Pressed" <<std::endl;
                        if(previousButton != -1)
                        {
                            ////std::cout << "The Previous Button was something so turn it off" <<std::endl;
                            midi.addEvent(MidiMessage::noteOff(1, (previousButton), float(1.0)), (sample_it));
                        }
                        currentSampleLength = barLength;
                        previousButton = -1;
                    }
                }
            }
            bar_time++;
            sample_time++;
        }
        
        mysynth.renderNextBlock(*bufferToFill.buffer, midi, 0, bufferToFill.numSamples);
        
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
    
    
    void setNewSound(const char* _name, int& sample_length, AudioFormatReader* reader)
    {
        if(reader != nullptr)
        {
            //std::cout << "MidiNote: " << midi_note << std::endl;
            
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
            
            //std::cout.precision(17);
            //std::cout << "Old Length: " << std::fixed << length << " New Length: " << std::fixed << newlength << " Diff : " << std::fixed << diff << std::endl;
            
            sample_lengths.push_back(newlength);
            
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
            
            //std::cout << "Name: " << name << std::endl;
            
            ToggleButton* togglebutton = new ToggleButton;
            togglebutton->setButtonText(_name);
            togglebutton->setToggleState(false, dontSendNotification);
            addAndMakeVisible(togglebutton);
            togglebutton->addListener(this);
            buttons.push_back(togglebutton);
            
            //std::cout << "BUTTON CREATED" <<std::endl;
            
            delete reader;
        }
    }
};


#endif  // Track_H_INCLUDED
