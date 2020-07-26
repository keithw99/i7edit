/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
  initializeSettings();
  
  StringArray expansionBanks = {"SRX-05", "SRX-06", "SRX-07", "SRX-08"};
  toneExplorer_.reset(new ToneExplorer(toneExplorerView_, oscSender_, expansionBanks));
  initializeOsc();
  
  addAndMakeVisible(toneExplorerView_);
  setSize (600, 400);
}

void MainComponent::initializeSettings() {
  PropertiesFile::Options options;
  options.osxLibrarySubFolder = "Application Support";
    
  options.applicationName = ProjectInfo::projectName;
  options.filenameSuffix = ".settings";
  options.folderName = File::getSpecialLocation(
    File::SpecialLocationType::userApplicationDataDirectory)
    .getChildFile(ProjectInfo::projectName).getFullPathName();
  appProperties_.setStorageParameters(options);
}

void MainComponent::initializeOsc()
{
  if (!OSCReceiver::connect(9010)) {
    DBG ("ERROR: could not connect to port 9010.");
  }
  OSCAddress address(osc::address::ToneSelect);
  OSCReceiver::addListener(toneExplorer_.get(), address);

}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

  /*
  g.setFont (Font (16.0f));
  g.setColour (Colours::white);
  g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
  */
}

void MainComponent::resized()
{
  // This is called when the MainComponent is resized.
  // If you add any child components, this is where you should
  // update their positions.
  toneExplorerView_.setBounds(0, 0, getWidth(), getHeight());
}
