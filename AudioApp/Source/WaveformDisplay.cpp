/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 11 Jan 2022 3:27:21pm
    Author:  Public

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager & formatManagerToUse,
                                 AudioThumbnailCache & cacheToUse) 
                                 : audioThumb(1000, formatManagerToUse , cacheToUse) , fileLoaded(false), position(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    audioThumb.addChangeListener(this);

}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));  // clear the background

    g.setColour (Colours::red);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    

    if(fileLoaded)
    {
      g.setColour (Colours::pink);
      //drawChannel takes 6 arguments: graphics g, size of rect, start time, end time , channel number, vertical zoom factor
      audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1.0f);
      g.setColour (Colours::red);
      g.drawRect(position * getWidth() ,0, 2 , getHeight());
    }
    else
    {
      g.setColour (Colours::pink);
      g.setFont (20.0f);
      g.drawText ("Load a file..", getLocalBounds(),
                  Justification::centred, true);   // draw some placeholder text
    }

    
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformDisplay::loadURL(URL audioURL)
{
    //clear any previous things
    audioThumb.clear();
    //URL unpacked and turned into a URLInputSource and it is set as source
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));

    if(fileLoaded)
    {
      //loaded successfully
    }
    else
    {
      std::cout<<"Not loaded"<< std::endl;
    }
};

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster *source)
{
    repaint();
};

void WaveformDisplay::setPositionRelative(double pos)
{
    if(position != pos)
    {
      position = pos;
      repaint();
    }
    

};