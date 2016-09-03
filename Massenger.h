
#ifndef Massenger_h
#define Massenger_h
#define MASSENGER_BUFFERSIZE 128

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define MASSENGER_ASCII  0
#define MASSENGER_BINARY 1
#define MASSENGER_AUTO   2

class Massenger
{
  typedef void (*callbackFunction)(void);

public:
  // Constructor.
  Massenger(byte mode=MASSENGER_AUTO, Stream* serial=&Serial);

  /**
   * Flushes previous message and reads serial port. Returns true if new
   * message has arrived.
   */
  bool receive();

  /// Flushes current message in buffer (if any).
  void flush();

  /// If current message matches "address", calls function "callback" and returns true.
  bool dispatch(const char* address, callbackFunction callback);

  /// Reads next byte.
  int8_t nextByte(bool* error=0);

  /// Reads next int.
  int16_t nextInt(bool* error=0);

  /// Reads next long.
  int32_t nextLong(bool* error=0);

  /// Reads next float.
  float nextFloat(bool* error=0);

  /// Reads next double.
  double nextDouble(bool* error=0);

  /// Begins the sending of a message.
  void sendBegin(const char* address);

  /// Sends a byte.
  void sendByte(uint8_t value);

  /// Sends an int.
  void sendInt(int16_t value);

  /// Sends a long.
  void sendLong(int32_t value);

  /// Sends a float.
  void sendFloat(float value);

  /// Sends a double.
  void sendDouble(double value);

  /// Ends the sending of a message.
  void sendEnd();

  /// Sends message with no arguments.
  void send(const char *address);

  /// Sends message with single byte value.
  void sendByte(const char *address, uint8_t value);

  /// Sends message with single int value.
  void sendInt(const char *address, int16_t value);

  /// Sends message with single long value.
  void sendLong(const char *address, int32_t value);

  /// Sends message with single float value.
  void sendFloat(const char *address, float value);

  /// Sends message with single double value.
  void sendDouble(const char *address, double value);

private:
  // Moves nextIndex to the next token.
  bool _updateNextIndex();

  // Returns true iff it is still possible to call next*().
  bool _hasNext() const;

  // Processes a single value read from the serial stream.
  bool _process(int serialByte);

  // Writes single byte to buffer (returns false if buffer is full and cannot be written to).
  bool _write(uint8_t value);

  // Sends *n* bytes of *data* to serial using SLIP.
  void _sendSlipData(const uint8_t* data, size_t n);

  // Pointer to the stream read by this object.
  Stream* serial;

  // Messenging mode (ASCII, BINARY/SLIP, AUTO).
  byte mode;

  // SLIP decoding state: currently escaping.
  bool slipEscaping;

  // Size of buffer.
  uint8_t bufferSize;

  // Index in the buffer of next argument to read.
  uint8_t nextIndex;

  // Buffer that holds the data for current message.
  char buffer[MASSENGER_BUFFERSIZE];
};

#endif
