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

String ToneExplorerView::getCurrentSelectionType() {
  return optionsPanel_.getCurrentSelectionType();
}

void ToneExplorerView::setCurrentSelectionType(const String& selectionType)
{
  optionsPanel_.getSelectionTypeGroup()->setSelectedValue(selectionType);
}

int ToneExplorerView::getSelectedPartNumber()
{
  return getHeader()->getPartHeader()->getSelectedPartNumber();
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
  partNumber_.setSelectedId(1);
  
  addAndMakeVisible(partLabel_);
  addAndMakeVisible(partNumber_);
}

void ToneExplorerView::Header::PartHeader::resized() {
  FlexBox fb;
  fb.justifyContent = FlexBox::JustifyContent::flexStart;
  
  int txtWidth = partLabel_.getFont().getStringWidth(partLabel_.getText());
  int borderWidth = partLabel_.getBorderSize().getLeftAndRight();
  
  fb.items.add(FlexItem(partLabel_).withMaxWidth(txtWidth + borderWidth).withFlex(1));
  fb.items.add(FlexItem(partNumber_).withMinWidth(55).withMaxWidth(55).withFlex(1));
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

String ToneExplorerView::OptionsPanel::getCurrentSelectionType() {
  return selectionTypeGroup_->getSelectedValue();
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
  //tabs_.reset(new TabbedButtonBar(juce::TabbedButtonBar::TabsAtTop));

  /*
  int barThickness = 30;
  int barWidth = 0;
  for (int i = 0; i < tabs_->getNumTabs(); ++i) {
    barWidth += tabs_->getTabButton(i)->getBestTabLength(barThickness);
  }
  tabs_->setSize(barWidth, barThickness);
  addAndMakeVisible(tabs_.get());
  */
  
  /*
  // Viewport for tabs that can be dragged left and right.
  tabView_.setViewedComponent(tabs_.get(), false);
  tabView_.setSize(getLocalBounds().getWidth(), barThickness);
  tabView_.setScrollOnDragEnabled(true);
  tabView_.setScrollBarsShown(false, false, false, true);
  */
  addAndMakeVisible(tabView_);
  
  // Tone tables.
  addAndMakeVisible(categoryTable_);

}

void ToneExplorerView::SelectionPanel::resized() {
  FlexBox fb;
  fb.flexDirection = FlexBox::Direction::column;
  fb.items.add(FlexItem(tabView_).withMaxHeight(30.0f).withFlex(1));
  fb.items.add(FlexItem(categoryTable_).withFlex(1));
  fb.performLayout(getLocalBounds().toFloat());
}

void ToneExplorerView::SelectionPanel::showCategories(const StringArray& categories) {
  setTabs(categories);
}

void ToneExplorerView::SelectionPanel::showBanks(const StringArray& banks) {
  setTabs(banks);
}

const String ToneExplorerView::SelectionPanel::getSelectedTabName() {
  //return tabs_->getCurrentTabName();
  return tabView_.getSelectedTabName();
}

void ToneExplorerView::SelectionPanel::addTabListener(ChangeListener* listener) {
  //tabs_->addChangeListener(listener);
  tabView_.addListener(listener);
}

void ToneExplorerView::SelectionPanel::setTabs(const StringArray& names) {
  /*
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
  */
  tabView_.setTabs(names);
}

TableListBox* ToneExplorerView::SelectionPanel::getToneTable(const SelectionType selectionType) {
  if (selectionType == SelectionType::byCategory) {
    return &categoryTable_;
  } else if (selectionType == SelectionType::byBank) {
    return &bankTable_;
  }
  return nullptr;
}

CategoryToneTable::CategoryToneTable(TableListBox* table, ToneMap* toneMap) :
  table_(table), toneMap_(toneMap), currentCategory_("") {
    table_->setModel(this);
    table_->getHeader().addColumn("Name", 1, 120);
    table_->getHeader().addColumn("Tone Type", 2, 60);
    table_->getHeader().addColumn("Bank", 3, 60);
    table_->getHeader().addColumn("Tone #", 4, 30);

    // Disable automatically selecting row on click. Selection will happen
    // in cellClicked().
    table_->setClickingTogglesRowSelection(false);
}

void CategoryToneTable::categoryChanged(const String& category) {
  if (category == currentCategory_) {
    return;
  }
  
  currentCategory_ = category;
  
  tones_.clearQuick(true);
  for (const auto& ttPair : *toneMap_) {
    for (const auto& bankPair : ttPair.second) {
      for (const ToneInfo& ti : bankPair.second) {
        if (category == ToneCategoryName(ti.category)) {
          tones_.add(new ToneId{ttPair.first, bankPair.first, ti.number});
        }
      }
    }
  }
  
  table_->updateContent();
}

ToneId CategoryToneTable::getSelectedTone(const String& category) {
  return selectedTone_.at(category);
}

ToneId CategoryToneTable::getSelectedTone() {
  return selectedTone_.at(currentCategory_);
}

int CategoryToneTable::getNumRows() {
  return tones_.size();
}

String CategoryToneTable::getTextForColumn(ToneId* toneId, int columnId) {
  const ToneInfo& info = ToneTable::getToneInfoFor(*toneId);
  switch (columnId) {
    case 1:
      return info.displayName;
    case 2:
      return ToneTypeName(toneId->toneType);
    case 3:
      return BankName(toneId->bank);
    case 4:
      return String(info.number);
    default:
      return {};
  }
}

void CategoryToneTable::paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) {
  auto alternateColour = table_->getLookAndFeel().findColour (ListBox::backgroundColourId)
                                         .interpolatedWith (table_->getLookAndFeel().findColour (ListBox::textColourId), 0.03f);
  if (rowIsSelected)
      g.fillAll (Colours::lightblue);
  else if (rowNumber % 2)
      g.fillAll (alternateColour);
}

void CategoryToneTable::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    g.setColour (rowIsSelected ? Colours::darkblue : table_->getLookAndFeel().findColour (ListBox::textColourId));
    //g.setFont (font);

  if (auto* toneId = tones_[rowNumber]) {
    auto text = getTextForColumn(toneId, columnId);

    g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
  }

  g.setColour (table_->getLookAndFeel().findColour (ListBox::backgroundColourId));
  g.fillRect (width - 1, 0, 1, height);
}

void CategoryToneTable::selectedRowsChanged(int lastRowSelected) {
  if (currentCategory_.isEmpty()) {
    return;
  }
  
  if (tones_[lastRowSelected] == nullptr) {
    return;
  }
  ToneId tid = *tones_[lastRowSelected];
  selectedTone_[currentCategory_] = tid;
  
  sendChangeMessage();
}

void CategoryToneTable::cellClicked(int rowNumber, int columnId, const MouseEvent&)
{
  table_->selectRow(rowNumber);
}

ToneExplorer::ToneExplorer(ToneExplorerView& view, OSCSender& oscSender, const StringArray& expansionBanks) :
  view_(view), oscSender_(oscSender), expansionBanks_(StringArray(expansionBanks)) {
    
    tones_.reset(new ToneMap(ToneTable::filteredToneMap(expansionBanks_)));
    categoryTable_.reset(
        new CategoryToneTable(
            view_.getSelectionPanel()->getToneTable(SelectionType::byCategory), tones_.get()));
    
    // Register as listener of view components.
    view_.getOptionsPanel()->addListener(this);
    view_.getSelectionPanel()->addTabListener(this);
    categoryTable_->addChangeListener(this);
    
    // Set defaults.
    view_.setCurrentSelectionType("Category");
    
    // Connect to the OSC receiver.
    if (!oscSender_.connect("127.0.0.1", 9009))
      AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
      "Connection error",
      "Could not connect to UDP port 9009.",
      "OK");
}

void ToneExplorer::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
  if (comboBoxThatHasChanged == view_.getHeader()->getPartHeader()->getPartNumberComboBox()) {
    // TODO: Update selection panel.
  }
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

void ToneExplorer::changeListenerCallback(ChangeBroadcaster *source) {
  if (source == view_.getSelectionPanel()->getTabs()) {
    tabChanged(view_.getSelectionPanel()->getSelectedTabName());
  } else if (source == categoryTable_.get()) {
    sendToneSelectMessage(categoryTable_->getSelectedTone());
  }
  
}

void ToneExplorer::oscMessageReceived(const OSCMessage &message)
{
    if (message.size() != 4) {
      DBG ("ERROR: received invalid tone_select OSC message");
    }
    
    ToneId toneId = {
      static_cast<ToneType>(message[1].getInt32()),
      static_cast<Bank>(message[2].getInt32()),
      message[3].getInt32()
    };
    
}

void ToneExplorer::tabChanged(const String& tabName) {
  const String& selection = view_.getCurrentSelectionType();
  if (selection == "Category") {
    categoryTable_->categoryChanged(tabName);
  }
}

void ToneExplorer::sendToneSelectMessage(const ToneId& toneId) {
  //int partNumber = 1;
  if (!oscSender_.send("/i7/function/tone_select",
                       view_.getSelectedPartNumber(),
                       static_cast<int32>(toneId.toneType),
                       static_cast<int32>(toneId.bank),
                       toneId.toneNumber))
    AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
    "Send Error",
    "Failed to send.",
    "OK");
  //oscSender_.send("/i7/bank", (int32)toneId.bank);
  //oscSender_.send("/i7/tone_number", (int32)toneId.toneNumber);
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
