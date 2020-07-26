/*
  ==============================================================================

    GuiCommon.cpp
    Created: 24 May 2020 4:00:52pm
    Author:  Keith

  ==============================================================================
*/

#include "GuiCommon.h"

TextButtonGroup::TextButtonGroup(StringArray names, int groupId, const String& selectedValue) : groupId_(groupId) {
  int maxWidth = 0;
  for (String& name : names) {
    auto* b = buttons_.add(new TextButton(name));
    b->setRadioGroupId(groupId);
    b->setClickingTogglesState(true);
    b->setConnectedEdges(Button::ConnectedOnRight + Button::ConnectedOnLeft);
    addAndMakeVisible(b);
    
    b->onStateChange = [this, b] {
      if (b->getToggleState()) {
        selectedButton_ = b;
        selectionChanged(b->getButtonText());
      }
    };
    
    int candidateWidth = b->getBestWidthForHeight(30);
    if (candidateWidth > maxWidth) {
      widestButton_ = b;
      maxWidth = candidateWidth;
    }
  }
  
  buttons_.getFirst()->setConnectedEdges(Button::ConnectedOnRight);
  buttons_.getLast()->setConnectedEdges(Button::ConnectedOnLeft);

  // Select the default value.
  TextButton* defaultSelection = getButtonWithText(selectedValue);
  selectedButton_ = (defaultSelection != nullptr) ? defaultSelection : buttons_.getFirst();
  selectedButton_->setToggleState(true, dontSendNotification);
  //buttons_.getFirst()->setToggleState(true, dontSendNotification);
  //selectedButton_ = buttons_.getFirst();
}

void TextButtonGroup::resized() {
  auto area = getLocalBounds();
  
  int totalWidth = getBestWidthForHeight(area.getHeight());
  
  int buttonWidth = widestButton_->getBestWidthForHeight(area.getHeight());
  if(area.getWidth() < totalWidth) {
    float scaleFactor = (float)area.getWidth() / (float)totalWidth;
    buttonWidth = std::round((float)buttonWidth * scaleFactor);
  }

  for (auto* b : buttons_) {
    b->setBounds(area.removeFromLeft(buttonWidth));
  }
}

String TextButtonGroup::getSelectedValue() {
  if (selectedButton_ == nullptr) {
    return String();  // empty.
  }
  return selectedButton_->getButtonText();
}

TextButton* TextButtonGroup::getButtonWithText(const String& value)
{
  for (TextButton* b : buttons_) {
    if (b->getButtonText() == value) {
      return b;
    }
  }
  return nullptr;
}

void TextButtonGroup::setSelectedValue(const String& value, bool sendChangeMessage)
{
  TextButton* buttonToSelect = getButtonWithText(value);
  
  if (buttonToSelect == nullptr)
    return;

  // If button is the same but sendChangeMessage is specified, force a change message.
  bool buttonChanged = buttonToSelect != selectedButton_;
  if (!buttonChanged && sendChangeMessage) {
    selectionChanged(selectedButton_->getButtonText());
    return;
  }
  
  // Set the selected button.
  selectedButton_ = buttonToSelect;
  selectedButton_->setToggleState(true, sendNotification);
}

int TextButtonGroup::getBestWidthForHeight(const int height) {
  if (widestButton_ == nullptr) {
    return 0;
  }
  return widestButton_->getBestWidthForHeight(height) * buttons_.size();
}

void TextButtonGroup::selectionChanged(const String& selection) {
  Component::BailOutChecker checker(this);
  listeners_.callChecked(checker, [this, selection] (Listener& l) { l.selectionChanged(groupId_, selection); });
}

void TextButtonGroup::addListener(Listener* listener) {
  listeners_.add(listener);
}

void TextButtonGroup::removeListener(Listener* listener) {
  listeners_.remove(listener);
}


ScrollingTabBar::ScrollingTabBar()
{
  tabs_.reset(new TabbedButtonBar(juce::TabbedButtonBar::TabsAtTop));
  
  setViewedComponent(tabs_.get(), false);
  setScrollOnDragEnabled(true);
  setScrollBarsShown(false, false, false, true);
  //tabs_->setViewportIgnoreDragFlag(true);
  //addAndMakeVisible(tabs_.get());
  
  //dragListener_.reset(new DragListener(this));
  //addMouseListener(dragListener_.get(), true);
}

void ScrollingTabBar::setTabs(const StringArray& tabNames)
{
  tabs_->clearTabs();
  auto bgColor = getLookAndFeel().findColour(ResizableWindow::backgroundColourId);
  int height = 30;
  int width = 0;
  int tabIndex = 0;
  for (const String& name : tabNames) {
    if (!name.isEmpty()) {
      tabs_->addTab(name, bgColor, tabIndex);
      width += tabs_->getTabButton(tabIndex)->getBestTabLength(height);
      tabIndex++;
    }
  }
  tabs_->setSize(width, height);
}

void ScrollingTabBar::addListener(ChangeListener* listener)
{
  tabs_->addChangeListener(listener);
}

void ScrollingTabBar::removeListener(ChangeListener* listener)
{
  tabs_->removeChangeListener(listener);
}

const String ScrollingTabBar::getSelectedTabName()
{
  return tabs_->getCurrentTabName();
}

/*
void ScrollingTabBar::mouseUp(const MouseEvent &event)
{
  Viewport::mouseUp(event);
}
*/

/*
void ScrollingTabBar::componentMovedOrResized(Component &, bool wasMoved, bool wasResized)
{
  if (!wasMoved)
    return;
}
*/


