#ifndef AsciiMassenger_h
#define AsciiMassenger_h

#include "Massenger.h"

/// Main AsciiMassenger class.
class AsciiMassenger : public Massenger
{
public:
  /// Constructor.
  AsciiMassenger(Stream* stream=&Serial);

  /// Flushes current message in buffer (if any).
  virtual void flush();

  /// Reads next byte.
  virtual int8_t nextByte(bool* error=0);

  /// Reads next int.
  virtual int16_t nextInt(bool* error=0);

  /// Reads next long.
  virtual int32_t nextLong(bool* error=0);

  /// Reads next float.
  virtual float nextFloat(bool* error=0);

  /// Reads next double.
  virtual double nextDouble(bool* error=0);

  /// Begins the sending of a message.
  virtual void sendBegin(const char* address);

  /// Sends a byte.
  virtual void sendByte(uint8_t value);

  /// Sends an int.
  virtual void sendInt(int16_t value);

  /// Sends a long.
  virtual void sendLong(int32_t value);

  /// Sends a float.
  virtual void sendFloat(float value);

  /// Sends a double.
  virtual void sendDouble(double value);

  /// Ends the sending of a message.
  virtual void sendEnd();

  /// Sends message with no arguments.
  virtual void send(const char *address);


  void debug() {
    Serial << "MASSENGER CONTENT ====" << endl;
    Serial << "length: " << _messageSize << endl
           << "next:   " << _nextIndex << endl;
    for (int i=0; i<MASSENGER_BUFFERSIZE; i++)
      Serial << (int)_buffer[i] << ":" << (char)_buffer[i] << " ";
    Serial.println();
  }
protected:
  /// Processes a single value read from the serial stream.
  virtual bool _process(int serialByte);

private:
  // Moves nextIndex to the next token.
  bool _updateNextIndex();

  // Returns true iff it is still possible to call next*().
  bool _hasNext() const;

  // Helper function to read next value.
  void _nextBlock(bool isInteger, uint8_t* value, size_t n, bool* error);

  // Index in the buffer of next argument to read.
  uint8_t _nextIndex;
};




#endif
