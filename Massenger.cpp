//ADDED FOR COMPATIBILITY WITH WIRING
extern "C" {
  #include <stdlib.h>
}

#include "Massenger.h"

AsciiMassenger::AsciiMassenger(byte mode, Stream* stream)
{
	_stream = stream;
  _mode   = mode;

  _slipEscaping = false;
}

bool AsciiMassenger::receive()
{
  // Flush.
  flush();

  // Read stream.
  while (_stream->available())
  {
    if (_process(_stream->read()))
      return true;
  }

  return false;
}

void AsciiMassenger::flush()
{
  _nextIndex = _messageSize = 0;
  _slipEscaping = false;
}

bool AsciiMassenger::dispatch(const char* address, callbackFunction callback)
{
  bool matches = (strcmp(_buffer, address) == 0);
  if (matches) callback();
  return matches;
}

int8_t AsciiMassenger::nextByte(bool* error) {
  int8_t v;
  _nextInteger((uint8_t*)&v, sizeof(int8_t), error);
  return v;
}

int16_t AsciiMassenger::nextInt(bool* error)
{
  int16_t v;
  _nextInteger((uint8_t*)&v, sizeof(int16_t), error);
  return v;
}

int32_t AsciiMassenger::nextLong(bool* error)
{
  int32_t v;
  _nextInteger((uint8_t*)&v, sizeof(int32_t), error);
  return v;
}

float AsciiMassenger::nextFloat(bool* error)
{
  float v;
  _nextReal((uint8_t*)&v, sizeof(float), error);
  return v;
}

double AsciiMassenger::nextDouble(bool* error)
{
  double v;
  _nextReal((uint8_t*)&v, sizeof(double), error);
  return v;
}

void AsciiMassenger::sendBegin(const char* address)
{
  _stream->print(address);
}

void AsciiMassenger::sendByte(uint8_t value)
{
  sendLong(value);
}

void AsciiMassenger::sendInt(int16_t value)
{
  sendLong(value);
}

void AsciiMassenger::sendLong(int32_t value)
{
  _stream->write(' ');
  _stream->print(value);
}

void AsciiMassenger::sendFloat(float value)
{
  sendDouble(value);
}

void AsciiMassenger::sendDouble(double value)
{
  _stream->write(' ');
  _stream->print(value);
}

void AsciiMassenger::setMode(uint8_t mode)
{
  // Check if mode is changed or valid.
  if (mode != _mode && mode <= MASSENGER_AUTO)
  {
    // Set mode and reset buffer.
    _mode = mode;
    flush();
  }
}

void AsciiMassenger::sendEnd()
{
  _stream->write('\n');
}

void AsciiMassenger::send(const char *address)
{
  sendBegin(address);
  sendEnd();
}

void AsciiMassenger::sendByte(const char *address, uint8_t value)
{
  sendBegin(address);
  sendByte(value);
  sendEnd();
}

void AsciiMassenger::sendInt(const char *address, int16_t value)
{
  sendBegin(address);
  sendInt(value);
  sendEnd();
}

void AsciiMassenger::sendLong(const char *address, int32_t value)
{
  sendBegin(address);
  sendLong(value);
  sendEnd();
}

void AsciiMassenger::sendFloat(const char *address, float value)
{
  sendBegin(address);
  sendFloat(value);
  sendEnd();
}

void AsciiMassenger::sendDouble(const char *address, double value)
{
  sendBegin(address);
  sendDouble(value);
  sendEnd();
}

bool AsciiMassenger::_updateNextIndex()
{
  while (_buffer[_nextIndex] != 0)
    _nextIndex++;
  _nextIndex++;
  return (_nextIndex < _messageSize);
}

bool AsciiMassenger::_hasNext() const {
  return (_nextIndex < _messageSize);
}

void AsciiMassenger::_nextInteger(uint8_t* number, size_t n, bool* error)
{
  bool err = !_hasNext();
  if (error) *error = err;

  if (err)
    memset(number, 0, n); // set to zero (default)

  else if (isAscii())
  {
    long val = strtol(&_buffer[_nextIndex], 0, 10);
    memcpy(number, &val, n);
    _updateNextIndex();
  }

  else
  {
    memcpy(number, &_buffer[_nextIndex], n);
    _nextIndex += n;
  }
}

void AsciiMassenger::_nextReal(uint8_t* value, size_t n, bool* error)
{
  bool err = !_hasNext();
  if (error) *error = err;

  if (err)
    memset(value, 0, n); // set to zero (default)

  else if (isAscii())
  {
    double val = strtod(&_buffer[_nextIndex], 0);
    memcpy(value, &val, n);
    _updateNextIndex();
  }

  else
  {
    memcpy(value, &_buffer[_nextIndex], n);
    _nextIndex += n;
  }
}

// Internal. SLIP reserved codes.
#define MASSENGER_SLIP_END     0xC0
#define MASSENGER_SLIP_ESC     0xDB
#define MASSENGER_SLIP_ESC_END 0xDC
#define MASSENGER_SLIP_ESC_ESC 0xDD

bool AsciiMassenger::_process(int streamByte)
{
  // ASCII mode. ///////////////////////////////////////////
  if (_mode == MASSENGER_ASCII)
  {
    // Check if we've reached the end of the buffer.
    if (_messageSize >= (MASSENGER_BUFFERSIZE-1))
    {
      _messageSize = MASSENGER_BUFFERSIZE-1;
      _write(0);
      return true;
    }

    // Process byte.
    switch (streamByte)
    {
      case '\n': // LF
      case '\r': // CR
        if (_messageSize > 0) // only process this if we are *not* at beginning
        {
          if (_buffer[_messageSize-1] != 0)
            _write(0);

          // Position _nextIndex after command address string.
          _nextIndex = 0;
          _updateNextIndex();

          return true;
        }
        break;
      case ' ':
        // Put null character instead of space to easily use atoi()/atof() functions.
        if (_messageSize > 0 && _buffer[_messageSize-1] != 0)
        {
          _write(0);
        }
        break;
      default: // caught a non-reserved character
        _write(streamByte);
    }

  }
  // Binary/SLIP mode. /////////////////////////////////////
  else
  {
    byte value = 0;

    // Check if we've reached the end of the buffer.
    if (_messageSize >= MASSENGER_BUFFERSIZE)
    {
      _messageSize = MASSENGER_BUFFERSIZE;
      return true;
    }

    // Process byte.
    _slipEscaping = false; // reset
    switch (streamByte) {
      case MASSENGER_SLIP_END:
        if (_messageSize > 0) // only process this if we are *not* at beginning
        {
          // Position _nextIndex after command address string.
          _nextIndex = 0;
          _updateNextIndex();

          return true;
        }
        break;

      case MASSENGER_SLIP_ESC:
        _slipEscaping = true;
        break;

      case MASSENGER_SLIP_ESC_END:
        _write(_slipEscaping ? MASSENGER_SLIP_END : MASSENGER_SLIP_ESC_END);
        break;
      case MASSENGER_SLIP_ESC_ESC:
        _write(_slipEscaping ? MASSENGER_SLIP_ESC : MASSENGER_SLIP_ESC_ESC);
        break;

      default:
        _write(streamByte);
    }
  }

  return false;
}

bool AsciiMassenger::_write(uint8_t value)
{
  if (_messageSize >= MASSENGER_BUFFERSIZE)
    return false;
  _buffer[_messageSize++] = value;
}

void AsciiMassenger::_sendSlipData(const uint8_t* data, size_t n)
{
  while (n--)
  {
    uint8_t value = *data++;
    switch (value)
    {
      case MASSENGER_SLIP_END:
        _stream->write(MASSENGER_SLIP_ESC);
        _stream->write(MASSENGER_SLIP_ESC_END);
        break;
      case MASSENGER_SLIP_ESC:
        _stream->write(MASSENGER_SLIP_ESC);
        _stream->write(MASSENGER_SLIP_ESC_ESC);
        break;
      default:
        _stream->write(value);
    }
  }
}
