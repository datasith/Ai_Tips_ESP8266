/*------------------------------------------------------------------------------
  11/30/2016
  Author: Makerbro
  Platforms: ESP8266
  Language: C++/Arduino
  File: server.ino
  ------------------------------------------------------------------------------
  Description: 
  Code for YouTube video demonstrating how to use Python to send data to a
  webserver running on an ESP8266.
  https://youtu.be/CpWhlJXKuDg 
  ------------------------------------------------------------------------------
  Please consider buying products from ACROBOTIC to help fund future
  Open-Source projects like this! We'll always put our best effort in every
  project, and release all our design files and code for you to use. 

  https://acrobotic.com/
  ------------------------------------------------------------------------------
  License:
  Please see attached LICENSE.txt file for details.
------------------------------------------------------------------------------*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

ESP8266WebServer server;
uint8_t pin_led = 2;
char* ssid = "YOUR_SSID";
char* password = "YOUR_PASSWORD";

void setup()
{
  pinMode(pin_led, OUTPUT);
  WiFi.begin(ssid,password);
  Serial.begin(115200);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/",[](){server.send(200,"text/plain","Hello World!");});
  server.on("/toggle",toggleLED);
  server.on("/jblink",jBlinkLED);
  server.on("/qblink",qBlinkLED);
  server.begin();
}

void loop()
{
  server.handleClient();
}

void toggleLED()
{
  digitalWrite(pin_led,!digitalRead(pin_led));
  server.send(204,"");
}

void jBlinkLED()
{
  String data = server.arg("plain");
  StaticJsonBuffer<200> jBuffer;
  JsonObject& jObject = jBuffer.parseObject(data);
  String del = jObject["pause"];
  String n = jObject["times"];
  for (int i=0; i<n.toInt();i++)
  {
   digitalWrite(pin_led,!digitalRead(pin_led));
   delay(del.toInt()); 
  }
  server.send(200,"text/plain","GOT THE DATA!"); 
}

void qBlinkLED()
{
  String del = server.arg("pause");
  String n = server.arg("times");
  for (int i=0; i<n.toInt();i++)
  {
   digitalWrite(pin_led,!digitalRead(pin_led));
   delay(del.toInt()); 
  }
}

