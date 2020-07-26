/*
  ==============================================================================

    ToneExplorer.h
    Created: 23 May 2020 12:46:29am
    Author:  Keith

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "GuiCommon.h"

//==============================================================================
/*
*/
enum class SelectionType {
  byCategory, byBank,
};

class ToneExplorerView    : public Component {
public:
  struct Header : public Component {
  
    struct PartHeader : public Component {
      PartHeader();
      void resized() override;
      ComboBox* getPartNumberComboBox() { return &partNumber_; }
      int getSelectedPartNumber() { return partNumber_.getSelectedId(); }
      
      //=============================================================================
      Label partLabel_;
      ComboBox partNumber_;
    };
    
    struct ToneHeader : public Component {
      ToneHeader();
      void resized() override;
      
      Label toneName_;
      Label toneNumber_;
    };
    
    Header();
    void resized() override;
    PartHeader* getPartHeader() { return &partHeader_; }
    ToneHeader* getToneHeader() { return &toneHeader_; }
    
    //==============================================================================
    PartHeader partHeader_;
    ToneHeader toneHeader_;
  };
 
  struct OptionsPanel : public Component {
    OptionsPanel();
    
    void toggleToneTypes(bool show);
    String getSelectedToneType();
    String getCurrentSelectionType();
    
    void addListener(TextButtonGroup::Listener* listener);
    void removeListener(TextButtonGroup::Listener* listener);
    void resized() override;
    //void selectionChanged(const int groupId, const String& selection) override;
  
    // Public accessors.
    TextButtonGroup* getSelectionTypeGroup() { return selectionTypeGroup_.get(); }
    
    //==============================================================================
    std::unique_ptr<TextButtonGroup> selectionTypeGroup_;
    std::unique_ptr<TextButtonGroup> toneTypeGroup_;
    
    OwnedArray<TextButton> toneTypeButtons_;
  };
  
  class SelectionPanel : public Component {
  public:
    SelectionPanel();
    void resized() override;
    
    void showCategories(const StringArray& categories);
    void showBanks(const StringArray& banks);
    
    //TabbedButtonBar* getTabs() { return tabs_.get(); }
    TabbedButtonBar* getTabs() { return tabView_.getTabs(); }
    const String getSelectedTabName();
    void addTabListener(ChangeListener* listener);
    
    // Table methods
    TableListBox* getToneTable(const SelectionType selectionType);
    
  private:
    void setTabs(const StringArray& names);
    //============================================================================
    /*
    std::unique_ptr<TabbedButtonBar> tabs_;
    Viewport tabView_;
    */
    ScrollingTabBar tabView_;
    TableListBox categoryTable_;
    TableListBox bankTable_;
  };
  
  // ToneExplorerView public methods.
  ToneExplorerView();
  ~ToneExplorerView();
  
  void displayCategoryView(const StringArray& categories);
  void displayBankView(const StringArray& banks);
  String getSelectedToneType();
  String getCurrentSelectionType();
  void setCurrentSelectionType(const String& selectionType);

  // Public accessors.
  Header* getHeader() { return &header_; }
  OptionsPanel* getOptionsPanel() { return &optionsPanel_; }
  SelectionPanel* getSelectionPanel() { return &selectionPanel_; }
  int getSelectedPartNumber();
  
  // Inherited method overrides.
  void paint (Graphics& g) override;
  void resized() override;

  //==============================================================================
private:
  Header header_;
  OptionsPanel optionsPanel_;
  SelectionPanel selectionPanel_;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToneExplorerView)
};

class CategoryToneTable : public TableListBoxModel, public ChangeBroadcaster {
public:
  CategoryToneTable(TableListBox* table, ToneMap* toneMap);
  void categoryChanged(const String& category);
  ToneId getSelectedTone(const String& category);
  ToneId getSelectedTone();
  
  // TableListBoxModel implementation.
  int getNumRows() override;
  void paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
  void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
  void selectedRowsChanged(int lastRowSelected) override;
  void cellClicked(int rowNumber, int columnId, const MouseEvent&) override;
  
private:
  String getTextForColumn(ToneId* toneId, int columnId);
  //==============================================================================
  TableListBox* table_;
  OwnedArray<ToneId> tones_;
  ToneMap* toneMap_;
  String currentCategory_;
  std::unordered_map<String, ToneId> selectedTone_;
  
#if JUCE_PROJUCER_LIVE_BUILD
public:
  CategoryToneTable() : CategoryToneTable(nullptr) {}
#endif
};

class ToneExplorer : public TextButtonGroup::Listener,
                     public ChangeListener,
                     public ComboBox::Listener,
                     public OSCReceiver::ListenerWithOSCAddress<OSCReceiver::MessageLoopCallback>
{
public:
  ToneExplorer(ToneExplorerView& view, OSCSender& oscSender, const StringArray& expansionBanks);
  
  // Implements ComboBox::Listener
  void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
  
  // Implements TextButtonGroup::Listener
  void selectionChanged(const int groupId, const String& selection) override;
  
  // Implements ChangeListener
  void changeListenerCallback (ChangeBroadcaster* source) override;
  
  // Implements OSCReceiver::Listener
  void oscMessageReceived(const OSCMessage& message) override;
  
private:
  StringArray getBanksPerToneType(const String& toneType);
  void tabChanged(const String& tabName);
  void sendToneSelectMessage(const ToneId& toneId);
  //==============================================================================
  ToneExplorerView& view_;
  OSCSender& oscSender_;
  StringArray expansionBanks_;
  std::unique_ptr<ToneMap> tones_;
  
  // TableListBoxModels.
  std::unique_ptr<CategoryToneTable> categoryTable_;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToneExplorer)
};


