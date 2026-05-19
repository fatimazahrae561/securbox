#ifndef LOGGER_H
#define LOGGER_H


void init_logger(const char *logfile);
void *logger_thread(void *arg);
//void log_event(const char *level , const char *message);
void close_logger();

#endif
