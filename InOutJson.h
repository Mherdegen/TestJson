#ifndef InOutJson
#define InOutJson

#include "Arduino.h"

#define JSON_BUFFER_LENGTH 100 // Länge des JSON Buffer
#define JSON_ERROR "{\"error\":true}" // JSON-Parser-Fehler

// JSON-Schlüssel
#define KEY_TOPIC "topic"
#define KEY_PIN "pin"
#define KEY_VALUE "value"

// JSON-Werte für JSON-Schlüssel "topic"
#define TOPIC_DIGITAL_IN "di"
#define TOPIC_DIGITAL_OUT "dq"
#define TOPIC_ANALOG_IN "ai"
#define TOPIC_ANALOG_OUT "aq"

class DigitalOut {
  public:
    DigitalOut(int pin);
    void setValue(int pin, int value);
  private:
    int _pin;
    int _value;
};

class AnalogOut {
  public:
    AnalogOut(int pin);
    void setValue(int pin, int value);
  private:
    int _pin;
    int _value;
};

class DigitalIn {
  public:
    DigitalIn(int pin);
    int getValue();
  private:
    int _pin;
    int _value;
};

class AnalogIn {
  public:
    AnalogIn(int pin, unsigned long interval);
    int getValue();
  private:
    int _pin;
    int _value;
    unsigned long _lastUpdate;
    unsigned long _interval;
};

#endif
