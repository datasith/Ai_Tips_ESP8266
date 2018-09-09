/*------------------------------------------------------------------------------
  12/28/2016
  Author: Makerbro
  Platforms: ESP8266
  Language: C++/Arduino
  File: webserver_mdns.ino
  ------------------------------------------------------------------------------
  Description: 
  Code for YouTube video demonstrating how to use mDNS.
  https://youtu.be/ewrF1LzrxH8 

  Do you like my videos? You can support the channel:
  https://patreon.com/acrobotic
  https://paypal.me/acrobotic
  ------------------------------------------------------------------------------
  Please consider buying products from ACROBOTIC to help fund future
  Open-Source projects like this! We'll always put our best effort in every
  project, and release all our design files and code for you to use. 

  https://acrobotic.com/
  https://amazon.com/acrobotic
  ------------------------------------------------------------------------------
  License:
  Please see attached LICENSE.txt file for details.
------------------------------------------------------------------------------*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

ESP8266WebServer server;
uint8_t pin_led = 16;
char* ssid = "YOUR_SSID";
char* password = "YOUR_PASSWORD";

MDNSResponder mdns;

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

  if (mdns.begin("esp8266-01", WiFi.localIP()))
    Serial.println("MDNS responder started");

  server.on("/",[](){server.send(200,"text/plain","Hello World!");});
  server.on("/toggle",toggleLED);
  server.begin();

  MDNS.addService("http", "tcp", 80);
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
