/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
{
    laf = new LookAndFeel_V3;
    setLookAndFeel(laf);
    
    //create new OscListener at port 7001 and start listening
    oscListener = new OscListener(7001);
    oscListener->startThread();
    
    //add a button to create new IP lines
    addIp = new TextButton("addIpLine");
    addIp->setButtonText("Add IP");
    addIp->addListener(this);
    addAndMakeVisible(addIp);
    
    //add a new line to enter an IP address
    //this also creates an OSC sender at 127.0.0.1
    addIpLine();
    
    //buttons to enable white list groups
    for ( int i = 0; i < 3; i++)
    {
        TextButton* button = new TextButton("enablewhitelist");
        button->setClickingTogglesState(true);
        button->setColour(TextButton::buttonColourId, Colours::darkgrey);
        button->setColour(TextButton::buttonOnColourId, Colours::lightgrey);
        button->addListener(this);
        switch ( i )
        {
            case 0:
                button->setButtonText("Layer Opacity");
                button->setToggleState(true, NotificationType::sendNotification);
                break;
            case 1:
                button->setButtonText("Clip Triggers");
                button->setToggleState(true, NotificationType::sendNotification);
                break;
            case 2:
                button->setButtonText("Playhead Position");
                button->setToggleState(false, NotificationType::sendNotification);
                break;
        }
        addAndMakeVisible(button);
        enableWhitelist.add(button);
    }
    
    setSize (500, 400);
}

MainContentComponent::~MainContentComponent()
{
    ips.clear();
    ipLabels.clear();
    removeIps.clear();
    oscListener = nullptr;
    laf = nullptr;
}

void MainContentComponent::buttonClicked(juce::Button *b)
{
    if (b == addIp)
    {
        //no more than ten ips
        if(ips.size() < 10)
            addIpLine();
    }
    
    for ( int i = 0; i < removeIps.size(); i++)
    {
        if (removeIps[i] == b)
        {
            removeIpLine( i + 1);
        }
    }
    
    for ( int i = 0; i < enableWhitelist.size(); i++)
    {
        if ( enableWhitelist[i] == b)
        {
            switch(i)
            {
                case 0:
                    oscListener->sendOpacity(b->getToggleState());
                    break;
                case 1:
                    oscListener->sendTriggers(b->getToggleState());
                    break;
                case 2:
                    oscListener->sendPlayhead(b->getToggleState());
                    break;
                    
            }
        }
    }
}

void MainContentComponent::addIpLine()
{
    //Texteditor for setting ip
    TextEditor* newIp = new TextEditor("ip");
    newIp->setTextToShowWhenEmpty("Enter IP address", Colours::lightgrey);
    newIp->addListener(this);
    addAndMakeVisible(newIp);
    ips.add(newIp);
    
    //Label to keep track of how many ips we have
    Label* newIpLabel = new Label("iplabel");
    newIpLabel->setText("IP " + String (ipLabels.size() + 1) + ":", NotificationType::dontSendNotification);
    addAndMakeVisible(newIpLabel);
    ipLabels.add(newIpLabel);
    
    //button to remove this ip entry
    if ( ips.size() > 0 )
    {
        TextButton* newRemoveIpButton = new TextButton("removeip");
        newRemoveIpButton->setButtonText("-");
        newRemoveIpButton->addListener(this);
        addAndMakeVisible(newRemoveIpButton);
        removeIps.add(newRemoveIpButton);
    }
    
    //let's create a sender for this entry
    OscSender* newOscSender = new OscSender();
    oscListener->oscSenders.add(newOscSender);
    
    //call resize to get the new locations
    resized();
}

void MainContentComponent::removeIpLine(int i)
{
    ips.remove(i);
    ipLabels.remove(i);
    removeIps.remove(i - 1);
    oscListener->oscSenders.remove(i);
    
    resized();
}

void MainContentComponent::textEditorTextChanged (TextEditor& te)
{
    for(int i = 0; i < ips.size(); i++)
    {
        if ( ips[i] == &te )
        {
            oscListener->oscSenders[i]->setIp(IPAddress(te.getText()));
        }
    }
}

void MainContentComponent::resized()
{
    for(int i = 0; i < ips.size(); i++)
    {
        ipLabels[i]->setBoundsRelative(0.03, 0.045 + i * 0.05, 0.075, 0.049);
        ips[i]->setBoundsRelative(0.1, 0.05 + i * 0.05, 0.50, 0.04);
        if ( i > 0 )
            removeIps[i - 1]->setBoundsRelative(0.61, 0.05 + i * 0.05, 0.05, 0.04);
    }
    
    addIp->setBoundsRelative(0.5, 0.05 + ips.size() * 0.05, 0.1, 0.05);
    
    for(int i = 0; i < enableWhitelist.size(); i++)
    {
        enableWhitelist[i]->setBoundsRelative(0.05 + i * 0.21, 0.9, 0.2, 0.05);
    }
}
