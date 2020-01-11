/*------------------------------------------------------------------------------
  07/01/2018
  Author: Makerbro
  11/01/2020
  added Code for esp32 and esp8266
  Platforms: ESP8266
  Language: C++/Arduino
  File: webserver_websockets.ino
  ------------------------------------------------------------------------------
  Description: 
  Code for YouTube video demonstrating how to transfer data between a web server
  and a web client in real-time using websockets.
  https://youtu.be/ROeT-gyYZfw
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
#if defined(ESP2866)
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
#endif

#if defined(ESP32)
  #include <WiFi.h>
  #include <WebServer.h>
#endif

#include <WebSocketsServer.h>

#if defined(ESP2866)
  ESP8266WebServer server;
#endif

#if defined(ESP32)
  WebServer server;
#endif

WebSocketsServer webSocket = WebSocketsServer(81);


char* ssid = "ssid";
char* password = "password";

  
int ledPin = 2;                // the blue board led
int ledChannel = 2;
int ledFreq    = 5000;
int ledResolution = 10;        // 2**10 = 1024

uint16_t brightness = 0;


char webpage[] PROGMEM = R"=====(
<html>
<head>
  <script>
    var Socket;
    function init() {
      Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
      Socket.onmessage = function(event){
        document.getElementById("rxConsole").value += event.data;
      }
    }
    function sendText(){
      Socket.send(document.getElementById("txBar").value);
      document.getElementById("txBar").value = "";
    }
    function sendBrightness(){
      Socket.send("#"+document.getElementById("brightness").value);
    }    
  </script>
</head>
<body onload="javascript:init()">
  <div>
    <textarea id="rxConsole"></textarea>
  </div>
  <hr/>
  <div>
    <input type="text" id="txBar" onkeydown="if(event.keyCode == 13) sendText();" />
  </div>
  <hr/>
  <div>
    <input type="range" min="0" max="1023" value="0" id="brightness" oninput="sendBrightness()" />
  </div>  
</body>
</html>
)=====";

void setup()
{
  Serial.begin(115200);

  // led setup esp32
  #if defined(ESP32)
    ledcSetup(ledChannel, ledFreq, ledResolution);
    ledcAttachPin(ledPin, ledChannel);
    ledcWrite(ledChannel, 1024);
    delay(800);
    ledcWrite(ledChannel, 0);
  #endif

  // led setup esp2866
   #if defined(ESP2866)
    pinMode(ledPin, OUTPUT);
   #endif
   
  WiFi.begin(ssid,password);
  
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/",[](){
    server.send_P(200, "text/html", webpage);  
  });
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop()
{     
  webSocket.loop();
  server.handleClient();
  if(Serial.available() > 0){
    char c[] = {(char)Serial.read()};
    webSocket.broadcastTXT(c, sizeof(c));
  }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  if(type == WStype_TEXT){
    if(payload[0] == '#'){
      brightness = (uint16_t) strtol((const char *) &payload[1], NULL, 10);
      brightness = 1024 - brightness;
      
      #if defined(ESP2866)
        analogWrite(ledPin, brightness);
      #endif
      
      #if defined(ESP32)
        ledcWrite(ledChannel, brightness);
      #endif
      
      Serial.print("brightness= ");
      Serial.println(brightness);
    }

    else{
      for(int i = 0; i < length; i++)
        Serial.print((char) payload[i]);
      Serial.println();
    }
  }
  
}
