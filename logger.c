#include "logger.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

static FILE *log_file =NULL;
static char log_path[256];

void init_logger(const char *logfile){
strncpy(log_path, logfile, sizeof(log_path) -1);
log_file = fopen(log_path, "a");
if(!log_file){
perror("Erreur ouverture log file");
exit(1);
}
printf("Ouverture de log file!");
}

void log_event(const char *level, const char *message){
if(!log_file) return;

time_t now = time(NULL);
struct tm *t = localtime(&now);

char time_str[64];
strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);

fprintf(log_file, "[%s] [%s] %s\n",time_str, level,message);
//sert a forcer l'ecriture immediate dans logger
fflush(log_file);
}

void close_logger(){
if(log_file){
fprintf(log_file, "[INFO] Logger ferme proprement\n");
fclose(log_file);
log_file = NULL;
}
}
