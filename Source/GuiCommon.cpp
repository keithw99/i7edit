/*
  ==============================================================================

    GuiCommon.cpp
    Created: 24 May 2020 4:00:52pm
    Author:  Keith

  ==============================================================================
*/

#include "GuiCommon.h"

TextButtonGroup::TextButtonGroup(StringArray names, int groupId) : groupId_(groupId) {
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

  // Select the first button.
  buttons_.getFirst()->setToggleState(true, dontSendNotification);
  selectedButton_ = buttons_.getFirst();
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
  return selectedButton_->getButtonText();
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
