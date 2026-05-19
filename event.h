#ifndef EVENT_H
#define EVENT_H

#include <time.h>

typedef struct {
int mask;
char filename[256];
time_t timestamp;
} event_t;

#endif
