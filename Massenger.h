#ifndef Massenger_h
#define Massenger_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/// ASCII mode.
#define MASSENGER_ASCII  0

/// Binary/SLIP mode.
#define MASSENGER_BINARY 1

/// "Auto" mode (experimental).
#define MASSENGER_AUTO   2

#ifndef MASSENGER_BUFFERSIZE
/**
 * Max. size of message buffer. Can be predefined before including Massenger.h
 * to increase/decrease size.
 */
#define MASSENGER_BUFFERSIZE 128
#endif

/// Main Massenger class.
class Massenger
{
  typedef void (*callbackFunction)(void);

public:
  /// Constructor.
  Massenger(byte mode=MASSENGER_AUTO, Stream* stream=&Serial);

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

  /// Returns current mode.
  uint8_t mode() { return _mode; }

  /// Changes the mode. Notice: this might flush the current message buffer.
  void setMode(uint8_t mode);

  /// Returns true iff we are in ASCII mode.
  bool isAscii() const { return _mode == MASSENGER_ASCII; }

  /// Returns true iff we are in binary/SLIP mode.
  bool isBinary() const { return _mode == MASSENGER_BINARY; }

private:
  // Moves nextIndex to the next token.
  bool _updateNextIndex();

  // Returns true iff it is still possible to call next*().
  bool _hasNext() const;

  // Helper function to read next integer (byte, int, long).
  void _nextInteger(uint8_t* value, size_t n, bool* error);

  // Helper function to read next real (float/double).
  void _nextReal(uint8_t* value, size_t n, bool* error);

  // Processes a single value read from the serial stream.
  bool _process(int serialByte);

  // Writes single byte to buffer (returns false if buffer is full and cannot be written to).
  bool _write(uint8_t value);

  // Sends *n* bytes of *data* to serial using SLIP.
  void _sendSlipData(const uint8_t* data, size_t n);

  // Pointer to the stream read by this object.
  Stream* _stream;

  // Messenging mode (ASCII, BINARY/SLIP, AUTO).
  byte _mode;

  // SLIP decoding state: currently escaping.
  bool _slipEscaping;

  // Current size of message in buffer.
  uint8_t _messageSize;

  // Index in the buffer of next argument to read.
  uint8_t _nextIndex;

  // Buffer that holds the data for current message.
  char _buffer[MASSENGER_BUFFERSIZE];
};

#endif
