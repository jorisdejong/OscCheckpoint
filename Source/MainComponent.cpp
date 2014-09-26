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
    enterIp = new TextEditor("enterIp");
    enterIp->setTextToShowWhenEmpty("Enter IP address...", Colours::lightgrey);
    enterIp->addListener(this);
    addAndMakeVisible(enterIp);
    
    //buttons to enable white list groups
    for ( int i = 0; i < 5; i++)
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
                button->setButtonText("Clip Ejects");
                button->setToggleState(false, NotificationType::sendNotification);
                break;
            case 3:
                button->setButtonText("Playhead Position");
                button->setToggleState(false, NotificationType::sendNotification);
                break;
            case 4:
                button->setButtonText("Everything");
                button->setToggleState(false, NotificationType::sendNotification);
                break;
        }
        addAndMakeVisible(button);
        enableWhitelist.add(button);
    }
    
    setSize (500, 400);
}

void MainContentComponent::paint(juce::Graphics &g)
{
    g.setColour(Colours::grey);
    g.drawLine(0, 45, getWidth(), 45, 1);
}

MainContentComponent::~MainContentComponent()
{
    addIp = nullptr;
    enterIp = nullptr;
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
        {
            String text = enterIp->getText();
            if ( text == "" )
                text = "127.0.0.1";
            addIpLine(text);
            enterIp->clear();
        }
    }
    
    for ( int i = 0; i < removeIps.size(); i++)
    {
        if (removeIps[i] == b)
        {
            removeIpLine( i );
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
                    oscListener->sendClears(b->getToggleState());
                    break;
                case 3:
                    oscListener->sendPlayhead(b->getToggleState());
                    break;
                case 4:
                    oscListener->sendEverything(b->getToggleState());
                    for(int i = 0; i < 4; i++)
                        enableWhitelist[i]->setToggleState(false, sendNotification);
                    break;
                    
            }
        }
    }
}

void MainContentComponent::addIpLine(String ip)
{
    //Texteditor for setting ip
    TextEditor* newIp = new TextEditor("ip");
    newIp->setTextToShowWhenEmpty("Enter IP address", Colours::lightgrey);
    newIp->setText(ip);
    newIp->addListener(this);
    addAndMakeVisible(newIp);
    ips.add(newIp);
    
    //Label to keep track of how many ips we have
    Label* newIpLabel = new Label("iplabel");
    newIpLabel->setText("IP " + String (ipLabels.size() + 1) + ":", NotificationType::dontSendNotification);
    addAndMakeVisible(newIpLabel);
    ipLabels.add(newIpLabel);
    
    //button to remove this ip entry
    TextButton* newRemoveIpButton = new TextButton("removeip");
    newRemoveIpButton->setButtonText("-");
    newRemoveIpButton->addListener(this);
    addAndMakeVisible(newRemoveIpButton);
    removeIps.add(newRemoveIpButton);
  
    
    //let's create a sender for this entry
    OscSender* newOscSender = new OscSender(IPAddress(ip));
    oscListener->oscSenders.add(newOscSender);
    //newOscSender->setIp();
    
    //call resize to get the new locations
    resized();
}

void MainContentComponent::removeIpLine(int i)
{
    ips.remove(i);
    ipLabels.remove(i);
    removeIps.remove(i);
    oscListener->oscSenders.remove(i);
    
    resized();
}

//void MainContentComponent::textEditorTextChanged (TextEditor& te)
//{
//
//}

void MainContentComponent::textEditorReturnKeyPressed(juce::TextEditor &te)
{
    if ( &te == enterIp )
    {
        addIp->triggerClick();
    }
    
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
        ipLabels[i]->setBoundsRelative(0.03, 0.123 + i * 0.05, 0.075, 0.049);
        ips[i]->setBoundsRelative(0.1, 0.125 + i * 0.05, 0.50, 0.04);
        
        removeIps[i]->setBoundsRelative(0.61, 0.125 + i * 0.05, 0.05, 0.04);
    }
    enterIp->setBoundsRelative(0.1, 0.05, 0.50, 0.04);
    addIp->setBoundsRelative(0.61, 0.045, 0.05, 0.05);
    
    for(int i = 0; i < enableWhitelist.size(); i++)
    {
        enableWhitelist[i]->setBoundsRelative(0.05 + i * 0.18, 0.9, 0.172, 0.05);
    }
}
