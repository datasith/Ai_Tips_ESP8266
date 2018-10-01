/*------------------------------------------------------------------------------
  09/20/2018
  Author: Makerbro
  Platforms: ESP8266
  Language: C++/Arduino
  File: espX_check.ino
  ------------------------------------------------------------------------------
  Description: 
  Code for YouTube video demonstrating how to write code that runs on both the
  ESP8266 and ESP32. In this case, the libraries used have similar class
  methods, thus each board can be handled in one way.
  https://youtu.be/1WqyMF6Jcss

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

#ifdef ESP8266
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include "WiFi.h"
#else
  #error "Not using the ESP8266 nor ESP32!"
#endif

void setup() {
  // put your setup code here, to run once:
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  int n = WiFi.scanNetworks();
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.println(")");
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(5000);
}
