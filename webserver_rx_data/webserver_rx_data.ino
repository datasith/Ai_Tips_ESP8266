/*------------------------------------------------------------------------------
  11/01/2016
  Author: Makerbro
  Platforms: ESP8266
  Language: C++/Arduino
  File: webserver_rx_data.ino
  ------------------------------------------------------------------------------
  Description: 
  Code for YouTube video demonstrating how to send data to a webserver running
  on an ESP8266.
  https://youtu.be/EAUCajLvDKw
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
#include <Adafruit_NeoPixel.h>

ESP8266WebServer server;
uint8_t pin_led = 16;
char* ssid = "YOUR_SSID";
char* password = "YOUR_PASSWORD";

#define PIN D1
Adafruit_NeoPixel strip = Adafruit_NeoPixel(20, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  strip.begin();
  strip.show();

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
  Serial.print(WiFi.localIP());

  server.on("/",[](){server.send(200,"text/plain","Hello World!");});
  server.on("/toggle",toggleLED);
  server.on("/setcolor",setPixelColor);
  server.on("/reset",resetPixelColor);
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

void setPixelColor()
{
  String pixel_num = server.arg("pixel_num");
  String r = server.arg("red");
  String g = server.arg("grn");
  String b = server.arg("blu");
  strip.setPixelColor(pixel_num.toInt(), r.toInt(), g.toInt(), b.toInt());
  strip.show();
  String rsp = "LED #"+pixel_num+" is set to: "+r+", "+g+", "+b;
  server.send(200,"text/plain",rsp);
}

void resetPixelColor()
{
  strip.clear();
  strip.show();
  server.send(200,"text/plain","All LEDs are off!");
}
