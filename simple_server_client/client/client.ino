/*------------------------------------------------------------------------------
  03/25/2019
  Author: Makerbro
  Platforms: ESP8266
  Language: C++/Arduino
  File: simple_server_client/client/client.ino
  ------------------------------------------------------------------------------
  Description: 
  Client-side code for YouTube video demonstrating how to communicate over WiFi
  between two ESP8266:
  https://youtu.be/gZhUi24_qms

  Do you like my videos? You can support the channel:
  https://patreon.com/acrobotic
  https://paypal.me/acrobotic
  ------------------------------------------------------------------------------
  Please consider buying products from ACROBOTIC to help fund future
  Open-Source projects like this! We'll always put our best effort in every
  project, and release all our design files and code for you to use. 

  https://acrobotic.com/
  https://amazon.com/shops/acrobotic
  ------------------------------------------------------------------------------
  License:
  Please see attached LICENSE.txt file for details.
------------------------------------------------------------------------------*/
// Setup the client to send sensor data to the server
#include <ESP8266WiFi.h>

// Initialize sensor parameters
float volts = 0.0, temperatureC = 0.0;

// Initialize network parameters
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
const char* host = "192.168.11.4"; // as specified in server.ino

// Set up the client objet
WiFiClient client;

// Configure deep sleep in between measurements
const int sleepTimeSeconds = 2;

void setup() {
  // Configure the sensor pin (optional)
  pinMode(A0, INPUT);
  // Connect to the server
  WiFi.begin(ssid,password);
  Serial.begin(115200);
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("IP Address (AP): "); Serial.println(WiFi.localIP());
  // Get a measurement from the sensor
  volts = analogRead(A0)*3.3/1024; // 10-Bit ADC on a 3.3VDC board
  temperatureC = (volts - 0.5) * 100; // Conversion from voltage to temperature follows the TMP36 datasheet
  Serial.print("Temperature C: "); Serial.println(temperatureC);
  // Connect to the server and send the data as a URL parameter
  if(client.connect(host,80)) {
    String url = "/update?value=";
    url += String(temperatureC);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host +  "\r\n" + 
                 "Connection: keep-alive\r\n\r\n"); // minimum set of required URL headers
    delay(10);
    // Read all the lines of the response and print them to Serial
    Serial.println("Response: ");
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
  }
  Serial.println("ESP8266 in sleep mode");
  ESP.deepSleep(sleepTimeSeconds * 1e6);
}

void loop() {
  // put your main code here, to run repeatedly:

}
