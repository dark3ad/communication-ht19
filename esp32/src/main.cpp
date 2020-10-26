
//#include <bsp.h>
#include <wifi_driver.h>

#define WIFI "Machete"
#define PASS "locoloco"
void setup()
{
}

void loop()
{
    Serial.begin(9600);
    wifi_driver_init(WIFI, PASS);
    if (wifi_status_driver() != OK)
    {
        Serial.print(".");
        delay(500);
    }
}