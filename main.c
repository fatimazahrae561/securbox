#include<stdio.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<pthread.h>
#include <string.h>
#include "logger.h"
#include "monitor.h"
#include "stats.h"
#include "signals.h"
#include "analyzer.h"
#include "queue.h"
#include "log_queue.h"

char monitored_path[256];

int main(int argc, char *argv[]){

if(argc != 2){
printf("Usage: %s <directory>\n", argv[0]);
return 1;
}
struct stat st;
if(stat(argv[1],&st) == -1){
perror("Erreur chemin");
return 1;
}
if(!S_ISDIR(st.st_mode)){
printf("Ce n'est pas un dossier\n");
return 1;
}
printf("Dossier valide: %s\n", argv[1]);

strncpy(monitored_path, argv[1], sizeof(monitored_path)-1);

init_logger("logs/securebox.log");

init_stats();

setup_signals();

pthread_t monitor_tid;
pthread_t analyzer_tid;
pthread_t logger_tid;

pthread_create(&monitor_tid,
               NULL,
               monitor_thread,
               argv[1]);

pthread_create(&analyzer_tid,
               NULL,
               analyzer_thread,
               NULL);

pthread_create(&logger_tid,
               NULL,
               logger_thread,
               NULL);

pthread_join(monitor_tid, NULL);

pthread_join(analyzer_tid, NULL);

pthread_join(logger_tid, NULL);

// ================= CLEANUP =================

queue_destroy();

log_queue_destroy();

close_logger();


return 0;

}
