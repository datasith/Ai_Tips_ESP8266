#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ACROBOTIC_SSD1306.h>
#include <ArduinoJson.h>

ESP8266WebServer server;
uint8_t pin_led = 16;
char* ssid = "AcroboticGuest";
char* password = "13meetup37";

void setup()
{
  Wire.begin();
  oled.init();
  oled.clearDisplay();
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
  Serial.print(WiFi.localIP());

  server.on("/",[](){server.send(200,"text/plain","Hello World!");});
  server.on("/toggle",toggleLED);
  server.on("/displaydata", HTTP_OPTIONS, displayDataOptions);
  server.on("/displaydata", HTTP_POST, displayData);
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

void displayDataOptions()
{
  server.sendHeader("Access-Control-Allow-Origin","*");
  server.send(204,"");
}

void displayData()
{
  String data = server.arg("plain");
  StaticJsonBuffer<200> jBuffer;
  JsonObject& jObject = jBuffer.parseObject(data);
  String msg = jObject["message"];
  oled.setTextXY(0,0);
  oled.putString(msg);
  server.send(204,"");
}
