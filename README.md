Massage
=================

"Massage" is a microcontroller message packer and parser for different types of protocols. 

A “massage” always starts with an address string and is followed by a user defined number of bytes, ints, floats or longs. 

This repository is the API. It is implemented in different packing formats that follow the same interface:
* AsciiMassage for ASCII messages: https://github.com/SofaPirate/AsciiMassage
* SlipMassage for SLIP messages: https://github.com/SofaPirate/SlipMassage
