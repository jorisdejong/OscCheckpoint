/*
  ==============================================================================

    OscSender.cpp
    Created: 11 Jun 2014 8:42:36pm
    Author:  Joris de Jong

  ==============================================================================
*/

#include "OscSender.h"

OscSender::OscSender(IPAddress ip) : DatagramSocket (7001, false)
{
    setIp(ip);
    messages.clear();
}

OscSender::~OscSender()
{
    close();
}

void OscSender::setOscOutputSocket()
{
    if(connect(ADDRESS, PORT, 3000))
    {
        DBG("OSC Output connected");
    }
    else
    {
        DBG("OSC Output connection failed");
    }
}

void OscSender::setIp(IPAddress ip)
{
    close();
    
    if(connect(ip.toString(), PORT, 3000))
    {
        DBG("OSC Output connected on: " + ip.toString());
    }
    else
    {
        DBG("OSC Output connection failed");
    }
}



void OscSender::sendOsc()
{
    //create message and bundle
    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream m( buffer, OUTPUT_BUFFER_SIZE );
    
    //format the bundle, using oscpack library
    //m << osc::BeginBundle();
    for(int i = 0; i < messages.size(); i++)
    {
        
		if( messages[ i ].address.startsWith( "/" ))
        {
            if(messages[i].type == "f")
                m << osc::BeginMessage( messages[i].address.toRawUTF8() ) << messages[i].floatValue << osc::EndMessage ;
            if(messages[i].type == "i")
                m << osc::BeginMessage( messages[i].address.toRawUTF8() ) << messages[i].intValue << osc::EndMessage ;
            
            //send bundle.
            if ( waitUntilReady(false, 3000) == 1 && m.Size() > 0)
            {
                int bytesWritten = write(m.Data(), m.Size());
                if( messages.size() > 0 )
                    messages.clear();
            }
        }
    }
    //m << osc::EndBundle;
   

}
