/*
  ==============================================================================

    PlaylistComponent.h
    Created: 13 Jan 2022 12:15:23pm
    Author:  Public

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <iostream>
#include <time.h>

#include "WaveformDisplay.h"
#include "DeckGUI.h"


//==============================================================================
/*
*/
class PlaylistComponent    : public Component,
                             public TableListBoxModel,
                             public Button::Listener,
                             public TextEditor::Listener
{
public:
    PlaylistComponent(DeckGUI* deck1,
                      DeckGUI* deck2);

    ~PlaylistComponent();

    void paint (Graphics&) override;
    void resized() override;

    int getNumRows() override;

    void paintRowBackground(Graphics &, int rowNumber, int width, int height, bool rowIsSelected) override;

    void paintCell(Graphics &, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    Component *refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected , Component *existingComponentToUpdate) override;
    
    void buttonClicked(Button *) override;

    //Runs each time the textEditor text is changed
    //Used for searching
    void textEditorTextChanged (TextEditor &text) override;

    //It exports the audio file into the folder when any track is added to playlist
    void exportAudioFile(File audioFile);

    //Sets the metadata of the tracks
    void setTracks(std::vector<File> Tracks);

    //To check which deck it should load the song to
    int timesClicked=0;

private:

    TableListBox tableComponent;

    TextEditor searchBar;

    //Empty array for storing all the tracks in the folder
    Array<File> MyLibrary;

    std::string searchText;

    //Vectors for storing song files and metadata
    std::vector<std::string> trackTitles;
    std::vector<std::string> trackSize;
    std::vector<std::string> creationTime;
    std::vector<File> trackPaths;

    //Path to the Traks folder which isnt static
    //Declared here because used in various places
    File f = File(File::getCurrentWorkingDirectory().getFullPathName() + "/Tracks");
    
    TextButton addButton{"Add New Track"}; 

    //Pointer to decks
    DeckGUI* deck1;
    DeckGUI* deck2;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
