#include "HttpControl.h"

Arguments HttpControl::getLedProfileFullParameters()
{
    HTTPClient http;
    String serverResponse;
    JsonDocument doc;
    Arguments result;

    http.begin(getCurrentLedProfileUrl);

    if(http.GET() != 200)
    {
        Serial.println("HTTP Error");
        return result;
    }

    serverResponse = http.getString();
    deserializeJson(doc, serverResponse);

    result.profileName = ((const char*)doc["profile_name"]);
    result.percentage = (int)doc["percentage"];
    result.enabled = (bool)doc["enabled"];
    result.useSecondary = (bool)doc["use_secondary"];
    result.red = (int)doc["red"];
    result.green = (int)doc["green"];
    result.blue = (int)doc["blue"];    
    result.secondaryRed = (int)doc["secondary_red"];
    result.secondaryGreen = (int)doc["secondary_green"];
    result.secondaryBlue = (int)doc["secondary_blue"];
    result.secondaryRed2 = (int)doc["secondary_red2"];
    result.secondaryGreen2 = (int)doc["secondary_green2"];
    result.secondaryBlue2 = (int)doc["secondary_blue2"];

    return result;
}

bool HttpControl::getLedProfileParameters(String& profileName, int& red, int& green, int& blue, int& percentage)
{
    Arguments arguments = getLedProfileFullParameters();

    percentage = arguments.percentage;
    profileName = arguments.profileName;

    if(arguments.enabled == false)
    {
        red = 0;
        green = 0;
        blue = 0;
        return true;
    }

    if(arguments.useSecondary == false)
    {
        red = arguments.red;
        green = arguments.green;
        blue = arguments.blue;
    }
    else
    {
        red = arguments.secondaryRed;
        green = arguments.secondaryGreen;
        blue = arguments.secondaryBlue;
    }
    

    return true;
}
