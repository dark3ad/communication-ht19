
#include "esp32.h"

static void (*request)(void);
static void (*recieve)(size_t);

void setup()
{
    Serial.begin(9600);
    delay(2000);

    RTC_init(request, recieve);
}

void loop()
{
}