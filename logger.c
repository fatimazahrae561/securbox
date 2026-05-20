#include "logger.h"
#include "log_queue.h"
#include "monitor.h"

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

static FILE *log_file = NULL;
static char log_path[256];

void init_logger(const char *logfile) {

    struct stat st = {0};

    if (stat("logs", &st) == -1) {
        mkdir("logs", 0700);
    }

    strncpy(log_path, logfile, sizeof(log_path) - 1);

    log_file = fopen(log_path, "a");

    if (!log_file) {
        perror("Erreur ouverture log file");
        exit(1);
    }

    printf("Logger initialisé\n");
}

void log_event_full(log_message_t log) {

    if (!log_file) return;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    char date[32];
    char hour[32];

    strftime(date, sizeof(date),
             "%Y-%m-%d", t);

    strftime(hour, sizeof(hour),
             "%H:%M:%S", t);

    pid_t pid = getpid();

    fprintf(log_file,
        "[%s] [%s] [%s] [%s] [%s] [%s] PID:%d\n",
        date,
        hour,
        log.level,
        log.event,
        log.resource,
        log.path,
        pid
    );

    fflush(log_file);
}

void close_logger() {

    if (log_file) {

        fprintf(log_file,
                "[INFO] Logger ferme proprement\n");

        fclose(log_file);
        log_file = NULL;
    }
}

// ================= THREAD LOGGER =================

void *logger_thread(void *arg) {

    while (running) {

        log_message_t log =
            log_queue_pop();

        log_event_full(log);
    }

    return NULL;
}  

