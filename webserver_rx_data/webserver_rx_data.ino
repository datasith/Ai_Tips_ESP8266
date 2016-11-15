#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>

ESP8266WebServer server;
uint8_t pin_led = 16;
char* ssid = "AcroboticGuest";
char* password = "13meetup37";

#define PIN D1
Adafruit_NeoPixel strip = Adafruit_NeoPixel(20, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  strip.begin();
  strip.show();

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
  server.on("/setcolor",setPixelColor);
  server.on("/reset",resetPixelColor);
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

void setPixelColor()
{
  String pixel_num = server.arg("pixel_num");
  String r = server.arg("red");
  String g = server.arg("grn");
  String b = server.arg("blu");
  strip.setPixelColor(pixel_num.toInt(), r.toInt(), g.toInt(), b.toInt());
  strip.show();
  String rsp = "LED #"+pixel_num+" is set to: "+r+", "+g+", "+b;
  server.send(200,"text/plain",rsp);
}

void resetPixelColor()
{
  strip.clear();
  strip.show();
  server.send(200,"text/plain","All LEDs are off!");
}
