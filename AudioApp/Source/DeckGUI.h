/*
  ==============================================================================

    DeckGUI.h
    Created: 11 Jan 2022 3:25:22pm
    Author:  Public

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WaveformDisplay.h"
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/


class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener, 
                   public FileDragAndDropTarget,
                   public Timer
{
public:
    DeckGUI(DJAudioPlayer* player,
            AudioFormatManager & formatManagerToUse,
            AudioThumbnailCache & cacheToUse);
    ~DeckGUI();

    void paint (Graphics&) override;
    void resized() override;

     /** implement Button::Listener */
    void buttonClicked (Button *) override;

    /** implement Slider::Listener */
    void sliderValueChanged (Slider *slider) override;

    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override; 

    void timerCallback() override;

    //Made public so playlist can access them
    WaveformDisplay waveformDisplay;

    DJAudioPlayer* player;

    int test;

private:

    TextButton playButton{"PLAY"};
    TextButton stopButton{"STOP"};
    TextButton importButton{"IMPORT"};
    TextButton replayButton{"REPLAY"};
    TextButton prevButton{"PLAY PREVIOUS"};
    TextButton nextButton{"PLAY NEXT"};
  
    Slider volSlider; 
    Slider speedSlider;
    Slider posSlider;

    Label volLabel;
    Label speedLabel;
    Label posLabel;

     

    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
