/*
  ==============================================================================

    OscListener.h
    Created: 13 Jul 2014 9:06:45pm
    Author:  Joris de Jong

  ==============================================================================
*/

#ifndef OSCLISTENER_H_INCLUDED
#define OSCLISTENER_H_INCLUDED

#define MAXINPUTBUFFER 65336

#include "JuceHeader.h"
#include "OscSender.h"




class OscListener : public Thread
{
public:
    OscListener (int p);
    ~OscListener();
    
    void run();
    
    void sendOpacity ( bool send );
    void sendTriggers ( bool send );
    void sendPlayhead ( bool send );
    void sendClears ( bool send );
    void sendEverything ( bool send );
    
    OwnedArray<OscSender> oscSenders;
    
private:
    int port;
    ScopedPointer < MemoryBlock > messageData;
    ScopedPointer < DatagramSocket > sock;
    

    
    Array <String> blacklist;
    Array <String> whitelist;
    bool openEverything;
    
};



#endif  // OSCLISTENER_H_INCLUDED
