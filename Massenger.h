
#ifndef Massenger_h
#define Massenger_h
#define MASSENGERBUFFERSIZE 128
#define MASSENGERBUFFERLAST 127
#define MASSENGERBUFFERSAFE 126

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

extern "C" {
// callback function
    typedef void (*massengerCallbackFunction)(void);
}

#define MASSENGER_ASCII 0
#define MASSENGER_SLIP  1
#define MASSENGER_AUTO  2

class Massenger
{
public:

  Massenger(byte mode=MASSENGER_AUTO, Stream* serial=&Serial);

  /**
   * Flushes previous message and reads serial port. Returns true if new
   * message has arrived.
   */
  bool receive();

  /// Flushes current message in buffer (if any).
  void flush();

  /// If current message matches "address", calls function "callback" and returns true.
  bool dispatch(const char* address, massengerCallbackFunction callback);

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
  int _nextBlob(uint8_t* data, size_t size);
  bool _updateNextIndex();
  bool _hasNext() const;

private:
  bool _process(int serialByte);

  char* tail; // Pointer to tail
  char* last;

  Stream* serial;
  byte mode;

  char token[2];

  uint8_t bufferSize; // size of buffer
  uint8_t nextIndex; // index in the buffer of next argument to read
  char buffer[MASSENGERBUFFERSIZE]; // Buffer that holds the data
};

#endif
