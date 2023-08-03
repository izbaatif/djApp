/*
  ==============================================================================

    DeckGUI.cpp
    Created: 11 Jan 2022 3:25:22pm
    Author:  Public

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player , 
                 AudioFormatManager & formatManagerToUse,
                 AudioThumbnailCache & cacheToUse) : player(_player), 
                                                     waveformDisplay(formatManagerToUse,cacheToUse)

{

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(importButton);
    addAndMakeVisible(replayButton);
    addAndMakeVisible(prevButton);
    addAndMakeVisible(nextButton);
       
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    
    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(posLabel);

    addAndMakeVisible(waveformDisplay);


    playButton.addListener(this);
    stopButton.addListener(this);
    importButton.addListener(this);
    replayButton.addListener(this);
    prevButton.addListener(this);
    nextButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    volSlider.setRange(0.0, 1.0);
    posSlider.setRange(0.0, 1.0);


    startTimer(500);

}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));  // clear the background

    g.setColour (Colours::red);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::red);
    g.setFont (14.0f);
    //g.drawText ("Volume", getLocalBounds(),
                //Justification::centred, true);   // draw some placeholder text


    //buttons properties
    playButton.setColour(TextButton::ColourIds::buttonColourId, Colours::transparentBlack);
    playButton.setColour(TextButton::ColourIds::textColourOffId, Colours::pink);

    stopButton.setColour(TextButton::ColourIds::buttonColourId, Colours::transparentBlack);
    stopButton.setColour(TextButton::ColourIds::textColourOffId, Colours::pink);

    importButton.setColour(TextButton::ColourIds::buttonColourId, Colours::transparentBlack);
    importButton.setColour(TextButton::ColourIds::textColourOffId, Colours::pink);

    replayButton.setColour(TextButton::ColourIds::buttonColourId, Colours::transparentBlack);
    replayButton.setColour(TextButton::ColourIds::textColourOffId, Colours::pink);

    prevButton.setColour(TextButton::ColourIds::buttonColourId, Colours::transparentBlack);
    prevButton.setColour(TextButton::ColourIds::textColourOffId, Colours::pink);

    nextButton.setColour(TextButton::ColourIds::buttonColourId, Colours::transparentBlack);
    nextButton.setColour(TextButton::ColourIds::textColourOffId, Colours::pink);

    //sliders properties and removing text box
    volSlider.setColour (Slider::ColourIds::backgroundColourId, Colours::pink);
    volSlider.setColour (Slider::ColourIds::textBoxTextColourId, Colours::pink);
    volSlider.setColour (Slider::ColourIds::thumbColourId, Colours::red);
    volSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    speedSlider.setColour (Slider::ColourIds::backgroundColourId, Colours::pink);
    speedSlider.setColour (Slider::ColourIds::textBoxTextColourId, Colours::pink);
    speedSlider.setColour (Slider::ColourIds::thumbColourId, Colours::red);
    speedSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    posSlider.setColour (Slider::ColourIds::backgroundColourId, Colours::pink);
    posSlider.setColour (Slider::ColourIds::textBoxTextColourId, Colours::pink);
    posSlider.setColour (Slider::ColourIds::thumbColourId, Colours::red);
    posSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    
    //Labels Properties
    volLabel.setText ("Volume:", juce::dontSendNotification);
    volLabel.setColour (juce::Label::textColourId, juce::Colours::pink);
    volLabel.setJustificationType (juce::Justification::left);

    speedLabel.setText ("Speed:", juce::dontSendNotification);
    speedLabel.setColour (juce::Label::textColourId, juce::Colours::pink);
    speedLabel.setJustificationType (juce::Justification::left);

    posLabel.setText ("Position:", juce::dontSendNotification);
    posLabel.setColour (juce::Label::textColourId, juce::Colours::pink);
    posLabel.setJustificationType (juce::Justification::left);
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 7 ; 
    playButton.setBounds(0, 0, getWidth()*0.3, rowH);
    stopButton.setBounds(0, rowH , getWidth()*0.3, rowH);
    importButton.setBounds(0, rowH * 2, getWidth()*0.3, rowH);

    waveformDisplay.setBounds(getWidth()*0.3 , 0 , getWidth()*0.7, rowH*3);

    posLabel.setBounds(getWidth()*0.1 ,rowH * 3, getWidth()*0.2 ,rowH);
    posSlider.setBounds(getWidth()*0.3, rowH * 3, getWidth()*0.7, rowH);

    volLabel.setBounds( getWidth()*0.1 ,rowH*4, getWidth()*0.2, rowH);
    volSlider.setBounds(getWidth()*0.3, rowH * 4, getWidth()*0.7, rowH);

    speedLabel.setBounds(getWidth()*0.1 , rowH*5 , getWidth()*0.2 ,rowH);
    speedSlider.setBounds(getWidth()*0.3, rowH * 5, getWidth()*0.7 , rowH);

    replayButton.setBounds(getWidth()*0.33, rowH*6 , getWidth()/3, rowH);

    prevButton.setBounds(0, rowH*6 , getWidth()/3, rowH);
    nextButton.setBounds(getWidth()*0.66, rowH*6 , getWidth()/3, rowH);
}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        player->start();
    }
     if (button == &stopButton)
    {
        player->stop();

    }
    if (button == &importButton)
    {
        //Opens popup for user to select
        //Returns true when user selects something
        //If user cancels it returns false
        FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen())
        {
            player->loadURL(URL{chooser.getResult()});
            waveformDisplay.loadURL(URL{chooser.getResult()});

            //Adds song to the vector so user can play it by pressing play previous button
            player->songsPlayed.push_back(File(chooser.getResult()));
            //Increases song number by 1
            player->songNumber += 1;
        }
    }

    if( button == &replayButton)
    {
        //Calls DJAudioPlayer replay function
        player->replay();
    };

    if( button == &prevButton)
    {
        //Calls DJAudioPlayer playPrev function
        player->playPrev();
        //Draws the waveform for the song that just loaded
        waveformDisplay.loadURL(URL{player->songsPlayed[player->songNumber]});
    };

    if( button == &nextButton)
    {
        //Calls DJAudioPlayer playNext function
        player->playNext();
        //Draws the waveform for the song that just loaded
        waveformDisplay.loadURL(URL{player->songsPlayed[player->songNumber]});
    };
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  //Allows one file
  if (files.size() == 1)
  {
    //if 1 gets URL
    player->loadURL(URL{File{files[0]}});
    waveformDisplay.loadURL(URL{ File{files[0]}});

    //Adds song to the vector so user can play it by pressing play previous button
    player->songsPlayed.push_back(File{files[0]});
    //Increases song number by 1
    player->songNumber += 1;
  }
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(player->getPositionRelative());
};
