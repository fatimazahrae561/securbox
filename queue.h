#ifndef QUEUE_H
#define QUEUE_H

#include "event.h"

#define MAX_QUEUE 100

void queue_push(event_t event);
event_t queue_pop();

#endif
