/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "OscListener.h"
#include "OscSender.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component, public TextEditorListener, public ButtonListener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void resized();
    void paint(Graphics &g);
    
   // void textEditorTextChanged (TextEditor& te);
    void textEditorReturnKeyPressed ( TextEditor& te);
    void buttonClicked(Button* b);
    
    void addIpLine(String ip);
    void removeIpLine(int i);

private:
    
    OwnedArray <TextButton> enableWhitelist;
    ScopedPointer<TextEditor> enterIp;
    OwnedArray <TextEditor> ips;
    OwnedArray <Label> ipLabels;
    OwnedArray <TextButton> removeIps;
    ScopedPointer <TextButton> addIp;
    ScopedPointer<OscListener> oscListener;
    ScopedPointer<LookAndFeel> laf;
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
