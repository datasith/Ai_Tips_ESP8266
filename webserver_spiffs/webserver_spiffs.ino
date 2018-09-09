/*------------------------------------------------------------------------------
  08/01/2018
  Author: Makerbro
  Platforms: ESP8266
  Language: C++/Arduino
  File: webserver_spiffs.ino
  ------------------------------------------------------------------------------
  Description: 
  Code for YouTube video demonstrating how to use the File System (SPIFFS) in 
  Flash Memory to read and write files containing HTML and JavaScript.
  https://youtu.be/pfJROpQg-Is

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
#include <FS.h>

ESP8266WebServer server;
uint8_t pin_led = 2;
char* ssid = "YOUR_SSID";
char* password = "YOUR_PASSWORD";

char webpage[] PROGMEM = R"=====(
<html>
<head>
</head>
<body>
<p> LED Status: <span id="led-state">__</span> </p>
<button onclick="myFunction()"> TOGGLE </button>
</body>
<script>
function myFunction()
{
  console.log("button was clicked!");
  var xhr = new XMLHttpRequest();
  var url = "/ledstate";

  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("led-state").innerHTML = this.responseText;
    }
  };

  xhr.open("GET", url, true);
  xhr.send();
};
document.addEventListener('DOMContentLoaded', myFunction, false);
</script>
</html>
)=====";

void setup()
{
  SPIFFS.begin();
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

  server.on("/",serveIndexFile);
  server.on("/ledstate",getLEDState);
  server.begin();
}

void loop()
{
  server.handleClient();
}

void serveIndexFile()
{
  File file = SPIFFS.open("/index.html","r");
  server.streamFile(file, "text/html");
  file.close();
}

void toggleLED()
{
  digitalWrite(pin_led,!digitalRead(pin_led));
}

void getLEDState()
{
  toggleLED();
  String led_state = digitalRead(pin_led) ? "OFF" : "ON";
  server.send(200,"text/plain", led_state);
}
