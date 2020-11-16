/**
 * @author      : jesper (jesper@jesper-HP-ENVY-Laptop-13-aq0xxx)
 * @file        : terminal
 * @created     : tisdag nov 10, 2020 13:28:56 CET
 */

#include <terminal.h>
#include <common.h>

static enum btn_type {
  LINK,
  OVERWRITTE,
  READ
};

static typedef struct btn_t {
  btn_type type;
  char * label;
  char * link;
  uint8_t visible;
};

static typedef struct menu_t {
  char * label;
  struct button_t * buttons;
};

/* MAIN MENU BUTTONS & MENU */
btn_t main_menu_snap = {LINK, "Get snapshop of the system", "snapshots"};


int terminal_initialize(void)
{
  // Check the CAN-bus status.
  // Todo: check the CAN-bus status.

  // Check the SD card status.
  if(get_sdcard_status != OKAY)
  {
    // Then, turn it on, how hard can it be?
    set_sdcard_status(OKAY);
  }
  
  // Everything is fine, you are good to go!
  return 0;
}

int terminal_run(const char cmd)
{
  // Display the navigation.
  //
  // Send a message.
  return 0;
}
