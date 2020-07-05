/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "juce_osc/juce_osc.h"
#include "ToneExplorer.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public Component
{
public:
  //==============================================================================
  MainComponent();
  ~MainComponent();

  //==============================================================================
  void paint (Graphics&) override;
  void resized() override;

private:
  void initializeSettings();
  //==============================================================================
  // Your private member variables go here...
  ToneExplorerView toneExplorerView_;
  std::unique_ptr<ToneExplorer> toneExplorer_;
  ApplicationProperties appProperties_;
  OSCSender oscSender_;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
