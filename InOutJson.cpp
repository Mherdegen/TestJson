/*************************************************** 
This is a library for testing the InOutJson library to control
outputs (digital and analog) and outputs (digital and PWM) 

JSON-Befehle:
{"topic":"dq","pin":12,"value":0}
{"topic":"dq","pin":12,"value":1}
{"topic":"dq","pin":13,"value":0}
{"topic":"dq","pin":13,"value":1}  
{"topic":"aq","pin":5,"value":0}
{"topic":"aq","pin":5,"value":1023}

Written by Martin Herdegen
BSD license, all text above must be included in any redistribution
****************************************************/

#include "Arduino.h"
#include "ArduinoJson.h"
#include "InOutJson.h"

DigitalOut::DigitalOut(int pin) {
  _pin = pin;
  pinMode(_pin, OUTPUT);
}

void DigitalOut::setValue(int pin, int value) {
  if (pin == _pin) {
    _value = value;
    digitalWrite(_pin, _value);
  }
}

AnalogOut::AnalogOut(int pin) {
  _pin = pin;
  pinMode(_pin, OUTPUT);
}

void AnalogOut::setValue(int pin, int value) {
  if (pin == _pin) {
    _value = value;
    analogWrite(_pin, _value);
  }
}

DigitalIn::DigitalIn(int pin) {
  _pin = pin;
  pinMode(_pin, INPUT_PULLUP);
  _value = 0;
}

int DigitalIn::getValue() {
  int reading = digitalRead(_pin);
  if (reading != _value) {
    _value = reading;
    StaticJsonBuffer<JSON_BUFFER_LENGTH> jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json[KEY_TOPIC] = TOPIC_DIGITAL_IN;
    json[KEY_PIN] = _pin;
    json[KEY_VALUE]= _value;
    json.printTo(Serial);
    Serial.println();
  }
  return _value;
}

AnalogIn::AnalogIn(int pin, unsigned long interval) {
  _pin = pin;
  _value = 0;
  _interval = interval;
}

int AnalogIn::getValue() {
  unsigned long currentTime = millis();
  if (currentTime - _lastUpdate >= _interval) { //update of analog input needed?
    StaticJsonBuffer<JSON_BUFFER_LENGTH> jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json[KEY_TOPIC] = TOPIC_ANALOG_IN;
    json[KEY_PIN] = _pin;
    _value = analogRead(_pin);
    json[KEY_VALUE] = _value;
    json.printTo(Serial);
    Serial.println();
    delay(10);
    _lastUpdate = currentTime;
    return _value;
  }
}

