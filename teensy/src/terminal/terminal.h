/**
 * @author      : jesper (jesper@jesper-HP-ENVY-Laptop-13-aq0xxx)
 * @file        : terminal
 * @created     : tisdag nov 10, 2020 13:26:45 CET
 */

#ifndef TERMINAL_H

#define TERMINAL_H

#include <stdint.h>

struct breadcrumbs_t {
    void (*presenters[5])(void);
    void (*handlers[5])(char *);
    uint8_t level;
};

struct canbus_data_t {
    bool (* setter_uint8)(uint8_t value);
    const char * message;
};

int terminal_initialize(void);
void terminal_run(void);

void update_canbus_uint8(char * args);


#endif /* end of include guard TERMINAL_H */

