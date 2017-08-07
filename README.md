Massage
=================

*Massage* is a microcontroller message packer and parser for different types of formats and protocols. 

A *massage* message always starts with an address string and is followed by a user defined number of bytes, ints, floats or longs. 
The address string is used to route the message as in Open Sound Control.

This repository is the API. It is implemented through different packing formats that follow the same interface:
* AsciiMassage for easy to read ASCII messages: https://github.com/SofaPirate/AsciiMassage
* SlipMassage for sendin lots of data with SLIP messages: https://github.com/SofaPirate/SlipMassage

These packing formats can then be used to send and receive messages over Serial, UDP or Websockets.

License
-------

The MIT License (MIT)

Copyright (c) 2017 Thomas O Fredericks & Sofian Audry

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
