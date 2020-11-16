/**
 * @author      : jesper (jesper@jesper-HP-ENVY-Laptop-13-aq0xxx)
 * @file        : terminal
 * @created     : tisdag nov 10, 2020 13:28:56 CET
 */

#include <stdint.h>
#include <terminal.h>
#include <canbus.h>
#include <common.h>

enum btn_type {
  LINK,
  OVERWRITTE,
  READ
};

struct btn_t {
  btn_type type;
  const char * label;
  const char * link;
  uint8_t visible;
};

struct menu_t {
  const char * label;
  struct btn_t * buttons;
};


/* MAIN MENU BUTTONS & MENU */
static btn_t main_menu_btns[3];
static menu_t main_menu;

static menu_t * current_menu;

int terminal_initialize(void)
{
  main_menu_btns[0] = {LINK, "Get snapshop of the system.", "snapshots", 1};
  main_menu_btns[1] = {LINK, "Get list of logged files.", "logged_files", 1};
  main_menu_btns[2] = {LINK, "Calibrate the system.", "calibration", 1};

  main_menu = {"main_menu", main_menu_btns};
  current_menu = &main_menu;

  // Check the CAN-bus status.
  // Todo: check the CAN-bus status.

  // Check the SD card status.
  if(get_sdcard_status() != OKAY)
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
