Massage
=================

*Massage* is a microcontroller message packer and parser for different types of formats and protocols. 

A *massage* message always starts with an address string and is followed by a user defined number of bytes, ints, floats or longs.  The address string is used to route the message as in Open Sound Control (http://opensoundcontrol.org/introduction-osc).

There are two basic *massage* objetcs to work with *massages*:
* *MassageParser* parses received data.
* *MassagePacker* packs data to be sent.


This repository (https://github.com/SofaPirate/Massage) is the API. It is implemented through different packing formats that follow the same interface:
* AsciiMassage for easy to read ASCII messages: https://github.com/SofaPirate/AsciiMassage
* SlipMassage for sending lots of binary data with SLIP messages: https://github.com/SofaPirate/SlipMassage

These packing formats can then be used to send and receive messages over Serial, UDP or Websockets.

MassageParser
-------------

`MassageParser ()`: Constructor.
 
`parse (int data, callbackFunction callback)` : Reads one element of serial port, flushing previous message if needed. Returns true iff new message has arrived. Optional callback function will be called if new message has arrived. 
 
`fullMatch (const char *address)` : Return true if current message matches "address".
 
`dispatch (const char *address, callbackFunction callback)` : If current message matches "address", calls function "callback" and returns true.
 
`nextByte ()` : Returns next byte in the parsed massage.

`nextFloat()` : Returns next float in the parsed massage.

`nextInt ()` : Returns next int in the parsed massage.

`nextLong()` : Returns next long in the parsed massage.
 
MassagePacker
-------------

`MassagePacker()` : Constructor

`beginPacket(const char* address)` : Begins a packet (erasing any previous packet).

`addByte(uint8_t value)` : Adds a byte.

`addInt(int16_t value)` : Adds an int.

`addLong(int32_t value)` : Adds a long.

`addFloat(float value)` : Adds a float.

`endPacket()` : Ends a packet.

`packEmpty(const char *address)` : Create a packet with no arguments.
  
`packOneByte(const char *address, uint8_t value)` : Create a packet with a single byte value.

`packOneInt(const char *address, int16_t value)` : Create a packet with a single int value.

`packOneLong(const char *address, int32_t value)` :   Create a packet with a single long value.

`packOneFloat(const char *address, float value)` : Create a packet with a single float value.

`size()` : Returns the size of the packed massage.

`buffer()` : Returns a pointer to the buffer.



