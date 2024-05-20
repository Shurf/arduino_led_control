#include "Arduino.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"

#define MAX_OUTPUT_VALUE 255
#define PROFILE_NAME "LED1"

struct Arguments {
    String profileName;
    int red;
    int green;
    int blue;
    int percentage;
    bool is_current;
    int secondaryRed;
    int secondaryGreen;
    int secondaryBlue;
    int secondaryRed2;
    int secondaryGreen2;
    int secondaryBlue2;
    bool enabled;
    bool useSecondary;
};

class HttpControl 
{    
public:

    bool getLedProfileParameters(String& profileName, int& red, int& green, int& blue, int& percentage);
    Arguments getLedProfileFullParameters();
    
private:

    String getCurrentLedProfileUrl = "http://led.haven/neon_led_control/led_profiles/get_by_name?led_profile_name=" PROFILE_NAME;
};