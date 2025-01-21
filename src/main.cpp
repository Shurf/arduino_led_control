#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

#include "HttpControl.h"
#include "NeoPixelLed.h"
#include "secrets.h"


//#define NEOPIXEL_LED_COUNT 16

// cross
#define NEOPIXEL_LED_COUNT 357

// ankh
//#define NEOPIXEL_LED_COUNT 298

// sound strip
// #define NEOPIXEL_LED_COUNT 88

#define NEOPIXEL_LED_PIN 26
#define NEOPIXEL_BRIGHTNESS 255
// #define NEOPIXEL_BRIGHTNESS 8
// #define NEOPIXEL_BRIGHTNESS 32



// NeoPixelLed * neoPixelLed = new NeoPixelLed(357, 255, NEOPIXEL_LED_PIN, false); // cross 
// NeoPixelLed * neoPixelLed = new NeoPixelLed(298, 255, NEOPIXEL_LED_PIN, true); // ankh 
// NeoPixelLed * neoPixelLed = new NeoPixelLed(24, 8, NEOPIXEL_LED_PIN, false); // circle
NeoPixelLed * neoPixelLed = new NeoPixelLed(20, 255, NEOPIXEL_LED_PIN, false); // rabbit

#define INTERVAL_MILLIS 1000

WiFiClientSecure client;
unsigned long lastMillis;
HttpControl* httpControl;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Setup started");

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, WIFIPASSWORD);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("WIFI Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  lastMillis = millis();

  httpControl = new HttpControl();

  Serial.println("Setup finished");
}

void loop() 
{

  unsigned long currentMillis = millis();
  if(currentMillis - lastMillis < INTERVAL_MILLIS && currentMillis >= lastMillis)
    return;

  lastMillis = currentMillis;


  auto arguments = httpControl->getLedProfileFullParameters();
  neoPixelLed->display(arguments);
}
