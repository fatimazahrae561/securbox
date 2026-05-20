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

    printf("[THREAD] Arret threads\n");

    push_log("INFO","Arret threads","Monitor,analyzer,logger");

    printf("hhh");
   queue_stop();
    
   push_log("INFO","Arret Queue","Monito->analyzer");

    log_queue_stop();

    push_log("INFO","Arret Log Queue","analyzer->logger");


    print_stats();


    //log_event("INFO", "Programme arrêté proprement");
    //log_message_t log;
push_log("INFO","SYSYTEM STOP","securebox");
exit(0);
}

void setup_signals() {
    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigint);
}

