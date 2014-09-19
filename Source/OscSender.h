/*
  ==============================================================================

    OscSender.h
    Created: 11 Jun 2014 8:42:36pm
    Author:  Joris de Jong

  ==============================================================================
*/

#ifndef OSCSENDER_H_INCLUDED
#define OSCSENDER_H_INCLUDED

#define ADDRESS "192.168.178.43"
#define PORT 7000
#define OUTPUT_BUFFER_SIZE 1024

#include "oscpack/osc/OscOutboundPacketStream.h"
#include "JuceHeader.h"
#include "OscMessage.h"


class OscSender : public DatagramSocket 
{
public:
    OscSender();
    ~OscSender();
    
    void setOscOutputSocket();
    void sendOsc ();
    
    Array < OscMessage > messages;
    
    void setIp( IPAddress ip );

private:

    
    
};




#endif  // OSCMANAGER_H_INCLUDED
