#include "analyzer.h"
#include "logger.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/inotify.h>

#define DELETE_THRESHOLD 5
#define DELETE_TIME_WINDOW 10

#define MODIFY_THRESHOLD 3
#define MODIFY_TIME_WINDOW 5

//Structures pour tracking
typedef struct {
int count;
time_t first_time;
}counter_t;

//variables globales
static counter_t delete_counter = {0, 0};
static counter_t modify_counter = {0, 0};

//verifier extension interdite 
int is_forbidden_extension(const char *filename){
const char *extensions[] = {".sh", ".exe", ".bat", ".py"};
int n = sizeof(extensions) / sizeof(extensions[0]);
for (int i=0 ; i < n ;i++){
int len_file = strlen(filename);
int len_ext = strlen(extensions[i]);

if(len_file >= len_ext && strcmp(filename + len_file - len_ext, extensions[i]) == 0){
return 1;
}
}
return 0;
}
//verifier fichier cache
int is_hidden_file(const char *filename){
return filename[0] == '.';
}
void analyze_event(int mask, const char *filename){
time_t now = time(NULL);
char message[512];

//============CREATE==============
if(mask & IN_CREATE){
sprintf(message, "CREATE FILE : %s", filename);
log_event("INFO", message);

//fichier cache
if(is_hidden_file(filename)){
sprintf(message, "Fichier cache detecte : %s",filename);
log_event("ALERT", message);
printf("[ALERTE] %s\n", message);
}
//extension interdite
if(is_forbidden_extension(filename)){
sprintf(message, "Extension interdite detectee : %s",filename);
log_event("ALERT", message);
printf("[ALERTE] %s\n", message);
}
}
//============DELETE===============
if(mask  & IN_DELETE){
sprintf(message, "DELETE FILE : %s", filename);
log_event("INFO", message);
//compteur
if(delete_counter.count == 0){
delete_counter.first_time = now;
}
delete_counter.count++;

if(difftime(now, delete_counter.first_time <= DELETE_TIME_WINDOW)){
if(delete_counter.count >= DELETE_THRESHOLD){
sprintf(message, "Activite suspecte: %d suppressions en %d secondes",delete_counter.count, DELETE_TIME_WINDOW);
log_event("ALERT",message);
printf("[ALERTE] %s\n", message);
delete_counter.count = 0;
}
}else{
delete_counter.count = 1;
delete_counter.first_time = now;
}
}
//============MODIFY===============
if(mask  & IN_MODIFY){
sprintf(message, "MODIFY FILE : %s", filename);
log_event("INFO", message);
//compteur
if(modify_counter.count == 0){
modify_counter.first_time = now;
}
modify_counter.count++;

if(difftime(now, delete_counter.first_time <= MODIFY_TIME_WINDOW)){
if(modify_counter.count >= MODIFY_THRESHOLD){
sprintf(message, "Activite suspecte: %d modificatiosn rapides (%s)",modify_counter.count, filename);
log_event("ALERT",message);
printf("[ALERTE] %s\n", message);
modify_counter.count = 0;
}
}else{
modify_counter.count = 1;
modify_counter.first_time = now;
}
}
}



