/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 11 Jan 2022 2:25:45pm
    Author:  Public

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"
#include <vector>

class DJAudioPlayer : public AudioSource

{
    
  public:

    DJAudioPlayer(AudioFormatManager& _formatManager);

    ~DJAudioPlayer();

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);

    void start();
    void stop();

    //Replays the song
    void replay();

    //Allows previous songs that were played 
    void playPrev();
    void playNext();

    //Stores the current position of the song thats being played
    int songNumber = -1;

    //Stores all the songs that the user has played
    std::vector<juce::File> songsPlayed;
    
    //gets relative position for playhead
    double getPositionRelative();

  private:
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;

    //& gives transport source address
    //For setting speed
    ResamplingAudioSource resampleSource{ &transportSource, false, 2 };


};