/*------------------------------------------------------------------------------
  11/20/2016
  Author: Makerbro
  Platforms: ESP8266
  Language: C++/Arduino
  File: server.ino
  ------------------------------------------------------------------------------
  Description: 
  Code for YouTube video demonstrating how to use JavaScript to send data to a
  webserver running on an ESP8266.
  https://youtu.be/hP3xQtrRMmQ
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
#include <ACROBOTIC_SSD1306.h>
#include <ArduinoJson.h>

ESP8266WebServer server;
uint8_t pin_led = 16;
char* ssid = "YOUR_SSID";
char* password = "YOUR_PASSWORD";

void setup()
{
  Wire.begin();
  oled.init();
  oled.clearDisplay();
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
  server.on("/displaydata", HTTP_OPTIONS, displayDataOptions);
  server.on("/displaydata", HTTP_POST, displayData);
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

void displayDataOptions()
{
  server.sendHeader("Access-Control-Allow-Origin","*");
  server.send(204,"");
}

void displayData()
{
  String data = server.arg("plain");
  StaticJsonBuffer<200> jBuffer;
  JsonObject& jObject = jBuffer.parseObject(data);
  String msg = jObject["message"];
  oled.setTextXY(0,0);
  oled.putString(msg);
  server.send(204,"");
}
