#ifndef Massenger_h
#define Massenger_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <limits.h>

#ifndef MASSENGER_BUFFERSIZE
/**
 * Max. size of message buffer. Can be predefined before including Massenger.h
 * to increase/decrease size.
 */
#define MASSENGER_BUFFERSIZE 128
#endif


/// Main Massenger abstract class.
class Massenger
{
public:
  typedef void (*callbackFunction)(void);

  /// Constructor.
  Massenger(Stream* stream) : _stream(stream) {}

  // Virtual destructor.
  virtual ~Massenger() {}

  /**
   * Flushes previous message and reads serial port. Returns true if new
   * message has arrived.
   */
  virtual bool receive() = 0;


  /// If current message matches "address", calls function "callback" and returns true.
  virtual bool dispatch(const char* address, callbackFunction callback) = 0;


  /// Reads next byte.
  virtual uint8_t nextByte(bool* error=0) = 0;

  /// Reads next int.
  virtual int16_t nextInt(bool* error=0) = 0;

  /// Reads next long.
  virtual int32_t nextLong(bool* error=0) = 0;

  /// Reads next float.
  virtual float nextFloat(bool* error=0) = 0;

  /// Begins the sending of a message.
  virtual void sendBegin(const char* address) = 0;

  /// Sends a byte.
  virtual void sendByte(uint8_t value) = 0;

  /// Sends an int.
  virtual void sendInt(int16_t value) = 0;

  /// Sends a long.
  virtual void sendLong(int32_t value) = 0;

  /// Sends a float.
  virtual void sendFloat(float value) = 0;

  /// Ends the sending of a message.
  virtual void sendEnd() = 0;

  /// Sends message with no arguments.
  virtual void sendMessage(const char *address)
  {
    sendBegin(address);
    sendEnd();
  }

  /// Sends message with single byte value.
  virtual void sendMessageByte(const char *address, uint8_t value)
  {
    sendBegin(address);
    sendByte(value);
    sendEnd();
  }

  /// Sends message with single int value.
  virtual void sendMessageInt(const char *address, int16_t value)
  {
    sendBegin(address);
    sendInt(value);
    sendEnd();
  }

  /// Sends message with single long value.
  virtual void sendMessageLong(const char *address, int32_t value)
  {
    sendBegin(address);
    sendLong(value);
    sendEnd();
  }

  /// Sends message with single float value.
  virtual void sendMessageFloat(const char *address, float value)
  {
    sendBegin(address);
    sendFloat(value);
    sendEnd();
  }

  // Advanced features (optional).

  /**
   * Sends an integer value within range [0, maxValue]. Needs to be
   * read using nextInteger(maxValue). Subclasses can override protected
   * function _sendInteger() to compress information in a more efficient way.
   */
  virtual void sendInteger(int32_t value, int32_t maxValue) {
    sendInteger(value, 0, maxValue);
  }
  virtual void sendInteger(int16_t value, int16_t maxValue) {
    sendInteger((int32_t)value, (int32_t)0, (int32_t)maxValue);
  }
  virtual void sendInteger(uint8_t value, uint8_t maxValue) {
    sendInteger((int32_t)value, (int32_t)0, (int32_t)maxValue);
  }

  /**
   * Sends an integer value within range [minValue, maxValue]. Needs to be
   * read using nextInteger(minValue, maxValue). Subclasses can override protected
   * function _sendInteger() to compress information in a more efficient way.
   */
  virtual void sendInteger(int32_t value, int32_t minValue, int32_t maxValue) {
    maxValue = max(minValue, maxValue); // make sure minValue <= maxValue
    _sendInteger(contrain(value, minValue, maxValue), minValue, maxValue);
  }
  virtual void sendInteger(int16_t value, int16_t minValue, int16_t maxValue) {
    sendInteger((int32_t)value, (int32_t)minValue, (int32_t)maxValue);
  }
  virtual void sendInteger(uint8_t value, uint8_t minValue, uint8_t maxValue) {
    sendInteger((int32_t)value, (int32_t)minValue, (int32_t)maxValue);
  }

  /// Reads an integer within the range [0, maxValue].
  virtual int32_t nextInteger(int32_t maxValue) {
    return nextInteger(0, maxValue);
  }
  virtual int16_t nextInteger(int16_t maxValue) {
    return nextInteger(0, maxValue);
  }
  virtual uint8_t nextInteger(uint8_t maxValue) {
    return nextInteger(0, maxValue);
  }

  /// Reads an integer within the range [minValue, maxValue].
  virtual int32_t nextInteger(int32_t minValue, int32_t maxValue) {
    return _nextInteger(minValue, max(minValue, maxValue));
  }
  virtual int16_t nextInteger(int16_t minValue, int16_t maxValue) {
    return (int16_t)nextInteger((uint32_t)minValue, (uint32_t)maxValue);
  }
  virtual uint8_t nextInteger(int32_t minValue, int32_t maxValue) {
    return (uint8_t)nextInteger((uint8_t)minValue, (uint8_t)maxValue);
  }

protected:

  // Override to compress in a more efficient way.
  virtual void _sendInteger(int32_t value, int32_t minValue, int32_t maxValue) {
    if (0 <= minValue && maxValue <= UCHAR_MAX)
      sendByte((byte)value);
    else if (INT_MIN <= minValue && maxValue <= INT_MAX)
      sendInt(value);
    else
      sendLong(value);
  }

  // Override to compress in a more efficient way.
  virtual int32_t _nextInteger(int32_t minValue, int32_t maxValue, bool* error=0) {
    if (0 <= minValue && maxValue <= UCHAR_MAX)
      return nextByte(error);
    else if (INT_MIN <= minValue && maxValue <= INT_MAX)
      return nextInt(error);
    else
      return nextLong(error);
  }

  // Pointer to the stream read by this object.
  Stream* _stream;
};




#endif
