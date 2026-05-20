#ifndef MONITOR_H
#define MONITOR_H
#include <signal.h>

extern volatile sig_atomic_t running;
extern char monitored_path[256];
void *monitor_thread(void *arg);

#endif


