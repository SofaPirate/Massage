//ADDED FOR COMPATIBILITY WITH WIRING
extern "C" {
  #include <stdlib.h>
}

#include "Massenger.h"

Massenger::Massenger(byte _mode, Stream* _serial)
{
	serial = _serial;
  mode   = _mode;

  slipEscaping = false;
}

bool Massenger::receive()
{
  // Flush.
  flush();

  // Read serial.
  while (serial->available())
  {
    if (_process(serial->read()))
      return true;
  }

  return false;
}

void Massenger::Massenger::Massenger::flush()
{
  nextIndex = bufferSize = 0;
}

bool Massenger::dispatch(const char* address, callbackFunction callback)
{
  bool matches = (strcmp(buffer, address) == 0);
  if (matches) callback();
  return matches;
}

int8_t Massenger::nextByte(bool* error) {
  return (int8_t) nextInt(error);
}

int16_t Massenger::nextInt(bool* error)
{
  bool err = !_hasNext();
  if (error) *error = err;
  if (err) return 0;

  int16_t value = atoi(&buffer[nextIndex]);
  _updateNextIndex();
  return value;
}

int32_t Massenger::nextLong(bool* error)
{
  bool err = !_hasNext();
  if (error) *error = err;
  if (err) return 0;

  // TODO: reimplement using strtol() to catch errors
  // cf. http://stackoverflow.com/questions/8871711/atoi-how-to-identify-the-difference-between-zero-and-error
  int16_t value = atol(&buffer[nextIndex]);
  _updateNextIndex();
  return value;
}

float Massenger::nextFloat(bool* error)
{
  return (float) nextDouble(error);
}

double Massenger::nextDouble(bool* error)
{
  bool err = !_hasNext();
  if (error) *error = err;
  if (err) return 0;
  double value = atof(&buffer[nextIndex]);
  _updateNextIndex();
  return value;
}

void Massenger::sendBegin(const char* address)
{
  serial->print(address);
}

void Massenger::sendByte(uint8_t value)
{
  sendLong(value);
}

void Massenger::sendInt(int16_t value)
{
  sendLong(value);
}

void Massenger::sendLong(int32_t value)
{
  serial->write(' ');
  serial->print(value);
}

void Massenger::sendFloat(float value)
{
  sendDouble(value);
}

void Massenger::sendDouble(double value)
{
  serial->write(' ');
  serial->print(value);
}

void Massenger::sendEnd()
{
  serial->write('\n');
}

void Massenger::send(const char *address)
{
  sendBegin(address);
  sendEnd();
}

void Massenger::sendByte(const char *address, uint8_t value)
{
  sendBegin(address);
  sendByte(value);
  sendEnd();
}

void Massenger::sendInt(const char *address, int16_t value)
{
  sendBegin(address);
  sendInt(value);
  sendEnd();
}

void Massenger::sendLong(const char *address, int32_t value)
{
  sendBegin(address);
  sendLong(value);
  sendEnd();
}

void Massenger::sendFloat(const char *address, float value)
{
  sendBegin(address);
  sendFloat(value);
  sendEnd();
}

void Massenger::sendDouble(const char *address, double value)
{
  sendBegin(address);
  sendDouble(value);
  sendEnd();
}

bool Massenger::_updateNextIndex()
{
  while (buffer[nextIndex] != 0)
    nextIndex++;
  nextIndex++;
  return (nextIndex < bufferSize);
}

#define MASSENGER_SLIP_END     0xC0
#define MASSENGER_SLIP_ESC     0xDB
#define MASSENGER_SLIP_ESC_END 0xDC
#define MASSENGER_SLIP_ESC_ESC 0xDD

bool Massenger::_process(int serialByte)
{
  // ASCII mode. ///////////////////////////////////////////
  if (mode == MASSENGER_ASCII)
  {
    // Check if we've reached the end of the buffer.
    if (bufferSize >= (MASSENGER_BUFFERSIZE-1))
    {
      bufferSize = MASSENGER_BUFFERSIZE-1;
      _write(0);
      return true;
    }

    // Process byte.
    switch (serialByte)
    {
      case '\n': // LF
      case '\r': // CR
        if (bufferSize > 0) // only process this if we are *not* at beginning
        {
          if (buffer[bufferSize-1] != 0)
            _write(0);

          // Position nextIndex after command address string.
          nextIndex = 0;
          _updateNextIndex();

          return true;
        }
        break;
      case ' ':
        // Put null character instead of space to easily use atoi()/atof() functions.
        if (bufferSize > 0 && buffer[bufferSize-1] != 0)
        {
          _write(0);
        }
        break;
      default: // caught a non-reserved character
        _write(serialByte);
    }

  }
  // Binary/SLIP mode. /////////////////////////////////////
  else
  {
    byte value = 0;

    // Check if we've reached the end of the buffer.
    if (bufferSize >= MASSENGER_BUFFERSIZE)
    {
      bufferSize = MASSENGER_BUFFERSIZE;
      return true;
    }

    // Process byte.
    slipEscaping = false; // reset
    switch (serialByte) {
      case MASSENGER_SLIP_END:
        if (bufferSize > 0) // only process this if we are *not* at beginning
        {
          // Position nextIndex after command address string.
          nextIndex = 0;
          _updateNextIndex();

          return true;
        }
        break;

      case MASSENGER_SLIP_ESC:
        slipEscaping = true;
        break;

      case MASSENGER_SLIP_ESC_END:
        _write(slipEscaping ? MASSENGER_SLIP_END : MASSENGER_SLIP_ESC_END);
        break;
      case MASSENGER_SLIP_ESC_ESC:
        _write(slipEscaping ? MASSENGER_SLIP_ESC : MASSENGER_SLIP_ESC_ESC);
        break;

      default:
        _write(serialByte);
    }
  }

  return false;
}

bool Massenger::_write(uint8_t value)
{
  if (bufferSize >= MASSENGER_BUFFERSIZE)
    return false;
  buffer[bufferSize++] = value;
}

void Massenger::_sendSlipData(const uint8_t* data, size_t n)
{
  while (n--)
  {
    uint8_t value = *data++;
    switch (value)
    {
      case MASSENGER_SLIP_END:
        serial->write(MASSENGER_SLIP_ESC);
        serial->write(MASSENGER_SLIP_ESC_END);
        break;
      case MASSENGER_SLIP_ESC:
        serial->write(MASSENGER_SLIP_ESC);
        serial->write(MASSENGER_SLIP_ESC_ESC);
        break;
      default:
        serial->write(value);
    }
  }
}
