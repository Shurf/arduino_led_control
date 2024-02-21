#include "Arduino.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"

#define MAX_OUTPUT_VALUE 255
#define PROFILE_NAME "LED1"

class HttpControl 
{    
public:

    bool getLedProfileParameters(String& profileName, int& red, int& green, int& blue);
    
private:

    String getCurrentLedProfileUrl = "http://led.haven/neon_led_control/led_profiles/get_by_name?led_profile_name=" PROFILE_NAME;
};