/*------------------------------------------------------------------------------
  10/28/2016
  Author: Makerbro
  Platforms: ESP8266
  Language: C++/Arduino
  File: wifi_modes.ino
  ------------------------------------------------------------------------------
  Description: 
  Code for YouTube video demonstrating how  how to use the ESP8266 in two modes 
  of operation: Station (STA) and Access Point (AP).
  https://youtu.be/uFjWKhScnVY

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

ESP8266WebServer server;
uint8_t pin_led = 16;
char* ssid = "NETWORK_SSID";
char* password = "NETWORK_PASSWORD";
char* mySsid = "ESP8266_SSID";

IPAddress ip(192,168,11,4);
IPAddress gateway(192,168,11,1);
IPAddress subnet(255,255,255,0);

void setup()
{
  pinMode(pin_led, OUTPUT);
  WiFi.mode(WIFI_AP_STA);
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
  server.begin();

  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(mySsid, password);
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
