/*
  ==============================================================================

    ToneExplorer.cpp
    Created: 23 May 2020 12:46:29am
    Author:  Keith

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ToneExplorer.h"

//==============================================================================
ToneExplorerView::ToneExplorerView()
{
  addAndMakeVisible(header_);
  addAndMakeVisible(optionsPanel_);
  addAndMakeVisible(selectionPanel_);
}

ToneExplorerView::~ToneExplorerView()
{
}

void ToneExplorerView::paint (Graphics& g)
{
  g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void ToneExplorerView::resized()
{
  FlexBox fb;
  fb.flexDirection = FlexBox::Direction::column;
  fb.items.add(FlexItem(header_).withMaxHeight(30.0f).withFlex(1));
  fb.items.add(FlexItem(optionsPanel_).withMaxHeight(20.0f).withFlex(1));
  fb.items.add(FlexItem(selectionPanel_).withFlex(1));
  fb.performLayout(getLocalBounds().toFloat());
}

void ToneExplorerView::displayCategoryView(const StringArray& categories) {
  optionsPanel_.toggleToneTypes(false);
  selectionPanel_.showCategories(categories);
}

void ToneExplorerView::displayBankView(const StringArray& banks) {
  optionsPanel_.toggleToneTypes(true);
  selectionPanel_.showBanks(banks);
}

String ToneExplorerView::getSelectedToneType() {
  return optionsPanel_.getSelectedToneType();
}

ToneExplorerView::Header::Header() {
   addAndMakeVisible(partHeader_);
   addAndMakeVisible(toneHeader_);
}

void ToneExplorerView::Header::resized() {
  
  FlexBox fb;
  fb.justifyContent = FlexBox::JustifyContent::spaceBetween;
 
  fb.items.add(FlexItem(partHeader_).withFlex(1));
  fb.items.add(FlexItem(toneHeader_).withFlex(1));
   
  fb.performLayout(getLocalBounds().toFloat());
}

ToneExplorerView::Header::PartHeader::PartHeader() {
  partLabel_.setText("Part:", dontSendNotification);
  auto* partNumbers = choice::getChoicesFor(choice::PartNumber).get();
  partNumber_.addItemList(*partNumbers, 1);
  
  addAndMakeVisible(partLabel_);
  addAndMakeVisible(partNumber_);
}

void ToneExplorerView::Header::PartHeader::resized() {
  FlexBox fb;
  fb.justifyContent = FlexBox::JustifyContent::flexStart;
  
  int txtWidth = partLabel_.getFont().getStringWidth(partLabel_.getText());
  int borderWidth = partLabel_.getBorderSize().getLeftAndRight();
  
  fb.items.add(FlexItem(partLabel_).withMaxWidth(txtWidth + borderWidth).withFlex(1));
  fb.items.add(FlexItem(partNumber_).withMaxWidth(30).withFlex(1));
  fb.performLayout(getLocalBounds().toFloat());
}

ToneExplorerView::Header::ToneHeader::ToneHeader() {
  toneName_.setText("'76 Stage", dontSendNotification);
  toneName_.setJustificationType(Justification::right);
  toneNumber_.setText("0012", dontSendNotification);
  toneNumber_.setJustificationType(Justification::right);
  
  addAndMakeVisible(toneName_);
  addAndMakeVisible(toneNumber_);
}

void ToneExplorerView::Header::ToneHeader::resized() {
  FlexBox fb;
  fb.justifyContent = FlexBox::JustifyContent::flexEnd;

  fb.items.add(FlexItem(toneName_).withFlex(1));
  fb.items.add(FlexItem(toneNumber_).withFlex(1));
  fb.performLayout(getLocalBounds().toFloat());
}

ToneExplorerView::OptionsPanel::OptionsPanel() {
#if JUCE_PROJUCER_LIVE_BUILD
  selectionTypeGroup_.reset(new TextButtonGroup());
  toneTypeGroup_.reset(new TextButtonGroup());
#else
  selectionTypeGroup_.reset(new TextButtonGroup({"Category", "Bank"}, 1));
  toneTypeGroup_.reset(new TextButtonGroup(*choice::getChoicesFor(choice::ToneType), 2));
#endif
  
  addAndMakeVisible(selectionTypeGroup_.get());
  addChildComponent(toneTypeGroup_.get());
}

void ToneExplorerView::OptionsPanel::toggleToneTypes(bool show) {
  toneTypeGroup_->setVisible(show);
}

String ToneExplorerView::OptionsPanel::getSelectedToneType() {
  return toneTypeGroup_->getSelectedValue();
}

void ToneExplorerView::OptionsPanel::addListener(TextButtonGroup::Listener* listener) {
  selectionTypeGroup_->addListener(listener);
  toneTypeGroup_->addListener(listener);
}

void ToneExplorerView::OptionsPanel::removeListener(TextButtonGroup::Listener* listener) {
  selectionTypeGroup_->removeListener(listener);
  toneTypeGroup_->removeListener(listener);
}

void ToneExplorerView::OptionsPanel::resized() {
  auto area = getLocalBounds();
  FlexBox fb;
  fb.justifyContent = FlexBox::JustifyContent::spaceBetween;
  
  int height = area.getHeight();
  int basis1 = selectionTypeGroup_->getBestWidthForHeight(height);
  fb.items.add(FlexItem(*selectionTypeGroup_).withFlex(0, 1, basis1));
  int basis2 = toneTypeGroup_->getBestWidthForHeight(height);
  fb.items.add(FlexItem(*toneTypeGroup_).withFlex(0, 1, basis2));
  fb.performLayout(area.toFloat());
}

ToneExplorerView::SelectionPanel::SelectionPanel() {
  tabs_.reset(new TabbedButtonBar(juce::TabbedButtonBar::TabsAtTop));

  int barThickness = 30;
  int barWidth = 0;
  for (int i = 0; i < tabs_->getNumTabs(); ++i) {
    barWidth += tabs_->getTabButton(i)->getBestTabLength(barThickness);
  }
  tabs_->setSize(barWidth, barThickness);
  addAndMakeVisible(tabs_.get());
 
  // Viewport for tabs that can be dragged left and right.
  tabView_.setViewedComponent(tabs_.get(), false);
  tabView_.setSize(getLocalBounds().getWidth(), barThickness);
  tabView_.setScrollOnDragEnabled(true);
  tabView_.setScrollBarsShown(false, false);
  addAndMakeVisible(tabView_);
}

void ToneExplorerView::SelectionPanel::resized() {
  FlexBox fb;
  fb.items.add(FlexItem(tabView_).withMaxHeight(30.0f).withFlex(1));
  fb.performLayout(getLocalBounds().toFloat());
}

void ToneExplorerView::SelectionPanel::showCategories(const StringArray& categories) {
  setTabs(categories);
}

void ToneExplorerView::SelectionPanel::showBanks(const StringArray& banks) {
  setTabs(banks);
}

void ToneExplorerView::SelectionPanel::setTabs(const StringArray& names) {
  tabs_->clearTabs();
  auto bgColor = getLookAndFeel().findColour(ResizableWindow::backgroundColourId);
  int height = 30;
  int width = 0;
  int tabIndex = 0;
  for (const String& name : names) {
    if (!name.isEmpty()) {
      tabs_->addTab(name, bgColor, tabIndex);
      width += tabs_->getTabButton(tabIndex)->getBestTabLength(height);
      tabIndex++;
    }
  }
  tabs_->setSize(width, height);
}

ToneExplorer::ToneExplorer(ToneExplorerView& view, const StringArray& expansionBanks) :
  view_(view), expansionBanks_(StringArray(expansionBanks)) {
    view_.getOptionsPanel()->addListener(this);
}

void ToneExplorer::selectionChanged(const int groupId, const String& selection) {
  switch (groupId) {
    case 1:  // selection type
      if (selection == "Category") {
        view_.displayCategoryView(*choice::getChoicesFor(choice::ToneCategory));
      } else {
        String toneType = view_.getSelectedToneType();
        view_.displayBankView(getBanksPerToneType(toneType));
      }
      break;
    case 2:  // tone type
      view_.displayBankView(getBanksPerToneType(selection));
      break;
    default:
      break;
  }
}

StringArray ToneExplorer::getBanksPerToneType(const String& toneType) {
  StringArray banks;
  if (toneType == bank::PcmS || toneType == bank::PcmD)
    banks = *choice::getChoicesFor(choice::InternalBankPcm);
  else
    banks = *choice::getChoicesFor(choice::InternalBank);
  
  auto bankSet = bank::getBanksFor(toneType);
  for (const String& b : expansionBanks_) {
    if (bankSet->find(b) != bankSet->end()) {
      banks.add(b);
    }
  }
  return banks;
}
