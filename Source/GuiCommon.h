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
  
  TextButtonGroup(StringArray names, int groupId, const String& selectedValue = "");
  
  void resized() override;
  
  // Returns the text value of the currently-selected button.
  String getSelectedValue();
  
  // Gets the TextButton with the given text value.
  TextButton* getButtonWithText(const String& value);
  
  // Sets the button with the given text value as selected.
  void setSelectedValue(const String& value, bool sendChangeMessage = true);
  
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

struct DragListener : public MouseListener
{
  DragListener(Viewport* target) : target_(target) {}
  
  ~DragListener() override
  {
      //viewport.contentHolder.removeMouseListener (this);
      Desktop::getInstance().removeGlobalMouseListener (this);
  }

  void mouseDown(const MouseEvent& event) override
  {
    //
  }
  
  void mouseUp(const MouseEvent& event) override
  {
    if (target_->isCurrentlyScrollingOnDrag()) {
      //
    }
  }
  
  // Member variables.
  Viewport* target_;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DragListener)
};


class ScrollingTabBar : public Viewport
{
public:
  ScrollingTabBar();
  TabbedButtonBar* getTabs() { return tabs_.get(); }
  void setTabs(const StringArray& tabNames);
  void addListener(ChangeListener* listener);
  void removeListener(ChangeListener* listener);
  const String getSelectedTabName();
  
 // Overrides Viewport.
 // void mouseUp(const MouseEvent &event) override;
 //void componentMovedOrResized(Component& , bool wasMoved, bool wasResized) override;
  
private:
  std::unique_ptr<TabbedButtonBar> tabs_;
  
  //struct DragListener;
  std::unique_ptr<DragListener> dragListener_;
};
