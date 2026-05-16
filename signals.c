#include "signals.h"
#include "stats.h"
#include "logger.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

extern int running;

void handle_sigint(int sig) {
    printf("\nArrêt du programme...\n");

    running = 0;

    print_stats();

    log_event("INFO", "Programme arrêté proprement");

    close_logger();

    exit(0);
}

void setup_signals() {
    signal(SIGINT, handle_sigint);
}
