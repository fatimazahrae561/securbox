#include "signals.h"
#include "stats.h"
#include "logger.h"
#include "log_queue.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int running;
char message[512];

void handle_sigint(int sig) {
    printf("\nArrêt du programme...\n");

    running = 0;

    print_stats();

    //log_event("INFO", "Programme arrêté proprement");
    log_message_t log;

    strcpy(log.level, "INFO");
    sprintf(message,"Programme arrêté proprement");
    strcpy(log.message, message);

    log_queue_push(log);	
    close_logger();

    exit(0);
}

void setup_signals() {
    signal(SIGINT, handle_sigint);
}
