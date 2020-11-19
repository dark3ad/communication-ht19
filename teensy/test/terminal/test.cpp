#ifdef TARGET
#include<Arduino.h>
#endif

#include<unity.h>
#include<bsp_io.h>
#include<terminal.h>

uint8_t bsp_serial_available(void)
{
  return 1;
}

char bsp_serial_read(void)
{
  return getchar();
}

void setUp()
{

}

void tearDown()
{

}

#ifdef TARGET
void run()
#else
int run()
#endif
{
  UNITY_BEGIN();

  terminal_initialize();
  terminal_run();

#ifdef TARGET
  UNITY_END();
#else
  return UNITY_END();
#endif
}

#ifdef TARGET
void loop()
{
}

void setup()
{
  delay(3000);
  run();
}
#else
int main()
{
  run();
}
#endif
