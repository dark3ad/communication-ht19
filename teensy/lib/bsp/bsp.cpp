#include <bsp.h>

#define BAUD_RATE (9600U)

void bsp_serial_begin(void)
{
  Serial.begin(BAUD_RATE);
}

void bsp_serial_write(const char *msg)
{
  Serial.write(msg);
}

char bsp_serial_read(void)
{
  return (char)Serial.read();
}

int bsp_serial_available(void)
{
  return Serial.available();
}

bool bsp_serial_dtr()
{
  return (1 == Serial.dtr());
}