#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <Adafruit_NeoPixel.h>

#include "HttpControl.h"
#include "secrets.h"

#define USE_NEOPIXEL

#ifdef USE_NEOPIXEL

#define NEOPIXEL_LED_COUNT 20
#define NO_BLANKS true

#define NEOPIXEL_LED_PIN 26
#define NEOPIXEL_BRIGHTNESS 255
#define SECTOR_MAX_CYCLES 1
#define CURRENT_SECTOR_PERCENT_LENGTH 10
// #define NEOPIXEL_BRIGHTNESS 32

Adafruit_NeoPixel rgbWS = Adafruit_NeoPixel(NEOPIXEL_LED_COUNT, NEOPIXEL_LED_PIN, NEO_GRB + NEO_KHZ800);

#else
#define PIN_RED 26
#define PIN_GREEN 23
#define PIN_BLUE 27
#endif

#define INTERVAL_MILLIS 1000

WiFiClientSecure client;
unsigned long lastMillis;
HttpControl* httpControl;

int currentSectorPercentEnd = 100;
int currentSectorCycles = 0;


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

  #ifdef USE_NEOPIXEL

  rgbWS.begin();
  rgbWS.setBrightness(NEOPIXEL_BRIGHTNESS);

  #else

  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);

  #endif

  lastMillis = millis();

  httpControl = new HttpControl();

  Serial.println("Setup finished");
}

int getLedIndex(int percentage)
{
  return percentage*NEOPIXEL_LED_COUNT/100 + ((percentage*NEOPIXEL_LED_COUNT % 100) == 0 ? 0 : 1);
}

void loop() {

  int currentMax = 0;

  String profileName;

  unsigned long currentMillis = millis();
  if(currentMillis - lastMillis < INTERVAL_MILLIS && currentMillis >= lastMillis)
    return;

  lastMillis = currentMillis;


  #ifdef USE_NEOPIXEL

  auto arguments = httpControl->getLedProfileFullParameters();

  if(arguments.useSecondary)
  {
    /*int sectorEnd = currentSectorPercentEnd*NEOPIXEL_LED_COUNT/100 + ((currentSectorPercentEnd*NEOPIXEL_LED_COUNT % 100) == 0 ? 0 : 1);
    int sectorStart = (currentSectorPercentEnd - CURRENT_SECTOR_PERCENT_LENGTH)*NEOPIXEL_LED_COUNT/100 + 
      (((currentSectorPercentEnd - CURRENT_SECTOR_PERCENT_LENGTH)*NEOPIXEL_LED_COUNT % 100) == 0? 0 : 1);*/
    int sectorEnd = getLedIndex(currentSectorPercentEnd);
    int sectorStart = getLedIndex(currentSectorPercentEnd - CURRENT_SECTOR_PERCENT_LENGTH);
    if(sectorStart < 0)
      sectorStart = 0;
    for(int i = 0; i < NEOPIXEL_LED_COUNT; i++)
    {
      // < and not <= because sectorEnd contains the starting index of the next block
      if(i >= sectorStart && i < sectorEnd)
        rgbWS.setPixelColor(i, arguments.red, arguments.green, arguments.blue);  
      else
        rgbWS.setPixelColor(i, arguments.secondaryRed2, arguments.secondaryGreen2, arguments.secondaryBlue2);
      currentSectorCycles++;      
    }
    if(currentSectorCycles >= SECTOR_MAX_CYCLES)
    {
      currentSectorCycles = 0;
      currentSectorPercentEnd -= CURRENT_SECTOR_PERCENT_LENGTH;
      if(currentSectorPercentEnd <= 0)
        currentSectorPercentEnd = 100;
    }
  }
  else
  {
    currentMax = getLedIndex(arguments.percentage);

    for(int i = 0; i < currentMax; i++)
      rgbWS.setPixelColor(i, arguments.red, arguments.green, arguments.blue);

    for(int i = currentMax; i < NEOPIXEL_LED_COUNT; i++)
    {
#ifdef NO_BLANKS
      rgbWS.setPixelColor(i, arguments.secondaryRed2, arguments.secondaryGreen2, arguments.secondaryBlue2);
#else
      rgbWS.setPixelColor(i, 0, 0, 0);
#endif
    }
  }

  

  rgbWS.show();

  #else

  if(!httpControl->getLedProfileParameters(profileName, red, green, blue, percentage))
    return;

  analogWrite(PIN_RED, red);
  analogWrite(PIN_GREEN, green);
  analogWrite(PIN_BLUE, blue);
  #endif

}
