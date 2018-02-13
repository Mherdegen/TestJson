// 2017.11.02: jsonString as String-Object replaced by char array
#include <ArduinoJson.h>
#include "InOutJson.h"

const long interval = 1000; //interval of updating inputs in millis

DigitalOut dq12(12);
DigitalOut dq13(13);
AnalogOut aq5(5);
DigitalIn di8(8);
AnalogIn aiA1(A1, interval);

/* JSON-Befehle:
{"topic":"dq","pin":12,"value":0}
{"topic":"dq","pin":12,"value":1}
{"topic":"dq","pin":13,"value":0}
{"topic":"dq","pin":13,"value":1}  
{"topic":"aq","pin":5,"value":0}
{"topic":"aq","pin":5,"value":1023}
*/

char jsonString[JSON_BUFFER_LENGTH];
boolean jsonStringValid = false;
int index = 0;

void setup() {
  Serial.begin(115200); // baud rate
  while (!Serial) {} //wait for serial port
  jsonString[index] = '\0';
  pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW);
  pinMode(A2, OUTPUT);
  digitalWrite(A2, HIGH);
}

int readJson() {
  while (Serial.available() > 0) {
    jsonString[index] = Serial.read();
    if (jsonString[index] == '{') {
      index = 0;
      jsonString[index] = '{';
    }
    if (jsonString[index] == '}') {
      jsonString[index + 1] = '\0';
      jsonStringValid = true;
      break;
    }  
    index++;
    if (index == JSON_BUFFER_LENGTH - 1)
      break;
  }
  if (jsonStringValid) {
    index = 0;
    StaticJsonBuffer<JSON_BUFFER_LENGTH> jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(jsonString);
    if (json.success()) {
      if (json[KEY_TOPIC] == TOPIC_DIGITAL_OUT) { //digital output
        dq12.setValue(json[KEY_PIN], json[KEY_VALUE]);
        dq13.setValue(json[KEY_PIN], json[KEY_VALUE]);
      }
      if (json[KEY_TOPIC] == TOPIC_ANALOG_OUT) { //analog (pwm) output
        aq5.setValue(json[KEY_PIN], json[KEY_VALUE]);
      }
    } else
      Serial.println(JSON_ERROR);
    jsonStringValid = false;
  }  
}

void loop() {
  readJson();
  di8.getValue();
  //aiA0.getValue();
  aiA1.getValue();
}
