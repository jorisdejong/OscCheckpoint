/*
  ==============================================================================

    OscListener.cpp
    Created: 13 Jul 2014 9:06:45pm
    Author:  Joris de Jong

  ==============================================================================
*/

#include "OscListener.h"
#include "oscpack/osc/OscReceivedElements.h"
#include "oscpack/osc/OscPacketListener.h"


OscListener::OscListener ( int p ) : Thread ("OscListener thread"), port ( p )
{
    //create black and white lists
    blacklist.add("/activelayer/");
    blacklist.add("/activeclip/");
    blacklist.add("/connectclip");
    //blacklist.add("/clear");


    sendOpacity(true);
    sendTriggers(true);
    sendPlayhead(false);


    
    
    //create oscsender
    //oscSender = new OscSender();
}

OscListener::~OscListener()
{
    blacklist.clear();
    whitelist.clear();
    oscSenders.clear();
    
    
    signalThreadShouldExit();
    if ( sock != 0)
        sock->close();
    
    stopThread(2000);
}

void OscListener::sendOpacity(bool send)
{
    if (send)
    {
        whitelist.add("/video/opacity/values");
        whitelist.add("/video/fadeout/values");
        whitelist.add("/opacityandvolume");
        whitelist.add("/audio/volume/values");
        whitelist.add("/bypassed");
        whitelist.add("/solo");
    }
    
    else
    {
        whitelist.removeAllInstancesOf("/video/opacity/values");
        whitelist.removeAllInstancesOf("/video/fadeout/values");
        whitelist.removeAllInstancesOf("/opacityandvolume");
        whitelist.removeAllInstancesOf("/audio/volume/values");
        whitelist.removeAllInstancesOf("/bypassed");
        whitelist.removeAllInstancesOf("/solo");
    }
}

void OscListener::sendTriggers(bool send)
{
    if ( send)
    {
        whitelist.add("/clear");
        whitelist.add("/connect");
    }
    else
    {
        whitelist.removeAllInstancesOf("/clear");
        whitelist.removeAllInstancesOf("/connect");
    }
        
}

void OscListener::sendPlayhead(bool send)
{
    if ( send)
    {
        whitelist.add("/video/position/values");
    }
    else
    {
         whitelist.removeAllInstancesOf("/video/position/values");
    }
}


void OscListener::run()
{
	messageData = new MemoryBlock( MAXINPUTBUFFER, true);
	sock = new DatagramSocket( port );
    
    
	int readyForReading;
	int bytesRead;
    
	while (! threadShouldExit())
	{
		readyForReading	= sock->waitUntilReady( true , 1000 );
        
        //if we received something
		if( readyForReading > 0 )
		{
            
            //cast the incoming data as an oscPack packet
            char buffer[MAXINPUTBUFFER];
			bytesRead = sock->read( static_cast <char*> ( buffer ), MAXINPUTBUFFER, false );
            osc::ReceivedPacket p (buffer, bytesRead);
            
            //we're not dealing with bundles, this needs to be turned off in Resolume prefs
            //TODO: maybe give a warning when a bundle is received?
            if ( p.IsMessage() )
            {
                osc::ReceivedMessage m ( p );
                
                //convert to Juce String vars
				const String address = String ( CharPointer_UTF8  ( m.AddressPattern() ) );
				String typeTag = m.TypeTags();
                
                //blacklist
                bool onBlacklist = false;
                for ( int i = 0; i < blacklist.size(); i++ )
                {
                    if ( address.contains(blacklist[i]))
                        onBlacklist = true;
                }
                
                //whitelist
                bool onWhitelist = false;
                for ( int i = 0; i < whitelist.size(); i++ )
                {
                    if ( address.contains(whitelist[i]))
                        onWhitelist = true;
                }
   
                //if the message is not on the blackllist
                //but it is included in the whitelist
                if ( !onBlacklist && onWhitelist )
                {
                    osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
                    OscMessage* m = new OscMessage();
                    if(typeTag == "f" )
                    {
                        float floatValue;
                        args >> floatValue >> osc::EndMessage;
                        DBG("Received: " + address + ", with float " + String (floatValue));
                        
                        
                        m->newMessage(address, floatValue);
                        
                        for ( int i = 0; i < oscSenders.size(); i++)
                            oscSenders[i]->messages.add(*m);
                    }
                    
                    if( typeTag == "i" ) //dealing with buttons
                    {
                        osc::int32 intValue;
                        args >> intValue >> osc::EndMessage;
                        DBG("Received: " + address + ", with int " + String ( (int) intValue));
                        m->newMessage(address, intValue);
                        for ( int i = 0; i < oscSenders.size(); i++)
                            oscSenders[i]->messages.add(*m);
                    }
                }
                
                for ( int i = 0; i < oscSenders.size(); i++)
                    oscSenders[i]->sendOsc();
                
            }
        }
    }
}