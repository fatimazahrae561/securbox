#ifndef MONITOR_H
#define MONITOR_H
#include <signal.h>

extern volatile sig_atomic_t running;
void *monitor_thread(void *arg);

#endif


