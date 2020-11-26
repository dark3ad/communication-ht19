/**
 * @author      : jesper (jesper@jesper-HP-ENVY-Laptop-13-aq0xxx)
 * @file        : main
 * @created     : onsdag nov 11, 2020 10:29:49 CET
 */

#include<terminal.h>
#include<Arduino.h>
#include<bsp.h>

void setup()
{
  bsp_serial_begin();
  delay(2000);
  terminal_initialize();
}

void loop()
{
  terminal_run();
}
