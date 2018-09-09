/*------------------------------------------------------------------------------
  11/01/2016
  Author: Makerbro
  Platforms: ESP8266
  Language: C++/Arduino
  File: simple_client.ino
  ------------------------------------------------------------------------------
  Description: 
  Code for YouTube video demonstrating how to run a web client on the ESP8266 to
  get Weather Data from the Weather Underground API:
  https://youtu.be/Edbxyl2BhyU
  
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
#include <ArduinoJson.h>

ESP8266WebServer server;
uint8_t pin_led = 16;
char* ssid = "YOUR_SSID";
char* password = "YOUR_PASSWORD";

#define WU_API_KEY "YOUR_WUNDERGROUND_API_KEY"
#define WU_LOCATION "Australia/Sydney"
#define WU_URL "api.wunderground.com"

static char respBuffer[4096];

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
  server.on("/getdata",getData);
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

WiFiClient client;

void getData()
{
  const char request[] = 
    "GET /api/" WU_API_KEY "/conditions/q/" WU_LOCATION ".json HTTP/1.1\r\n"
    "User-Agent: ESP8266/0.1\r\n"
    "Accept: */*\r\n"
    "Host: " WU_URL "\r\n"
    "Connection: close\r\n"
    "\r\n";
  Serial.println(request);
  client.connect(WU_URL, 80);
  client.print(request);
  client.flush();
  delay(1000);

  uint16_t index = 0;
  while(client.connected())
  {
    if(client.available())
    {
      respBuffer[index++] = client.read();
      delay(1);
    }
  }
  client.stop();
  char * json = strchr(respBuffer,'{');

  DynamicJsonBuffer jBuffer;
  JsonObject& root = jBuffer.parseObject(json);
  JsonObject& current = root["current_observation"];
  String temp_c = current["temp_c"];
  String weather = current["weather"];
  String msg = "Temp(C): "+temp_c+", Weather: "+weather;
  server.send(200,"text/plain", msg);
}
