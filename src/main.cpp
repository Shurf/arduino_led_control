#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

#include "HttpControl.h"
#include "secrets.h"

#define PIN_RED 26
#define PIN_GREEN 23
#define PIN_BLUE 27

#define INTERVAL_MILLIS 1000

WiFiClientSecure client;
unsigned long lastMillis;
HttpControl* httpControl;


void setup() {
  Serial.begin(115200);
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

  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);

  lastMillis = millis();

  httpControl = new HttpControl();

  Serial.println("Setup finished");
}

void loop() {

  int red;
  int green;
  int blue;

  String profileName;

  unsigned long currentMillis = millis();
  if(currentMillis - lastMillis < INTERVAL_MILLIS && currentMillis >= lastMillis)
    return;

  lastMillis = currentMillis;

  if(!httpControl->getLedProfileParameters(profileName, red, green, blue))
    return;

  analogWrite(PIN_RED, red);
  analogWrite(PIN_GREEN, green);
  analogWrite(PIN_BLUE, blue);

}
