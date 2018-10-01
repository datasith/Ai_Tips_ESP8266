/*------------------------------------------------------------------------------
  09/20/2018
  Author: Makerbro
  Platforms: ESP8266
  Language: C++/Arduino
  File: espX_bmp180_check.ino
  ------------------------------------------------------------------------------
  Description: 
  Code for YouTube video demonstrating how to write code that runs on both the
  ESP8266 and ESP32. In this case, the libraries used have different class
  methods, thus each board needs to be handled differently.
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
  #include <Adafruit_BMP085.h>
  Adafruit_BMP085 bmp;  
#elif defined(ESP32)
  #include <SFE_BMP180.h>
  SFE_BMP180 bmp;
#else
  #error "Not using ESP8266 or ESP32!"
#endif

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  bmp.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  double temp = getTemperature();  
  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.println(" *C");
  delay(2000); 
}

double getTemperature()
{
  #ifdef ESP8266
    return bmp.readTemperature();
  #else
    double T;
    delay(bmp.startTemperature());
    bmp.getTemperature(T);
    return T;
  #endif
}
