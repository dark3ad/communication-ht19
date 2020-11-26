#include <bsp.h>

#define BAUD_RATE (9600U)

void bsp_serial_begin(void)
{
    Serial.begin(BAUD_RATE);
    while (!Serial)
    {
    }
}

void bsp_serial_print(int value)
{
  Serial.print(value);
}

void bsp_serial_write(const char *msg)
{
  Serial.write(msg);
}

char bsp_serial_read(void)
{
  if(bsp_serial_available() <= 0)
  {
    return 0;
  }

  return (char)Serial.read();
}

int bsp_serial_available(void)
{
  return Serial.available();
}
