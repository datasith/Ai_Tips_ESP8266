/*------------------------------------------------------------------------------
  07/01/2018
  Author: Makerbro
  Platforms: ESP8266
  Language: C++/Arduino
  File: webserver_html_js.ino
  ------------------------------------------------------------------------------
  Description: 
  Code for YouTube video demonstrating how to use JavaScript in HTML weppages
  that are served in a web server's response.
  https://youtu.be/ZJoBy2c1dPk

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

  server.on("/",[](){server.send_P(200,"text/html", webpage);});
  server.on("/ledstate",getLEDState);
  server.begin();
}

void loop()
{
  server.handleClient();
}

void toggleLED()
{
  digitalWrite(pin_led,!digitalRead(pin_led));
}

void getLEDState()
{
  toggleLED();
  String led_state = digitalRead(pin_led) ? "ON" : "OFF";
  server.send(200,"text/plain", led_state);
}
