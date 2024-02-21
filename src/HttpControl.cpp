#include "HttpControl.h"

bool HttpControl::getLedProfileParameters(String& profileName, int& red, int& green, int& blue)
{
    HTTPClient http;
    String serverResponse;
    JsonDocument doc;
    http.begin(getCurrentLedProfileUrl);

    if(http.GET() != 200)
    {
        Serial.println("HTTP Error");
        return false;
    }

    serverResponse = http.getString();
    deserializeJson(doc, serverResponse);

    profileName = ((const char*)doc["profile_name"]);

    if((bool)doc["enabled"] == false)
    {
        red = 0;
        green = 0;
        blue = 0;
        return true;
    }

    if((bool)doc["use_secondary"] == false)
    {
        red = (int)doc["red"];
        green = (int)doc["green"];
        blue = (int)doc["blue"];
    }
    else
    {
        red = (int)doc["secondary_red"];
        green = (int)doc["secondary_green"];
        blue = (int)doc["secondary_blue"];
    }

    return true;
}
