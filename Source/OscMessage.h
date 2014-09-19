/*
  ==============================================================================

    OscMessage.h
    Created: 16 Jul 2014 12:56:08pm
    Author:  Joris de Jong

  ==============================================================================
*/

#ifndef OSCMESSAGE_H_INCLUDED
#define OSCMESSAGE_H_INCLUDED

#include "JuceHeader.h"

//quick class to hold both a String and a float
class OscMessage
{
public:
	OscMessage()
	{
		address = String();
		floatValue = 0.0;
        intValue = 0;
        type = "na";
	};
    
	~OscMessage()
	{
		address = String();
        type = String();
	};
    
    void newMessage( String address_, float value_)
    {
        address = address_;
        floatValue = value_;
        type = "f";
    };
    
    void newMessage( String address_, int value_)
    {
        address = address_;
        intValue = value_;
        type = "i";
    };
    
    String address;
    String type;
    float floatValue;
    int intValue;
    
};



#endif  // OSCMESSAGE_H_INCLUDED
