/*------------------------------------------------------------------------------
  09/04/2018
  Author: Makerbro
  Platforms: ESP8266
  Language: C++/Arduino
  File: wemos_sd_card.ino
  ------------------------------------------------------------------------------
  Description: 
  Code for YouTube video demonstrating how how to increase the available memory 
  of the ESP8266 by using an 8gb micro SD card to store data.
  https://youtu.be/Hu7Znu5smoo
  
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
#include <SdFat.h>
#include <DHTesp.h>

SdFat sd;
DHTesp dht;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Initializing SD card...");
  sd.begin(D8);
  dht.setup(D4, DHTesp::DHT11);

}

void loop() {
  // put your main code here, to run repeatedly:
  String dataString = "";
  float temp = dht.getTemperature();
  dataString += String(temp);
  File dataFile = sd.open("temperature.txt", FILE_WRITE);
  if(dataFile){
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString);
  }
  delay(2000);

}
