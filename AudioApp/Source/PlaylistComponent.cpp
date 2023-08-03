/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 13 Jan 2022 12:15:23pm
    Author:  Public

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

#include <string>

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _deck1,
                                     DeckGUI* _deck2)
                                     : deck1(_deck1),
                                       deck2(_deck2)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.


    //Checks if the Tracks directory exists
    if(f.exists() == false)
    {
        //Creates it if it doesnt already exist
        f.createDirectory();
    }
    
    //Stores any files in the Tracks directory to MyLibrary Array
    MyLibrary = f.findChildFiles (2, false, "*");

    //Copies the files to trackPaths vector
    for(int i=0; i< static_cast<int>(MyLibrary.size()); i++)
    {
        trackPaths.push_back(MyLibrary[i]);
    };

    //Sets the metadata
    setTracks(trackPaths);

    //Draws the table columns
    tableComponent.getHeader().addColumn("Title", 0, 160);
    tableComponent.getHeader().addColumn("Size", 1, 80);
    tableComponent.getHeader().addColumn("Created On", 2, 80);
    tableComponent.getHeader().addColumn("", 3 , 80);
    tableComponent.setModel(this);

    //Make visible
    addAndMakeVisible(addButton);
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(searchBar);

    //Set initial text of search bar
    searchBar.setText(TRANS("Search Song..."));

    //Adds listeners
    addButton.addListener(this);
    searchBar.addListener(this);

}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::red);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::transparentBlack);
    g.setFont (14.0f);
    //g.drawText ("PlaylistComponent", getLocalBounds(),
                //Justification::centred, true);   // draw some placeholder text

    //Add Track Button properties
    addButton.setColour(TextButton::ColourIds::buttonColourId, Colours::transparentBlack);
    addButton.setColour(TextButton::ColourIds::textColourOffId, Colours::pink);


    //Search Bar properties
    searchBar.setColour(TextEditor::ColourIds::textColourId , Colours::white);
    searchBar.setColour(TextEditor::ColourIds::highlightColourId, Colours::pink);
    searchBar.setColour(TextEditor::ColourIds::highlightedTextColourId, Colours::red);

}


void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    int row = getHeight()/20;
    searchBar.setBounds(0, 0 ,getWidth(),row);
    tableComponent.setBounds(0, row, getWidth(), row*17.5);
    addButton.setBounds(0,  row*18.5, getWidth(), row*1.5);

}

int PlaylistComponent::getNumRows()
{
    return trackTitles.size();
};

void PlaylistComponent::paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected)
{
    
    if(rowIsSelected)
    {
        //Properties of selected row in table
        g.fillAll(Colours::pink);
        g.setColour(Colours::red);
    }
    else
    {
        //Properties otherwise
        g.fillAll(Colours::transparentBlack);
        g.setColour (Colours::pink);
    }
};

void PlaylistComponent::paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    if(rowNumber < getNumRows())
    {
        //Displays the appropriate vectors content for each column
        if(columnId==0)
        {
            g.drawText(trackTitles[rowNumber], 2, 0, width-4, height, Justification::centredLeft, true);
        }
        if(columnId==1)
        {
            g.drawText(trackSize[rowNumber], 2 , 0, width-4, height, Justification::centredLeft, true);
        }
        if(columnId==2)
        {
            g.drawText(creationTime[rowNumber], 2 , 0, width-4, height, Justification::centredLeft, true);
        }
        
    }

    else
    {

    };
    
};

Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected , Component *existingComponentToUpdate)
{
    //Draws play buttons and adds listener to them
    if(columnId==3)
    {
        if(existingComponentToUpdate==nullptr)
        {
            TextButton *btn =  new TextButton{"Play"};
            //Gets the id of the buttons
            String id{std::to_string(rowNumber)};

            btn->setComponentID(id);

            btn->addListener(this);
            existingComponentToUpdate= btn;
            
        }
    }
    return existingComponentToUpdate;
};

void PlaylistComponent::buttonClicked(Button* button)
{
    if (button == &addButton)
    {
        //Allows user to select file
        FileChooser chooser{ "Select a file..." };
        if (chooser.browseForFileToOpen())
        {
            //Adds the file to trackPath vector
            trackPaths.push_back(File(chooser.getResult())); 

            //Makes a copy of it to the Tracks folder
            exportAudioFile(trackPaths[trackPaths.size()-1]);

            //Resets the metadata
            setTracks(trackPaths);

            //Updates the table row to add the new song
            tableComponent.updateContent();
        }
        
        
    }

    else
    {
        //Gets the id of the button clicked
        int id= std::stoi(button->getComponentID().toStdString());

        //Increases by one on each play button click
        timesClicked+=1;

        //If the times the button is clicked is odd, it loads it to deck 1
        if(timesClicked%2==1)
        {
            //Loads the song URL into deck 1
            deck1->player->loadURL(URL(File(trackPaths[id])));
            //Plays the song in deck 1
            deck1->player->start();

            //Adds song to the vector so user can play it by pressing play previous button
            deck1->player->songsPlayed.push_back(File(trackPaths[id]));
            //Increases the song number by 1
            deck1->player->songNumber += 1;
        
            //Gets the wavformDisplay of Deck 1 and loads waveform to it
            deck1->waveformDisplay.loadURL(URL(File(trackPaths[id])));
        }

        //If its even loads if to deck 2
        else
        {
            //Loads the song URL into the deck 2
            deck2->player->loadURL(URL(File(trackPaths[id])));
            //Plays the song in deck 2
            deck2->player->start();

            //Adds song to the vector so user can play it by pressing play previous button
            deck2->player->songsPlayed.push_back(File(trackPaths[id]));
            //Increases the song number by 1
            deck2->player->songNumber += 1;
        
            //Gets the wavformDisplay of Deck 2 and loads waveform to it
            deck2->waveformDisplay.loadURL(URL(File(trackPaths[id])));
        }
        
        
        
    };
};

void PlaylistComponent::textEditorTextChanged(TextEditor& text)
{
    //Gets the text the user entered into the textEditor
    //Changes each time the text changes
    searchText = searchBar.getText().toStdString();
    
    //Searches for files in the directory f which include the searchText
    Array<File>FilesSearched = f.findChildFiles(2,false,"*" + searchText + "*");

    //Clears the trackPaths vector so new files can be stored
    trackPaths.clear();

    //Pushes the array files into the vector
    for(int i=0; i< static_cast<int>(FilesSearched.size()); i++)
    {
        trackPaths.push_back(FilesSearched[i]);
    };

    //Resets all the tracks metadata
    setTracks(trackPaths);
    
    //Updates the table rows
    tableComponent.updateContent();
    
    
};

void PlaylistComponent::exportAudioFile(File audioFile)
{
    //Gets the name of the file
    std::string nameOfFile = audioFile.getFileName().toStdString();

    //Creates a new file by that name in the Tracks directory
    File newFile = File(File::getCurrentWorkingDirectory().getFullPathName() + "/Tracks/" + nameOfFile);
    newFile.create();

    //Copies the file into that new empty file
    audioFile.copyFileTo(newFile);
}

void PlaylistComponent::setTracks(std::vector<File> Tracks)
{
    //Clear all first 
    trackTitles.clear();
    trackSize.clear();
    creationTime.clear();
    
    //Loops through the Track vector and resets all of these
    for(int i=0; i<Tracks.size(); i++)
    {
        //Gets the name of the file and turns it to string and adds it to trackTitles vector
        std::string fileName = File(Tracks[i]).getFileName().toStdString();
        trackTitles.push_back(fileName); 

        //Gets the file size in string and adds it to fileSize vector
        std::string fileSize = File::descriptionOfSizeInBytes(Tracks[i].getSize()).toStdString();
        trackSize.push_back(fileSize);

        //Gets the creationTime of file in string and adds it to creationTime vector
        std::string CreateTime = Tracks[i].getCreationTime().toString(true,true,false,false).toStdString();
        creationTime.push_back(CreateTime);
    }


}




