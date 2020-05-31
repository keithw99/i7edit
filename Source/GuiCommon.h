/*
  ==============================================================================

    GuiCommon.h
    Created: 24 May 2020 4:00:52pm
    Author:  Keith

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


class TextButtonGroup : public Component {
public:
  
  class Listener {
  public:
    virtual ~Listener() = default;
    virtual void selectionChanged(const int groupId, const String& selection) = 0;
  };
  
  TextButtonGroup(StringArray names, int groupId);
  
  void resized() override;
  
  // Returns the text value of the currently-selected button.
  String getSelectedValue();
  
  int getBestWidthForHeight(const int height);
  
  // Notifies listeners that the selected button has changed.
  void selectionChanged(const String& selection);
  
  void addListener(Listener* listener);
  void removeListener(Listener* listener);
  
private:
  //void setUniformWidth(int height);
  void computeLongestButtonWidth();
  
  //=========================
  OwnedArray<TextButton> buttons_;
  int groupId_;
  TextButton* widestButton_;
  TextButton* selectedButton_;
  ListenerList<Listener> listeners_;
  
#if JUCE_PROJUCER_LIVE_BUILD
public:
  TextButtonGroup() : TextButtonGroup({"A", "B"}, 1) {}
#endif
};
