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
class ToneExplorerView    : public Component {
public:
  struct Header : public Component {
    Header();
    void resized() override;
   
    struct PartHeader : public Component {
      PartHeader();
      void resized() override;
      
      Label partLabel_;
      ComboBox partNumber_;
    };
    
    struct ToneHeader : public Component {
      ToneHeader();
      void resized() override;
      
      Label toneName_;
      Label toneNumber_;
    };
    
    //==============================================================================
    PartHeader partHeader_;
    ToneHeader toneHeader_;
  };
 
  struct OptionsPanel : public Component {
    OptionsPanel();
    
    void toggleToneTypes(bool show);
    String getSelectedToneType();
    
    void addListener(TextButtonGroup::Listener* listener);
    void removeListener(TextButtonGroup::Listener* listener);
    void resized() override;
    //void selectionChanged(const int groupId, const String& selection) override;
  
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
    
  private:
    void setTabs(const StringArray& names);
    //============================================================================
    std::unique_ptr<TabbedButtonBar> tabs_;
    Viewport tabView_;
  };
  
  // ToneExplorerView public methods.
  ToneExplorerView();
  ~ToneExplorerView();
  
  void displayCategoryView(const StringArray& categories);
  void displayBankView(const StringArray& banks);
  String getSelectedToneType();

  // Public accessors.
  OptionsPanel* getOptionsPanel() { return &optionsPanel_; }
  SelectionPanel* getSelectionPanel() { return &selectionPanel_; }
  
  // Inherited method overrides.
  void paint (Graphics& g) override;
  void resized() override;

  //==============================================================================
  Header header_;
  OptionsPanel optionsPanel_;
  SelectionPanel selectionPanel_;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToneExplorerView)
};

class ToneExplorer : public TextButtonGroup::Listener {
public:
  ToneExplorer(ToneExplorerView& view, const StringArray& expansionBanks);
  void selectionChanged(const int groupId, const String& selection) override;
  
private:
  StringArray getBanksPerToneType(const String& toneType);
  //==============================================================================
  ToneExplorerView& view_;
  StringArray expansionBanks_;
  ToneMap tones_;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToneExplorer)
};
