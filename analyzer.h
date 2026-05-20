#ifndef ANALYZER_H
#define ANALYSER_H

//void analyze_event(int mask,const char *filename);
void push_log(const char *level,
              const char *event,
              const char *filename);
void *analyzer_thread(void *arg);

#endif
