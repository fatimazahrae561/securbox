#include<stdio.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<pthread.h>
#include "logger.h"
#include "monitor.h"

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

init_logger("logs/securebox.log");

pthread_t thread;
//lance le thread monitor
pthread_create(&thread, NULL, monitor_thread, argv[1]);
//attendre la fin 
pthread_join(thread, NULL);


return 0;

}
