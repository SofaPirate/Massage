Massage
=================

*Massage* is a microcontroller message packer and parser for different types of formats and protocols. 

A *massage* message always starts with an address string and is followed by a user defined number of bytes, ints, floats or longs.  The address string is used to route the message as in Open Sound Control (http://opensoundcontrol.org/introduction-osc).

There are two basic *massage* objetcs to work with *massages*:
* *MassageParser* parses received data.
* *MassagePacker* packs data to be sent.


This repository (https://github.com/SofaPirate/Massage) is the API. It is implemented through different packing formats that follow the same interface:
* AsciiMassage for easy to read ASCII messages: https://github.com/SofaPirate/AsciiMassage
* SlipMassage for sendin lots of data with SLIP messages: https://github.com/SofaPirate/SlipMassage

These packing formats can then be used to send and receive messages over Serial, UDP or Websockets.

MassageParser
-------------

`MassageParser ()`: Constructor.
 
virtual bool 	parse (int data, callbackFunction callback=0)
 
virtual void 	flush ()
 	Flushes current message in buffer (if any).
 
virtual bool 	dispatch (const char *address, callbackFunction callback)
 	If current message matches "address", calls function "callback" and returns true.
 
virtual bool 	fullMatch (const char *address)
 	Return true if current message matches "address".
 
virtual int8_t 	nextByte (bool *error=0)=0
 	Reads next byte.
 
virtual int16_t 	nextInt (bool *error=0)=0
 	Reads next int.
 
virtual int32_t 	nextLong (bool *error=0)=0
 	Reads next long.
 
virtual float 	nextFloat (bool *error=0)=0
 	Reads next float. 
