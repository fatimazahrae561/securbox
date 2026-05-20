#include "monitor.h"
#include "logger.h"
#include "analyzer.h"
#include "queue.h"
#include "event.h"
#include <string.h>
#include <time.h>
#include <sys/inotify.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

#define BUFFER_SIZE 1024

volatile sig_atomic_t  running = 1; 

void *monitor_thread(void *arg){
	char *path =(char *)arg;
	int fd= inotify_init();
if (fd == -1){
perror("inotify_init");
return NULL;
}
//surveillance
int wd = inotify_add_watch(
    fd,
    path,

    IN_CREATE |
    IN_DELETE |
    IN_MODIFY |
    IN_MOVED_FROM |
    IN_MOVED_TO
);
if(wd == -1){
perror("inotify_add_watch");
return NULL;
}
char buffer[BUFFER_SIZE];

while(running){
//read remplit buffer
int length = read(fd, buffer, BUFFER_SIZE);
if(length <0){
perror("read");
break;
}
int i=0;
while (i< length){
struct inotify_event *event = (struct inotify_event *)&buffer[i];

/*if(event->mask & IN_CREATE){
printf("CREATE: %s\n", event->name);
char msg[256];
sprintf(msg,"CREATE FILE :  %s\n", event->name);
log_event("INFO",msg);
}
if(event->mask & IN_DELETE){
printf("DELETE: %s\n", event->name);
char msg[256];
sprintf(msg,"DELETE FILE :  %s\n", event->name);
log_event("INFO",msg);
}
if(event->mask & IN_MODIFY){
printf("MODIFY: %s\n", event->name);
char msg[256];
sprintf(msg,"MODIFIED FILE :  %s\n", event->name);
log_event("INFO",msg);
}*/

if(event->len > 0){

    event_t new_event;

    new_event.mask = event->mask;

    strcpy(new_event.filename,
           event->name);

    new_event.timestamp = time(NULL);

    // =========================
    // CREATION DOSSIER
    // =========================

    if ((event->mask & IN_CREATE) &&
        (event->mask & IN_ISDIR)) {

        printf("CREATE DIRECTORY : %s\n",
               event->name);
    }

    // =========================
    // SUPPRESSION DOSSIER
    // =========================

    else if ((event->mask & IN_DELETE) &&
             (event->mask & IN_ISDIR)) {

        printf("DELETE DIRECTORY : %s\n",
               event->name);
    }

    // =========================
    // RENOMMAGE / DEPLACEMENT
    // =========================

    else if (event->mask & IN_MOVED_FROM) {

        printf("MOVED FROM : %s\n",
               event->name);
    }

    else if (event->mask & IN_MOVED_TO) {

        printf("MOVED TO : %s\n",
               event->name);
    }

    // =========================
    // FICHIERS NORMAUX
    // =========================

    else if (event->mask & IN_CREATE) {

        printf("CREATE FILE : %s\n",
               event->name);
    }

    else if (event->mask & IN_DELETE) {

        printf("DELETE FILE : %s\n",
               event->name);
    }

    else if (event->mask & IN_MODIFY) {

        printf("MODIFY FILE : %s\n",
               event->name);
    }

    queue_push(new_event);
}


i += sizeof(struct inotify_event) + event->len;
}
}
close(fd);
return NULL;
}

