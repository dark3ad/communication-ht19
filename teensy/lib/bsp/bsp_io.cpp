#include <bsp.h>
#include <bsp_io.h>

void bsp_pin_mode(uint8_t pin, uint8_t mode)
{
    pinMode(pin, mode);
}

void bsp_digital_write(uint8_t pin, uint8_t val)
{
    digitalWrite(pin, val);
}

uint8_t bsp_serial_available()
{
  Serial.available();
}

void bsp_serial_write(uint8_t * buf, uint16_t len)
{
  Serial.write(buf, len);
}

char bsp_serial_read(void)
{
  return (char)Serial.read();
}

uint8_t bsp_digital_read(uint8_t pin)
{
    return digitalRead(pin);
}
