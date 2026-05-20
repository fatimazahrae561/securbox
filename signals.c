#include "signals.h"
#include "stats.h"
#include "analyzer.h"
#include "logger.h"
#include "log_queue.h"
#include "queue.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int running;
char message[512];

void handle_sigint(int sig) {  
  printf("\nArrêt du programme...\n");
  
    running = 0;
   queue_stop();
    log_queue_stop();
    print_stats();
exit(0);
}

void setup_signals() {
    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigint);
}

