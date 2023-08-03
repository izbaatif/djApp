/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 11 Jan 2022 2:25:45pm
    Author:  Public

  ==============================================================================
*/

#include "DJAudioPlayer.h"
#include "../JuceLibraryCode/JuceHeader.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager)
                            : formatManager(_formatManager)
{
    
};
    
DJAudioPlayer::~DJAudioPlayer()
{

};

void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
  //prepares these to play
  transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
  resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

};
   
void DJAudioPlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
  //runs audio blocks again and again
  resampleSource.getNextAudioBlock(bufferToFill);
};
    
void DJAudioPlayer::releaseResources()
{
  transportSource.releaseResources();
  resampleSource.releaseResources();
};

void DJAudioPlayer::loadURL(URL audioURL)
{
  //audio Url is converted into an input stream which is passed to format manager which creates a reader
  auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));

  if (reader != nullptr) //good file
  {
      //Takes no. out of audio file and wraps it into audio life cycle
      std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));

      transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);

      readerSource.reset(newSource.release());
  };
};
    
void DJAudioPlayer::setGain(double gain)
{
  if(gain<0.0 || gain>1.0)
  {

  }
  else
  {
    //Sets the volume according to slider value
    transportSource.setGain(gain);
  };
};
    
void DJAudioPlayer::setSpeed(double ratio)
{
  if(ratio<0.0 || ratio>10.0)
  {

  }
  else
  {
    //Resmaple by the ratio of the value from slider
    resampleSource.setResamplingRatio(ratio);
  };

};
    
void DJAudioPlayer::setPosition(double posInSecs)
{
  transportSource.setPosition(posInSecs);
};

void DJAudioPlayer::setPositionRelative(double pos)
{
  if(pos<0.0 || pos>1.0)
  {

  }
  else
  {
    double posInSecs = transportSource.getLengthInSeconds() * pos; 
    setPosition(posInSecs);
  };
};

void DJAudioPlayer::start()
{
  transportSource.start();
};
    
void DJAudioPlayer::stop()
{
  transportSource.stop();
};

void DJAudioPlayer::replay()
{
  //Sets the position at start and starts the song
  transportSource.setPosition(0.0);
  transportSource.start();
};

void DJAudioPlayer::playPrev()
{
    //Makes sure songNumber doesnt go below 0
    if(songNumber <= 0)
    {
      songNumber=0;
    }

    else
    {
        //Decreases the song number by 1 and loads the song at that position of songsPlayed vector into deck
        songNumber = songNumber-1;
        loadURL(URL{songsPlayed[songNumber]});
        //Starts the song
        transportSource.start();
        
    }
};
    
    
void DJAudioPlayer::playNext()
{
    //Makes sure songNumber doesnt go above the songsPlayed vector size
    if(songNumber == static_cast<int>(songsPlayed.size()-1))
    {
      songNumber = static_cast<int>(songsPlayed.size()-1);
    }
    else
    {
        //Increases the song number by 1 and loads the song at that position of songsPlayed vector into deck
        songNumber = songNumber+1;
        loadURL(URL{songsPlayed[songNumber]});
        //Starts the song
        transportSource.start();
    }

};

double DJAudioPlayer::getPositionRelative()
{
  return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
};



